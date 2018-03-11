#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <unordered_map>

#include "IMQNode.hpp"

class Cluster
{
private:
  // Store the nodes in this map.
  // Key is the hostname, value is the port.
  std::unordered_map<std::string, unsigned> nodes_;

public:
  void AddNode (const std::string&, unsigned port);
};

#endif // CLUSTER_HPP
