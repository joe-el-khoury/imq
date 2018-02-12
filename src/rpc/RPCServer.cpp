#include "RPCServer.hpp"
#include "RPC.hpp"
#include "../../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

void RPCServer::InitSockets ()
{
  server_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::rep);
}

void RPCServer::InitWorkerPool ()
{
  worker_pool_ = new rpc::RPCServerWorkerPool(num_workers_);
}

RPCServer::RPCServer (const std::string& host, unsigned port) : host_(host), port_(port), ctx_()
{
  InitSockets();
  InitWorkerPool();
}

RPCServer::RPCServer (unsigned port) : port_(port), ctx_()
{
  InitSockets();
  InitWorkerPool();
}

RPCServer::~RPCServer ()
{
  server_thread_->join();
  if (server_socket_) {
    delete server_socket_;
  }
}

void RPCServer::AddRPC (const std::string& rpc_name, const RPCFunc& rpc_func)
{
  worker_pool_->AddRPC(rpc_name, rpc_func);
}

void RPCServer::RunServer ()
{
  utils::BindSocket(server_socket_, host_, port_);
  
  running_.store(true);
  while (running_.load()) {
    rpc::RPCMessage received_message = rpc::ReceiveMessage(server_socket_);
    if (!received_message.received) {
      continue;
    }

    std::cout << "pushing" << std::endl;
    worker_pool_->PushMessage(received_message);
    std::cout << "pushed" << std::endl;
  }
}

void RPCServer::Run ()
{
  server_thread_ = new std::thread(&RPCServer::RunServer, this);
}
