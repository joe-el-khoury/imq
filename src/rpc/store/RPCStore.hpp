#ifndef RPCSTORE_HPP
#define RPCSTORE_HPP

#include "../RPCObject.hpp"

#include "../../../utils/HostAndPort.hpp"

#include <unordered_map>

#include <memory>

namespace rpc {

class RPCStore
{
private:
  std::unordered_map<HostAndPort, std::shared_ptr<rpc::RPCObject>> rpc_objects_;

  void AddRPCObject (const std::shared_ptr<rpc::RPCObject>&);

  bool HasRPCObject (const HostAndPort&);
  std::shared_ptr<rpc::RPCObject> GetRPCObject(const HostAndPort&);
};

} // namespace rpc

#endif // RPCSTORE_HPP
