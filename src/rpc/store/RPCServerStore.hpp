#ifndef RPCSERVERSTORE_HPP
#define RPCSERVERSTORE_HPP

#include "rpc/store/RPCStore.hpp"

#include "rpc/RPCServer.hpp"

#include <memory>

namespace rpc {

class RPCServerStore
{
private:
  RPCStore* rpc_store_ = &RPCStore::GetInstance();

  std::shared_ptr<rpc::RPCServer> GetRPCServer (const std::string&, unsigned);
};

} // namespace rpc

#endif // RPCSERVERSTORE_HPP
