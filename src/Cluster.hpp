#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <unordered_map>

class Cluster
{
private:
  // Store the nodes in this map.
  // Key is the hostname, value is the port.
  std::unordered_map<std::string, unsigned> nodes_;

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
