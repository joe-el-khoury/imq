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
  const HostAndPort& GetHostAndPort () const
  {
    return host_and_port_;
  }

  const std::string& GetHost () const
  {
    return GetHostAndPort().host;
  }

  unsigned GetPort () const
  {
    return GetHostAndPort().port;
  }
};

} // namespace rpc

#endif // RPCOBJECT_HPP
