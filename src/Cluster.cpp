#include "Cluster.hpp"

#include "rpc/RPCCall.hpp"
#include "rpc/RPCResponse.hpp"
#include "utils/HostAndPort.hpp"

#include <string>
#include <stdexcept>

Cluster::Cluster ()
{
  for (int i = 0; i < MAX_NUM_NODES; ++i) {
    nodes_.push_back(utils::HostAndPort());
  }
}

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
    unsigned idx = std::stoi(it.key(), nullptr);
    for (const std::string& node_str : it.value()) {
      utils::HostAndPort node = utils::HostAndPort::FromString(node_str);

      AddNode(idx, node.host, node.port);
    }
  }
}

bool Cluster::NodeInCluster (const std::string& hostname, unsigned port)
{
  utils::HostAndPort finding(hostname, port);
  for (const utils::HostAndPort& node : nodes_) {
    if (node == finding) {
      return true;
    }
  }
  
  return false;
}

void Cluster::AddNode (const std::string& hostname, unsigned port)
{
  for (int i = 0; i < MAX_NUM_NODES; ++i) {
    if (nodes_[i].host == "") {
      nodes_[i] = utils::HostAndPort(hostname, port);
      break;
    }
  }
}

void Cluster::AddNode (unsigned idx, const std::string& hostname, unsigned port)
{
  // We'll assume there's nothing at this index.
  nodes_[idx] = utils::HostAndPort(hostname, port);
}

void Cluster::RemoveNode (const std::string& hostname, unsigned port)
{
  utils::HostAndPort to_remove(hostname, port);
  for (int i = 0; i < MAX_NUM_NODES; ++i) {
    utils::HostAndPort node = nodes_[i];
    if (node == to_remove) {
      nodes_[i] = utils::HostAndPort();
    }
  }
}

std::shared_ptr<rpc::RPCClient> Cluster::GetNodeClient (const std::string& hostname, unsigned port)
{
  if (!NodeInCluster(hostname, port)) {
    throw std::logic_error("Node not found.");
  }

  return rpc_client_store_.GetRPCClient(hostname, port);
}

std::vector<std::pair<int, utils::HostAndPort>> Cluster::GetNodesWithIdxInCluster ()
{
  std::vector<std::pair<int, utils::HostAndPort>> nodes;
  for (int i = 0; i < MAX_NUM_NODES; ++i) {
    utils::HostAndPort node = nodes_[i];

    nodes.push_back({i, node});
  }

  return nodes;
}

std::vector<utils::HostAndPort> Cluster::GetNodesInCluster ()
{
  return nodes_;
}
