#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

#include "IMQNode.hpp"
#include "../utils/Common.hpp"

IMQNode::IMQNode (const std::string& host, unsigned server_port)
{
  rpc_server_ = new rpc::RPCServer(host, server_port);
}

IMQNode::IMQNode (unsigned server_port)
{
  rpc_server_ = new rpc::RPCServer(server_port);
}

IMQNode::~IMQNode ()
{
  if (rpc_server_) {
    delete rpc_server_;
  }
}

void IMQNode::Run ()
{
}
