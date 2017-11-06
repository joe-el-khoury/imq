#include <zmqpp/zmqpp.hpp>

#include "Node.hpp"

Node::Node (unsigned server_port) : server_port_(server_port)
{
  // Init the server socket. Do not init the client socket because we might not
  // even need it.
  server_socket_ = new zmqpp::socket(ctx_, zmqpp::socket_type::pair);
  std::string address = "tcp://*:" + std::to_string(server_port_);
  server_socket_->bind(address);
}
