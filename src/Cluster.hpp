#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <unordered_map>
#include <vector>

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
  // Store the nodes in this map.
  // Key is host and port, value is the rpc client connected to that node.
  std::unordered_map<HostAndPort, rpc::RPCClient*> nodes_;
  // To quickly get the nodes in the cluster.
  std::vector<HostAndPort> host_and_ports_;

  Cluster ();

public:
  Cluster (const Cluster&) = delete;
  void operator= (const Cluster&) = delete;

  // Singleton.
  static Cluster& GetInstance ();
  
  void AddNode (const std::string&, unsigned port);
  bool NodeInCluster (const std::string&, unsigned port);
};

#endif // CLUSTER_HPP
