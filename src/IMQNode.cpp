#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

#include "IMQNode.hpp"
#include "../utils/Common.hpp"

IMQNode::IMQNode (unsigned server_port) : server_port_(server_port)
{
  // Init the server socket. Do not init the client socket because we might not
  // even need it.
  utils::InitAndBindSocket(ctx_, server_socket_, zmqpp::socket_type::pair, server_port_);
}

void IMQNode::Run ()
{
  running_.store(true);
  while (running_.load()) {
    // Do stuff
  }
}
