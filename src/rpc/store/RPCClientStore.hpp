#ifndef RPCCLIENTSTORE_HPP
#define RPCCLIENTSTORE_HPP

#include "RPCStore.hpp"

#include "../RPCClient.hpp"

#include <memory>

namespace rpc {

class RPCClientStore
{
private:
  RPCStore* rpc_store_ = &RPCStore::GetInstance();

  std::shared_ptr<rpc::RPCClient> GetRPCClient (const std::string&, unsigned);
};

} // namespace rpc

#endif // RPCCLIENTSTORE_HPP
