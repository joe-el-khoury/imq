#include <stdexcept>

#include "IMQNode.hpp"

#include "utils/Common.hpp"
#include "utils/HostAndPort.hpp"

#include "meta/MetaStore.hpp"

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
  
  std::vector<HostAndPort> nodes = cluster->GetNodesInCluster();
  
  json ret;
  // The nodes will reside in the nodes_part of the json.
  ret["nodes"] = {};
  auto& nodes_part = ret["nodes"];
  
  for (const auto& node : nodes) {
    const std::string& host = node.host;
    unsigned port = node.port;

    if (nodes_part.find(host) != nodes_part.end()) {
      nodes_part[host].push_back(port);
    
    } else {
      nodes_part[host] = std::vector<unsigned>(1, port);
    }
  }

  return ret;
}

IMQNode::IMQNode (const std::string& host, unsigned port)
{
  rpc_server_ = rpc_server_store_.GetRPCServer(host, port);

  cluster = &Cluster::GetInstance();
  cluster->AddNode(host, port);
  rpc_server_->AddRPC("GetNodesInCluster", std::bind(&IMQNode::GetNodesInCluster, this, std::placeholders::_1));
}

IMQNode::~IMQNode ()
{
  rpc_server_->Stop();
}

void IMQNode::JoinCluster (
    const std::string& leader_hostname, unsigned leader_port)
{
  cluster->Bootstrap(MetaStore::GetHost(), MetaStore::GetPort(), leader_hostname, leader_port);
}

void IMQNode::Run ()
{
  health_checker_.Run();
  rpc_server_->Run();
}
