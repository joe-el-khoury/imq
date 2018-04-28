#include "RPCServerWorkerPool.hpp"
#include "../../utils/Common.hpp"

rpc::RPCServerWorkerPool::RPCServerWorkerPool (unsigned num_workers, const std::string& ipc_name) : ipc_name_(ipc_name)
{
  worker_sockets_.resize(num_workers);
  server_workers_.resize(num_workers);
}

void rpc::RPCServerWorkerPool::Start ()
{
  unsigned num_workers = server_workers_.size();
  for (unsigned i = 0; i < num_workers; ++i) {
    worker_sockets_[i] = CreateWorkerSocket();
    server_workers_[i] = CreateServerWorker(worker_sockets_[i]);
  }
}

void rpc::RPCServerWorkerPool::AddRPC (const std::string& rpc_name, const RPCFunc& rpc_func)
{
  std::lock_guard<std::mutex> lk(rpcs_mutex_);
  rpcs_.insert({rpc_name, rpc_func});
}

rpc::RPCServerWorkerPool::RPCFunc& rpc::RPCServerWorkerPool::GetRPC (const std::string& rpc)
{
  std::lock_guard<std::mutex> lk(rpcs_mutex_);
  auto got = rpcs_.find(rpc);
  if (got == rpcs_.end()) {
    throw std::invalid_argument("RPC " + rpc + " not found.");
  }

  return got->second;
}

rpc::RPCServerWorkerPool::RPCResult rpc::RPCServerWorkerPool::PerformRPC (const RPCAndArgs& rpc_and_args)
{
  return rpc_and_args.rpc(rpc_and_args.args);
}

rpc::RPCServerWorkerPool::RPCAndArgs rpc::RPCServerWorkerPool::MessageToParts (rpc::RPCMessage& struct_message)
{
  zmqpp::message& message = struct_message.message;
  
  int num_parts = message.parts();
  if (num_parts == 0 || num_parts > 3) {
    // The message consists of [client-address] [empty] [rpc-name] [rpc-args]
    return {false};
  }

  RPCAndArgs ret;
  ret.valid = true;

  std::string client_addr;
  message >> client_addr;
  
  ret.client_addr = client_addr;
  
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

zmqpp::message rpc::RPCServerWorkerPool::ConstructMessage (const std::string& client_addr, const RPCResult& result)
{
  zmqpp::message msg;
  
  msg << client_addr;
  msg << result.dump();

  return msg;
}

zmqpp::message rpc::RPCServerWorkerPool::ConstructErrorMessage (const std::string& client_addr)
{
  zmqpp::message msg;

  msg << client_addr;
  msg << json({{"error", 1}}).dump();

  return msg;
}

void rpc::RPCServerWorkerPool::Work (zmqpp::socket* worker_socket)
{
  while (true) {
    RPCAndArgs rpc_and_args;

    rpc::RPCMessage received_message = rpc::ReceiveMessage(worker_socket);
    if (!received_message.received) {
      continue;
    }

    rpc_and_args = MessageToParts(received_message);
    std::string client_addr = rpc_and_args.client_addr;
    
    if (rpc_and_args.valid) {
      RPCResult result = PerformRPC(rpc_and_args);
      zmqpp::message to_send = ConstructMessage(client_addr, result);
      worker_socket->send(to_send);
    } else {
      zmqpp::message error_message = ConstructErrorMessage(client_addr);
      worker_socket->send(error_message);
    }
  }
}

std::thread* rpc::RPCServerWorkerPool::CreateServerWorker (zmqpp::socket* worker_socket)
{
  return new std::thread(std::bind(&rpc::RPCServerWorkerPool::Work, this, worker_socket));
}

zmqpp::socket* rpc::RPCServerWorkerPool::CreateWorkerSocket ()
{
    zmqpp::socket* worker_socket = utils::CreateSocket(ctx_, zmqpp::socket_type::dealer);
    utils::ConnectIPCSocket(worker_socket, ipc_name_);

    return worker_socket;
}
