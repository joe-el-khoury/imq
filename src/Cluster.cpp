#include "Cluster.hpp"

Cluster::Cluster ()
{}

Cluster& Cluster::GetInstance ()
{
  static Cluster instance;
  return instance;
}

bool Cluster::NodeInCluster (const std::string& hostname, unsigned port)
{
  return (nodes_.find(hostname) != nodes_.end());
}

void Cluster::AddNode (const std::string& hostname, unsigned port)
{
  if (NodeInCluster(hostname, port)) {
    // We should just overwrite. This'll probably never happen though.
    nodes_[hostname] = port;
  } else {
    nodes_.insert({hostname, port});
  }
}
