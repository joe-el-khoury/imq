#include "RPCServer.hpp"
#include "RPC.hpp"
#include "../../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>

#include <stdexcept>

void RPCServer::InitSockets ()
{
  frontend_ = utils::CreateSocket(ctx_, zmqpp::socket_type::router);
  backend_ = utils::CreateSocket(ctx_, zmqpp::socket_type::dealer);
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
  if (frontend_) {
    delete frontend_;
  }
  if (backend_) {
    delete backend_;
  }
}

void RPCServer::AddRPC (const std::string& rpc_name, const RPCFunc& rpc_func)
{
  worker_pool_->AddRPC(rpc_name, rpc_func);
}

void RPCServer::RunServer ()
{
  utils::BindSocket(frontend_, host_, port_);
  utils::BindIPCSocket(backend_, backend_ipc_name);
  
  running_.store(true);
  while (running_.load()) {
    rpc::RPCMessage received_message = rpc::ReceiveMessage(frontend_);
    if (!received_message.received) {
      continue;
    }

    worker_pool_->PushMessage(received_message);
  }
}

void RPCServer::Run ()
{
  server_thread_ = new std::thread(&RPCServer::RunServer, this);
}
