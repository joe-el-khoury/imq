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

  auto nodes = json_response["nodes"];
  // Add the cluster members.
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    const std::string& hostname = it.key();
    for (unsigned port : it.value()) {
      AddNode(hostname, port);
    }
  }
}

bool Cluster::NodeInCluster (const std::string& hostname, unsigned port)
{
  HostAndPort host_and_port(hostname, port);
  return std::find(nodes_.begin(), nodes_.end(), host_and_port) != nodes_.end();
}

void Cluster::AddNode (const std::string& hostname, unsigned port)
{
  HostAndPort host_and_port(hostname, port);
  if (!NodeInCluster(hostname, port)) {
    nodes_.push_back(host_and_port);
  }
}

void Cluster::RemoveNode (const std::string& hostname, unsigned port)
{
  if (!NodeInCluster(hostname, port)) {
    return;
  }

  auto erase_iter = std::find(nodes_.begin(), nodes_.end(), HostAndPort(hostname, port));
  nodes_.erase(erase_iter);
}

std::shared_ptr<rpc::RPCClient> Cluster::GetNodeClient (const std::string& hostname, unsigned port)
{
  if (!NodeInCluster(hostname, port)) {
    throw std::logic_error("Node not found.");
  }

  return rpc_client_store_.GetRPCClient(hostname, port);
}

const std::vector<HostAndPort>& Cluster::GetNodesInCluster ()
{
  return nodes_;
}
