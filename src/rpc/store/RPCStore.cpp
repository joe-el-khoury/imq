#include "RPCStore.hpp"

#include "../RPCObject.hpp"

#include "../../../utils/HostAndPort.hpp"

bool rpc::RPCStore::HasRPCObject (const HostAndPort& host_and_port)
{
  return (rpc_objects_.find(host_and_port) != rpc_objects_.end());
}

std::shared_ptr<rpc::RPCObject> rpc::RPCStore::GetRPCObject (const HostAndPort& host_and_port)
{
  if (HasRPCObject(host_and_port)) {
    return rpc_objects_[host_and_port];
  
  } else {
    return nullptr;
  }
}
