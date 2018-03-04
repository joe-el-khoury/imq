#include "RPCResponse.hpp"

#include <chrono>

rpc::RPCResponse::RPCResponse (zmqpp::socket* socket) : socket_(socket)
{
  received_.store(false);
  did_message_callback_.store(false);
  message_thread_ = new std::thread(&rpc::RPCResponse::CheckMessageReceipt, this);
}

rpc::RPCResponse::RPCResponse (const RPCResponse& other)
{
  received_.store(other.received_.load());

  socket_ = other.socket_;
  message_thread_ = other.message_thread_;

  message_callback_ = other.message_callback_;
  did_message_callback_.store(other.did_message_callback_.load());
}

rpc::RPCResponse::~RPCResponse ()
{
  message_thread_->join();
  delete message_thread_;
}

void rpc::RPCResponse::DoMessageCallback (zmqpp::message& message)
{
  if (!message_callback_) {
    // To avoid getting a bad_function error when calling the callback.
    return;
  }
  
  if (did_message_callback_.load()) {
    return;
  }
  
  std::string message_str;
  message >> message_str;

  message_callback_(json::parse(message_str));

  did_message_callback_.store(true);
}

void rpc::RPCResponse::DoTimeoutCallback ()
{
  if (timeout_callback_) {
    timeout_callback_();
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
  if (!timeout_callback_set_) {
    // Can't timeout if no timeout is set!
    return false;
  }
  
  unsigned now = CurrentTime();
  return (now - start_time_) >= timeout_duration_;
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

void rpc::RPCResponse::OnMessageReceipt (std::function<void(const json&)> func)
{
  message_callback_ = func;
}

void rpc::RPCResponse::OnTimeout (unsigned timeout, std::function<void()> func)
{
  start_time_ = CurrentTime();
  timeout_duration_ = timeout;
  timeout_callback_ = func;
  timeout_callback_set_ = true;
}
