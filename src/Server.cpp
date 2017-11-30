#include "Server.hpp"
#include "../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

Server::Server (const std::string& host, unsigned port) : host_(host), port_(port), ctx_()
{
  server_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::rep);
}

Server::Server (unsigned port) : port_(port), ctx_()
{
  server_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::rep);
}

Server::~Server ()
{
  if (server_socket_) {
    delete server_socket_;
  }
}

void Server::Run ()
{
  utils::BindSocket(server_socket_, host_, port_);
}

void Server::AddRequest (const std::string& request_uri, const RequestFunc& request_func)
{
  requests_.insert({request_uri, request_func});
}

Server::Response Server::PerformRequest (const std::string& request, const json& args)
{
  auto got = requests_.find(request);
  if (got == requests_.end()) {
    throw std::invalid_argument("Request " + request + " not supported");
  }

  RequestFunc& func = got->second;
  return func(args);
}
