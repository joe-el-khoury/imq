#ifndef RPCSERVERSTORE_HPP
#define RPCSERVERSTORE_HPP

#include "RPCStore.hpp"

#include "../RPCServer.hpp"

#include <memory>

namespace rpc {

class RPCServerStore
{
private:
  RPCStore* rpc_store_ = &RPCStore::GetInstance();

  std::shared_ptr<rpc::RPCServer> NewRPCServer (const std::string&, unsigned);
};

} // namespace rpc

#endif // RPCSERVERSTORE_HPP
