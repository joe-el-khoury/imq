#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

#include "IMQNode.hpp"

#include "../utils/Common.hpp"
#include "Cluster.hpp"

#include "rpc/RPCCall.hpp"

IMQNode::IMQNode (const std::string& host, unsigned server_port, bool first_node_in_cluster)
{
  rpc_server_ = std::make_shared<rpc::RPCServer>(host, server_port);

  cluster = &Cluster::GetInstance();
  if (first_node_in_cluster) {
    cluster->AddNode(host, server_port);
  }
}

void IMQNode::BootstrapCluster (
    const std::string& cluster_node_hostname, unsigned cluster_node_port)
{
}

void IMQNode::Run ()
{
  // rpc_server_->Add(...);
  // rpc_server_->Run();
}
