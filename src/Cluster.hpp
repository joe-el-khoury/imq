#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "json.hpp"
#include "rpc/RPCClient.hpp"
#include "rpc/store/RPCClientStore.hpp"
#include "utils/HostAndPort.hpp"
#include "State.hpp"

#include <unordered_map>
#include <list>

#include <memory>

class Cluster
{
private:
  using json = nlohmann::json;

  rpc::RPCClientStore rpc_client_store_;
  
  std::vector<utils::HostAndPort> nodes_;

  bool cluster_has_leader_ = false;
  utils::HostAndPort leader_;

  Cluster ();

public:
  Cluster (const Cluster&) = delete;
  void operator= (const Cluster&) = delete;

  // Singleton.
  static Cluster& GetInstance ();

  bool HasLeader ()
  {
    return cluster_has_leader_;
  }
  utils::HostAndPort GetLeader ()
  {
    return leader_;
  }
  void SetLeader (const utils::HostAndPort& leader)
  {
    leader_ = leader;
    cluster_has_leader_ = true;
  }

  // Bootstrap the cluster by connecting to the node with cluster info.
  void Bootstrap (const std::string&, unsigned, const std::string&, unsigned);
  
  void AddNode (const std::string&, unsigned port);
  void AddNode (const utils::HostAndPort& hap)
  {
    AddNode(hap.host, hap.port);
  }
  
  void RemoveNode (const std::string&, unsigned port);
  void RemoveNode (const utils::HostAndPort& hap)
  {
    RemoveNode(hap.host, hap.port);
  }
  
  bool NodeInCluster (const std::string&, unsigned port);
  bool NodeInCluster (const utils::HostAndPort& hap)
  {
    return NodeInCluster(hap.host, hap.port);
  }

  std::shared_ptr<rpc::RPCClient> GetNodeClient (const std::string&, unsigned port);
  std::shared_ptr<rpc::RPCClient> GetNodeClient (const utils::HostAndPort& hap)
  {
    return GetNodeClient(hap.host, hap.port);
  }

  const std::vector<utils::HostAndPort>& GetNodesInCluster ();
};

#endif // CLUSTER_HPP
