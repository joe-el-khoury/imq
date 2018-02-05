#include "RPCResponse.hpp"

RPCResponse::RPCResponse (zmqpp::socket* socket) : socket_(socket)
{
  received_.store(false);
  message_thread_ = new std::thread(&RPCResponse::CheckMessageReceipt, this);
}

RPCResponse::~RPCResponse ()
{
  message_thread_->join();
  delete message_thread_;
}

void RPCResponse::CheckMessageReceipt ()
{
  while (true) {
    bool received = socket_->receive(received_message_, /* dont_block */ true);
    received_.store(received);

    if (received) {
      std::string message_str;
      received_message_ >> message_str;

      message_callback_(json::parse(message_str));
    }
  }
}
