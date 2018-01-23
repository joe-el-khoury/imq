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

Server::RPCFunc& Server::GetRPC (const std::string& rpc)
{
  auto got = rpcs_.find(rpc);
  if (got == rpcs_.end()) {
    throw std::invalid_argument("RPC " + rpc + " not found.");
  }

  return got->second;
}

Server::Message Server::ReceiveMessage (zmqpp::socket* socket)
{
  zmqpp::message received_message;
  bool received = socket->receive(received_message, /*dont_block*/ true);

  Message message;
  message.received = received;
  message.message = std::move(received_message);
  
  return message;
}

Server::RPCAndArgs Server::Message::ToParts ()
{
  int num_parts = message.parts();
  if (num_parts == 0 || num_parts >= 3) {
    return {false, "", ""};
  }

  RPCAndArgs ret;
  ret.valid = true;
  
  std::string rpc;
  message >> rpc;
  ret.rpc = rpc;

  std::string args_str;
  message >> args_str;
  json args;
  try {
    args = json::parse(args_str);
  } catch (const std::invalid_argument& e) {
    // Parse error.
    ret.valid = false;
  }
  ret.args = args;

  return ret;
}

void Server::RunServer ()
{
  utils::BindSocket(server_socket_, host_, port_);
  running_.store(true);

  while (running_.load()) {
    Server::Message received_message = ReceiveMessage(server_socket_);
    if (!received_message.received) {
      continue;
    }
    
    Server::RPCAndArgs rpc_and_args = received_message.ToParts();
    if (!rpc_and_args.valid) {
      server_socket_->send("invalid");
    } else {
      server_socket_->send("valid");
    }
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
