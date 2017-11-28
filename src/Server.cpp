#include "Server.hpp"

#include <zmqpp/zmqpp.hpp>

Server::Server (unsigned port) : port_(port), ctx_()
{
  server_socket_ = new zmqpp::socket(ctx_, zmqpp::socket_type::rep);
}

Server::~Server ()
{
  if (server_socket_) {
    delete server_socket_;
  }
}

void Server::RunServer ()
{
  server_socket_->bind("tcp://*" + std::to_string(port_));
}
