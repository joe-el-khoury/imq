#include "RPC.hpp"

rpc::utils::RPCMessage rpc::utils::ReceiveMessage (zmqpp::socket* socket)
{
  zmqpp::message received_message;
  bool received = socket->receive(received_message);

  rpc::utils::RPCMessage ret;
  ret.received = received;
  ret.message = std::move(received_message);

  return ret;
}
