#include "rpc/store/RPCServerStore.hpp"

#include "rpc/RPCObject.hpp"
#include "rpc/RPCServer.hpp"

#include "utils/HostAndPort.hpp"

std::shared_ptr<rpc::RPCServer> rpc::RPCServerStore::GetRPCServer (const std::string& host, unsigned port)
{
  utils::HostAndPort host_and_port(host, port);
  if (rpc_store_->HasRPCObject(host_and_port)) {
    return std::static_pointer_cast<rpc::RPCServer>(rpc_store_->GetRPCObject(host_and_port));
  }

  std::shared_ptr<rpc::RPCServer> rpc_server(new rpc::RPCServer(host, port));
  rpc_store_->AddRPCObject(rpc_server);

  return rpc_server;
}
