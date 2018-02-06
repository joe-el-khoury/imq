#include "RPCResponse.hpp"

rpc::utils::RPCResponse::RPCResponse (zmqpp::socket* socket) : socket_(socket)
{
  received_.store(false);
  did_callback_.store(false);
  message_thread_ = new std::thread(&rpc::utils::RPCResponse::CheckMessageReceipt, this);
}

rpc::utils::RPCResponse::~RPCResponse ()
{
  message_thread_->join();
  delete message_thread_;
}

void rpc::utils::RPCResponse::DoCallback (zmqpp::message& message)
{
  if (did_callback_.load()) {
    return;
  }
  
  std::string message_str;
  message >> message_str;

  message_callback_(json::parse(message_str));

  did_callback_.store(true);
}

void rpc::utils::RPCResponse::CheckMessageReceipt ()
{
  while (true) {
    bool received = socket_->receive(received_message_, /* dont_block */ true);
    received_.store(received);

    if (received) {
      DoCallback(received_message_);
    }
  }
}

void rpc::utils::RPCResponse::OnMessageReceipt (std::function<void(const json&)> func)
{
  message_callback_ = func;
  if (received_.load()) {
    DoCallback(received_message_);
  }
}
