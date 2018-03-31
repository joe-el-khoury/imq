#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <unordered_map>
#include <vector>

#include <memory>

#include "json.hpp"

#include "rpc/RPCClient.hpp"

struct HostAndPort
{
  std::string host;
  unsigned port;

  HostAndPort (const std::string& host_, unsigned port_)
  {
    host = host_;
    port = port_;
  }

  bool operator== (const HostAndPort& other) const
  {
    return host == other.host && port == other.port;
  }
};
namespace std
{
template<>
struct hash<HostAndPort>
{
  std::size_t operator() (const HostAndPort& host_and_port) const
  {
    const std::string& host = host_and_port.host;
    unsigned port = host_and_port.port;
    
    return std::hash<std::string>()(host + std::to_string(port));
  }
};
}

class Cluster
{
private:
  using json = nlohmann::json;
  
  // Store the nodes in this map.
  // Key is host and port, value is the rpc client connected to that node.
  std::unordered_map<HostAndPort, std::shared_ptr<rpc::RPCClient>> nodes_;
  // To quickly get the nodes in the cluster.
  std::vector<HostAndPort> host_and_ports_;

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

  std::vector<std::pair<std::string, unsigned>> GetNodesInCluster ();
};

#endif // CLUSTER_HPP
