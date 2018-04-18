#include "Cluster.hpp"

#include "rpc/RPCCall.hpp"
#include "rpc/RPCResponse.hpp"

#include "utils/HostAndPort.hpp"

#include <stdexcept>

Cluster::Cluster ()
{}

Cluster& Cluster::GetInstance ()
{
  static Cluster instance;
  return instance;
}

void Cluster::Bootstrap (const std::string& requester_host, unsigned requester_port, const std::string& leader_host, unsigned leader_port)
{
  if (!NodeInCluster(leader_host, leader_port)) {
    AddNode(leader_host, leader_port);
  }

  std::shared_ptr<rpc::RPCClient> node = GetNodeClient(leader_host, leader_port);
  rpc::RPCCall rpc_call("GetNodesInCluster",
      {{"host", requester_host}, {"port", requester_port}, {"register_node", true}});
  rpc::RPCResponse rpc_response = node->Call(rpc_call);

  json json_response = rpc_response.Get();

  // Add the cluster members.
  for (auto it = json_response.begin(); it != json_response.end(); ++it) {
    const std::string& hostname = it.key();
    for (unsigned port : it.value()) {
      AddNode(hostname, port);
    }
  }
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
  if (!NodeInCluster(hostname, port)) {
    host_and_ports_.push_back(host_and_port);
    // nullptr because we will lazy-load the rpc clients if needed.
    nodes_.insert({{hostname, port}, nullptr});
  }
}

std::shared_ptr<rpc::RPCClient> Cluster::GetNodeClient (const std::string& hostname, unsigned port)
{
  if (!NodeInCluster(hostname, port)) {
    throw std::logic_error("Node not found.");
  }

  return rpc_client_store_.GetRPCClient(hostname, port);
}

std::vector<std::pair<std::string, unsigned>> Cluster::GetNodesInCluster ()
{
  std::vector<std::pair<std::string, unsigned>> nodes;
  for (const HostAndPort& host_and_port : host_and_ports_) {
    const std::string& host = host_and_port.host;
    unsigned port = host_and_port.port;

    nodes.push_back({host, port});
  }

  return nodes;
}
