#include "rpc/store/RPCClientStore.hpp"

#include "rpc/RPCObject.hpp"
#include "rpc/RPCClient.hpp"

#include "utils/HostAndPort.hpp"

std::shared_ptr<rpc::RPCClient> rpc::RPCClientStore::GetRPCClient (const std::string& host, unsigned port)
{
  HostAndPort host_and_port(host, port);
  if (rpc_store_->HasRPCObject(host_and_port)) {
    return std::static_pointer_cast<rpc::RPCClient>(rpc_store_->GetRPCObject(host_and_port));
  }

  std::shared_ptr<rpc::RPCClient> rpc_client(new rpc::RPCClient(host, port));
  rpc_store_->AddRPCObject(rpc_client);

  return rpc_client;
}
