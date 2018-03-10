#include "RPCResponse.hpp"

#include <chrono>

rpc::RPCResponse::RPCResponse (zmqpp::socket* socket, const rpc::RPCCall& rpc_call) : socket_(socket), rpc_call_(rpc_call)
{
  received_.store(false);
  message_thread_ = new std::thread(&rpc::RPCResponse::CheckMessageReceipt, this);
}

rpc::RPCResponse::RPCResponse (const RPCResponse& other) : rpc_call_(other.rpc_call_)
{
  received_.store(other.received_.load());

  socket_ = other.socket_;
  message_thread_ = other.message_thread_;
}

rpc::RPCResponse::~RPCResponse ()
{
  message_thread_->join();
  delete message_thread_;
}

void rpc::RPCResponse::DoMessageCallback (zmqpp::message& message)
{
  if (!rpc_call_.MessageCallbackIsSet()) {
    // To avoid getting a bad_function error when calling the callback.
    return;
  }
  
  std::string message_str;
  message >> message_str;

  rpc_call_.GetMessageCallback()(json::parse(message_str));
}

void rpc::RPCResponse::DoTimeoutCallback ()
{
  if (rpc_call_.TimeoutIsSet()) {
    rpc_call_.GetTimeoutCallback()();
  }
}

unsigned rpc::RPCResponse::CurrentTime ()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();
}

bool rpc::RPCResponse::HasTimedOut ()
{
  if (!rpc_call_.TimeoutIsSet()) {
    // Can't timeout if no timeout is set!
    return false;
  }
  
  unsigned now = CurrentTime();
  return (now - start_time_) >= rpc_call_.GetTimeoutDuration();
}

void rpc::RPCResponse::CheckMessageReceipt ()
{
  zmqpp::message received_message;
  while (true) {
    bool received = socket_->receive(received_message, true);
    received_.store(received);

    if (received_.load()) {
      DoMessageCallback(received_message);
      return;
    }
    if (HasTimedOut()) {
      DoTimeoutCallback();
      return;
    }
  }
}
