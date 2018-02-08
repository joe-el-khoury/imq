#include "RPC.hpp"

rpc::RPCMessage rpc::ReceiveMessage (zmqpp::socket* socket)
{
  zmqpp::message received_message;
  bool received = socket->receive(received_message);

  rpc::RPCMessage ret;
  ret.received = received;
  ret.message = std::move(received_message);

  return ret;
}
