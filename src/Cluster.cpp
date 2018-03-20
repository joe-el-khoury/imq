#include "Cluster.hpp"

#include <stdexcept>

Cluster::Cluster ()
{}

Cluster& Cluster::GetInstance ()
{
  static Cluster instance;
  return instance;
}

bool Cluster::NodeInCluster (const std::string& hostname, unsigned port)
{
  HostAndPort host_and_port(hostname, port);
  bool in_vector = (std::find(host_and_ports_.begin(), host_and_ports_.end(), host_and_port) != host_and_ports_.end());
  bool in_map = nodes_.find(host_and_port) != nodes_.end();
  
  return in_vector && in_map;
}

void Cluster::AddNode (const std::string& hostname, unsigned port)
{
  HostAndPort host_and_port(hostname, port);
  if (NodeInCluster(hostname, port)) {
    nodes_[host_and_port] = nullptr;
  } else {
    // nullptr because we will lazy-load the rpc clients if needed.
    nodes_.insert({{hostname, port}, nullptr});
  }
}

rpc::RPCClient* Cluster::GetRPCClient (const std::string& hostname, unsigned port)
{
  if (!NodeInCluster(hostname, port)) {
    throw std::logic_error("Node not found.");
  }

  HostAndPort host_and_port(hostname, port);
  rpc::RPCClient* rpc_client = nodes_[host_and_port];

  if (!rpc_client) {
    rpc_client = new rpc::RPCClient(hostname, port);
  }

  return rpc_client;
}
