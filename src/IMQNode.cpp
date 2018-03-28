#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

#include "IMQNode.hpp"

#include "../utils/Common.hpp"
#include "Cluster.hpp"

#include "rpc/RPCCall.hpp"

IMQNode::json IMQNode::GetNodesInCluster (const json& j)
{
  // Whether to register the calling node in the cluster.
  // I did it this way to reduce the number of RPCs needed to add a node
  // to a cluster (we could have had a separate RegisterNode RPC).
  bool register_node = j["register_node"];
  
  if (register_node) {
    const std::string& requester_host = j["host"];
    unsigned requester_port = j["port"];
    
    if (!cluster->NodeInCluster(requester_host, requester_port)) {
      cluster->AddNode(requester_host, requester_port);
    }
  }
  
  std::vector<std::pair<std::string, unsigned>> nodes = cluster->GetNodesInCluster();
  
  json ret;
  for (const auto& node : nodes) {
    const std::string& host = node.first;
    unsigned port = node.second;

    if (ret.find(host) != ret.end()) {
      unsigned temp = ret[host];

      ret[host] = std::vector<unsigned>();
      ret[host].push_back(temp);
      ret[host].push_back(port);
    
    } else {
      ret[host] = port;
    }
  }

  return ret;
}

IMQNode::IMQNode (const std::string& host, unsigned server_port, bool first_node_in_cluster)
{
  rpc_server_ = std::make_shared<rpc::RPCServer>(host, server_port);

  cluster = &Cluster::GetInstance();
  cluster->AddNode(host, server_port);
  rpc_server_->AddRPC("GetNodesInCluster", std::bind(&IMQNode::GetNodesInCluster, this, std::placeholders::_1));
}

void IMQNode::JoinCluster (
    const std::string& cluster_node_hostname, unsigned cluster_node_port)
{
  cluster->Bootstrap(rpc_server_->GetHost(), rpc_server_->GetPort(), cluster_node_hostname, cluster_node_port);
}

void IMQNode::Run ()
{
  // rpc_server_->Add(...);
  // rpc_server_->Run();
  rpc_server_->Run();
}
