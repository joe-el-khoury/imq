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

    // A message can consist of multiple frames or parts.
    int num_parts = received_message.parts();
    std::vector<std::string> message_parts(num_parts, "");
    for (int part_number = 0; part_number < num_parts; ++part_number) {
      std::string part;
      received_message >> part;
      
      message_parts[part_number] = part;
    }
    
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
