#ifndef RPCOBJECT_HPP
#define RPCOBJECT_HPP

#include "../../utils/HostAndPort.hpp"

namespace rpc {

// This is a useless object, only here for polymorphic purposes for the RPCStore.
class RPCObject
{
private:
  HostAndPort host_and_port_;

protected:
  void SetHostAndPort (const std::string& host, unsigned port)
  {
    host_and_port_.host = host;
    host_and_port_.port = port;
  }

public:
  const HostAndPort& GetHostAndPort ()
  {
    return host_and_port_;
  }
};

} // namespace rpc

#endif // RPCOBJECT_HPP
