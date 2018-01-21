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
  server_thread_->join();
  if (server_socket_) {
    delete server_socket_;
  }
}

std::vector<std::string> Server::MessageIntoParts (zmqpp::message& message)
{
  int num_parts = message.parts();
  if (num_parts == 0) {
    return std::vector<std::string>();
  }
  
  // A message can consist of multiple frames/parts.
  std::vector<std::string> parts(num_parts, "");
  for (int i = 0; i < num_parts; ++i) {
    std::string part;
    message >> part;

    parts[i] = part;
  }

  return parts;
}

void Server::RunServer ()
{
  utils::BindSocket(server_socket_, host_, port_);
  running_.store(true);

  while (true) {
    zmqpp::message received_message;

    bool received = server_socket_->receive(received_message, /*dont_block*/ true);
    if (!received) {
      continue;
    }

    std::vector<std::string> parts = MessageIntoParts(received_message);
    
    // crashes if we don't send anything.
    server_socket_->send("test");
  }
}

void Server::Run ()
{
  server_thread_ = new std::thread(&Server::RunServer, this);
}

void Server::AddRPC (const std::string& rpc_name, const RPCFunc& rpc_func)
{
  rpcs_.insert({rpc_name, rpc_func});
}

Server::Response Server::PerformRPC (const std::string& rpc, const json& args)
{
  auto got = rpcs_.find(rpc);
  if (got == rpcs_.end()) {
    throw std::invalid_argument("rpc " + rpc + " not supported");
  }

  RPCFunc& func = got->second;
  return func(args);
}
