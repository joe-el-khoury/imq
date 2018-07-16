#include "RPCServer.hpp"
#include "RPC.hpp"
#include "../../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>
#include <zmqpp/proxy.hpp>

#include <stdexcept>

void rpc::RPCServer::InitSockets ()
{
  frontend_ = utils::CreateSocket(ctx_, zmqpp::socket_type::router);
  backend_ = utils::CreateSocket(ctx_, zmqpp::socket_type::dealer);
}

void rpc::RPCServer::InitWorkerPool ()
{
  worker_pool_ = new rpc::RPCServerWorkerPool(num_workers_, backend_ipc_name_);
}

rpc::RPCServer::RPCServer (const std::string& host, unsigned port) : host_(host), port_(port), ctx_()
{
  SetHostAndPort(host, port);
  
  InitSockets();
  InitWorkerPool();
}

rpc::RPCServer::~RPCServer ()
{
  if (frontend_) {
    delete frontend_;
  }
  if (backend_) {
    delete backend_;
  }
}

void rpc::RPCServer::AddRPC (const std::string& rpc_name, const RPCFunc& rpc_func)
{
  worker_pool_->AddRPC(rpc_name, rpc_func);
}

void rpc::RPCServer::RunServer ()
{
  utils::BindSocket(frontend_, host_, port_);
  utils::BindIPCSocket(backend_, backend_ipc_name_);

  // Always start the worker pool after binding the ipc socket.
  worker_pool_->Start();
  
  zmqpp::proxy(*frontend_, *backend_); 
}

void rpc::RPCServer::Run ()
{
  if (running_.load()) {
    return;
  }
  
  server_thread_ = new std::thread(&rpc::RPCServer::RunServer, this);
}

void rpc::RPCServer::Stop ()
{
  running_.store(false);
  if (server_thread_) {
    server_thread_->join();
  }
}
