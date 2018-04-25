#ifndef NODE_HPP
#define NODE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>
#include <string>

#include <memory>

#include "json.hpp"

#include "rpc/RPCServer.hpp"
#include "rpc/RPCClient.hpp"

#include "Cluster.hpp"

class IMQNode
{
private:
  using json = nlohmann::json;
  
  std::shared_ptr<rpc::RPCServer> rpc_server_;
  
  Cluster* cluster;

  json RegisterNode (const json&);
  json GetNodesInCluster (const json&);

public:
  IMQNode (const std::string&, unsigned);
  ~IMQNode ();

  // Join the cluster by asking the leader for cluster info.
  void JoinCluster (const std::string&, unsigned);

  void Run ();
};

#endif // NODE_HPP
