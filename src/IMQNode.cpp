#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

#include "IMQNode.hpp"

std::string ConstructTCPBindAddress (unsigned port)
{
  return "tcp://*:" + std::to_string(port);
}

void IMQNode::InitAndBindSocket (zmqpp::socket* socket, zmqpp::socket_type socket_type, unsigned port_to_bind_to)
{
  socket = new zmqpp::socket(ctx_, socket_type);
  std::string address = ConstructTCPBindAddress(port_to_bind_to);
  socket->bind(address);
}

IMQNode::IMQNode (unsigned server_port) : server_port_(server_port)
{
  // Init the server socket. Do not init the client socket because we might not
  // even need it.
  InitAndBindSocket(server_socket_, zmqpp::socket_type::pair, server_port_);
}
