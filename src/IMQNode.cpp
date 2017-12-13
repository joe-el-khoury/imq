#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

#include "IMQNode.hpp"
#include "../utils/Common.hpp"

IMQNode::IMQNode (const std::string& host, unsigned server_port)
{
  server_ = new Server(host, server_port);
}

IMQNode::IMQNode (unsigned server_port)
{
  server_ = new Server(server_port);
}

IMQNode::~IMQNode ()
{
  if (server_) {
    delete server_;
  }
}

void IMQNode::Run ()
{
}
