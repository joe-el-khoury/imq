#ifndef RPC_HPP
#define RPC_HPP

#include <zmqpp/zmqpp.hpp>

namespace rpc {

struct RPCMessage
{
  bool received;
  zmqpp::message message;
};

RPCMessage ReceiveMessage (zmqpp::socket* socket);

}

#endif // RPC_HPP
