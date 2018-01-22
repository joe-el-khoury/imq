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

zmqpp::message Server::ReceiveMessage (zmqpp::socket* socket)
{
  zmqpp::message received_message;
  socket->receive(received_message, /*dont_block*/ true);
  return received_message;
}

Server::RPCAndArgs Server::MessageIntoParts (zmqpp::message& message)
{
  int num_parts = message.parts();
  if (num_parts == 0) {
    return {false, "", ""};
  }

  RPCAndArgs ret;
  
  std::string rpc;
  message >> rpc;
  ret.rpc = rpc;

  std::string args_str;
  message >> args_str;
  json args(args_str);
  ret.args = args;

  ret.valid = true;

  return ret;
}

void Server::RunServer ()
{
  utils::BindSocket(server_socket_, host_, port_);
  running_.store(true);

  while (true) {
    zmqpp::message received_message = ReceiveMessage(server_socket_);
    Server::RPCAndArgs rpc_and_args = MessageIntoParts(received_message);
    
    if (!rpc_and_args.valid) {
      continue;
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
