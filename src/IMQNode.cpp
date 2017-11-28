#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

#include "IMQNode.hpp"
#include "../utils/Common.hpp"

IMQNode::IMQNode (unsigned server_port) : server_port_(server_port)
{
  // Init the server socket. Do not init the client socket because we might not
  // even need it.
  server_socket_ = utils::GetInitSocket(ctx_, zmqpp::socket_type::pair);
  utils::BindSocket(server_socket_, server_port_);
}

void IMQNode::Run ()
{
  running_.store(true);
  while (running_.load()) {
    // Do stuff
  }
}
