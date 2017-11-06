#include <zmqpp/zmqpp.hpp>

#include "Node.hpp"

Node::Node (unsigned server_port) :
  client_socket_(ctx_, zmqpp::socket_type::pair),
  server_socket_(ctx_, zmqpp::socket_type::pair)
{
}
