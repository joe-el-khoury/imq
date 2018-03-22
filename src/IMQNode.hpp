#ifndef NODE_HPP
#define NODE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>
#include <string>

#include <memory>

#include "rpc/RPCServer.hpp"
#include "rpc/RPCClient.hpp"

#include "Cluster.hpp"

class IMQNode
{
private:
  std::shared_ptr<rpc::RPCServer> rpc_server_;
  
  Cluster* cluster;

public:
  IMQNode (const std::string&, unsigned, bool);

  // Bootstrapping means getting the cluster up and running.
  void BootstrapCluster (const std::string&, unsigned);

  void Run ();
};

#endif // NODE_HPP
