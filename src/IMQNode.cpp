#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

#include "IMQNode.hpp"

#include "../utils/Common.hpp"
#include "Cluster.hpp"

#include "rpc/RPCCall.hpp"

IMQNode::IMQNode (const std::string& host, unsigned server_port)
{
  rpc_server_ = new rpc::RPCServer(server_port);

  cluster = &Cluster::GetInstance();
}

IMQNode::~IMQNode ()
{
  if (rpc_server_) {
    delete rpc_server_;
  }
}

void IMQNode::Run ()
{
  // rpc_server_->Add(...);
  // rpc_server_->Run();
}
