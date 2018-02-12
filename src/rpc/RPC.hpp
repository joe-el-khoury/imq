#ifndef RPC_HPP
#define RPC_HPP

#include <zmqpp/zmqpp.hpp>

namespace rpc {

struct RPCMessage
{
  bool received;
  zmqpp::message message;

  RPCMessage () {
    received = false;
  }

  RPCMessage (const RPCMessage& other) {
    received = other.received;
    message = other.message.copy();
  }
  RPCMessage (RPCMessage& other) {
    received = other.received;
    message = other.message.copy();
  }
};

RPCMessage ReceiveMessage (zmqpp::socket* socket);

}

#endif // RPC_HPP
