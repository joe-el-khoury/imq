#include "rpc/RPCResponse.hpp"

#include "rpc/exception/RPCException.hpp"

#include <chrono>

#include <stdexcept>

rpc::RPCResponse::RPCResponse (zmqpp::socket* socket, const rpc::RPCCall& rpc_call)
  : socket_(socket), rpc_call_(rpc_call)
{
  received_.store(false);
  timedout_.store(false);
  
  if (rpc_call_.IsAsync()) {
    message_thread_ = new std::thread(
        &rpc::RPCResponse::MessageReceiptMainWorkLoop, this, true);
  }
}

rpc::RPCResponse::RPCResponse (const RPCResponse& other) : rpc_call_(other.rpc_call_)
{
  received_.store(other.received_.load());
  timedout_.store(other.timedout_.load());

  socket_ = other.socket_;
  message_thread_ = other.message_thread_;
}

rpc::RPCResponse::~RPCResponse ()
{
  running_.store(false);
  if (rpc_call_.IsAsync()) {
    message_thread_->join();
    delete message_thread_;
    message_thread_ = nullptr;
  }
}

void rpc::RPCResponse::AddResponseMetadata (json& j)
{
  j["callee_info"]["host"] = rpc_call_.GetCalleeHost();
  j["callee_info"]["port"] = rpc_call_.GetCalleePort();
}

void rpc::RPCResponse::DoMessageCallback (zmqpp::message& message)
{
  if (!rpc_call_.MessageCallbackIsSet()) {
    // To avoid getting a bad_function error when calling the callback.
    return;
  }
  
  std::string message_str;
  message >> message_str;

  json parsed = json::parse(message_str);
  AddResponseMetadata(parsed);
  
  rpc_call_.GetMessageCallback()(parsed);
}

void rpc::RPCResponse::DoTimeoutCallback ()
{
  if (!rpc_call_.TimeoutIsSet()) {
    return;
  }

  json error = rpc::except::TimeoutException().AsJson();
  AddResponseMetadata(error);

  rpc_call_.GetTimeoutCallback()(error);
}

unsigned rpc::RPCResponse::CurrentTime ()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();
}

bool rpc::RPCResponse::HasTimedOut ()
{
  unsigned now = CurrentTime();
  return (now - start_time_) >= rpc_call_.GetTimeoutDuration();
}

void rpc::RPCResponse::MessageReceiptMainWorkLoop (bool do_cb=true)
{
  running_.store(true);
  start_time_ = CurrentTime();
  
  while (running_.load()) {
    bool received = socket_->receive(received_message_, true);
    received_.store(received);

    if (received_.load()) {
      if (do_cb) {
        DoMessageCallback(received_message_);
      }
      
      return;
    }
    if (HasTimedOut()) {
      timedout_.store(true);
      if (do_cb) {
        DoTimeoutCallback();
      }
      
      return;
    }
  }
}

// Synchronously get the result of the call.
rpc::RPCResponse::json rpc::RPCResponse::Get ()
{
  if (rpc_call_.IsAsync()) {
    throw std::runtime_error("Cannot get on an async call.");
  }

  MessageReceiptMainWorkLoop(false);
  
  // Once the main loop exits, we check whether we timed out or have a message.
  
  if (received_.load()) {
    std::string message_str;
    received_message_ >> message_str;

    json parsed = json::parse(message_str);
    AddResponseMetadata(parsed);

    return parsed;
  
  } else if (timedout_.load()) {
    return rpc::except::TimeoutException().AsJson();
  
  } else {
    // Will never get here, but just in case.
    return rpc::except::InternalException().AsJson();
  }
}
