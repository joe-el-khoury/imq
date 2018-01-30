#ifndef RPCUTILS_HPP
#define RPCUTILS_HPP

#include <zmqpp/zmqpp.hpp>

namespace rpc { namespace utils
{

struct RPCMessage
{
  bool received;
  zmqpp::message message;
};

}}

#endif // RPCUTILS_HPP
