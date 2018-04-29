#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "json.hpp"

#include "rpc/RPCClient.hpp"
#include "rpc/store/RPCClientStore.hpp"

#include "utils/HostAndPort.hpp"

#include <unordered_map>
#include <list>

#include <memory>

class Cluster
{
private:
  using json = nlohmann::json;

  rpc::RPCClientStore rpc_client_store_;
  
  std::vector<HostAndPort> nodes_;

  Cluster ();

public:
  Cluster (const Cluster&) = delete;
  void operator= (const Cluster&) = delete;

  // Singleton.
  static Cluster& GetInstance ();

  // Bootstrap the cluster by connecting to the node with cluster info.
  void Bootstrap (const std::string&, unsigned, const std::string&, unsigned);
  
  void AddNode (const std::string&, unsigned port);
  bool NodeInCluster (const std::string&, unsigned port);

  std::shared_ptr<rpc::RPCClient> GetNodeClient (const std::string&, unsigned port);

  const std::vector<HostAndPort>& GetNodesInCluster ();
};

#endif // CLUSTER_HPP
