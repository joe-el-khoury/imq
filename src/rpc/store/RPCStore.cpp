#include "rpc/store/RPCStore.hpp"

#include "rpc/RPCObject.hpp"

#include "utils/HostAndPort.hpp"

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

void rpc::RPCStore::AddRPCObject (const std::shared_ptr<rpc::RPCObject>& rpc_object)
{
  if (!rpc_object) {
    return;
  }

  // I'll assume the client calling this has checked if the RPC object exists.
  rpc_objects_.insert({rpc_object->GetHostAndPort(), rpc_object});
}
