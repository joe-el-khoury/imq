#include "Cluster.hpp"

Cluster::Cluster ()
{}

Cluster& Cluster::GetInstance ()
{
  static Cluster instance;
  return instance;
}

void Cluster::AddNode (const std::string& hostname, unsigned port)
{
  bool in_map = (nodes_.find(hostname) != nodes_.end());

  if (in_map) {
    // We should just overwrite. This'll probably never happen though.
    nodes_[hostname] = port;
  } else {
    nodes_.insert({hostname, port});
  }
}
