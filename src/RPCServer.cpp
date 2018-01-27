#include "RPCServer.hpp"
#include "../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

RPCServer::RPCServer (const std::string& host, unsigned port) : host_(host), port_(port), ctx_()
{
  server_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::rep);
}

RPCServer::RPCServer (unsigned port) : port_(port), ctx_()
{
  server_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::rep);
}

RPCServer::~RPCServer ()
{
  server_thread_->join();
  if (server_socket_) {
    delete server_socket_;
  }
}

RPCServer::RPCFunc& RPCServer::GetRPC (const std::string& rpc)
{
  auto got = rpcs_.find(rpc);
  if (got == rpcs_.end()) {
    throw std::invalid_argument("RPC " + rpc + " not found.");
  }

  return got->second;
}

RPCServer::Message RPCServer::ReceiveMessage (zmqpp::socket* socket)
{
  zmqpp::message received_message;
  bool received = socket->receive(received_message, /*dont_block*/ true);

  Message message;
  message.received = received;
  message.message = std::move(received_message);
  
  return message;
}

RPCServer::RPCAndArgs RPCServer::MessageToParts (Message& struct_message)
{
  zmqpp::message& message = struct_message.message;
  
  int num_parts = message.parts();
  if (num_parts == 0 || num_parts >= 3) {
    return {false};
  }

  RPCAndArgs ret;
  ret.valid = true;
  
  std::string rpc;
  message >> rpc;
  try {
    ret.rpc = GetRPC(rpc);
  } catch (const std::invalid_argument& e) {
    // RPC doesn't exist
    ret.valid = false;
  }

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

void RPCServer::RunServer ()
{
  utils::BindSocket(server_socket_, host_, port_);
  
  running_.store(true);
  while (running_.load()) {
    RPCServer::Message received_message = ReceiveMessage(server_socket_);
    if (!received_message.received) {
      continue;
    }
    
    RPCServer::RPCAndArgs rpc_and_args = MessageToParts(received_message);
    if (!rpc_and_args.valid) {
      server_socket_->send("invalid");
    } else {
      RPCFunc& rpc = rpc_and_args.rpc;
      json& args = rpc_and_args.args;

      json result = rpc(args);
      std::string result_as_string = result.dump();
      server_socket_->send(result_as_string);
    }
  }
}

void RPCServer::Run ()
{
  server_thread_ = new std::thread(&RPCServer::RunServer, this);
}

void RPCServer::AddRPC (const std::string& rpc_name, const RPCFunc& rpc_func)
{
  rpcs_.insert({rpc_name, rpc_func});
}

RPCServer::Response RPCServer::PerformRPC (const std::string& rpc, const json& args)
{
  auto got = rpcs_.find(rpc);
  if (got == rpcs_.end()) {
    throw std::invalid_argument("rpc " + rpc + " not supported");
  }

  RPCFunc& func = got->second;
  return func(args);
}
