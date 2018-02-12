#include "RPCServerWorkerPool.hpp"

rpc::RPCServerWorkerPool::RPCServerWorkerPool (unsigned num_workers)
{
  for (unsigned i = 0; i < num_workers; ++i) {
    server_workers_.push_back(CreateServerWorker());
  }
}

void rpc::RPCServerWorkerPool::AddRPC (const std::string& rpc_name, const RPCFunc& rpc_func)
{
  rpcs_.insert({rpc_name, rpc_func});
}

rpc::RPCServerWorkerPool::RPCFunc& rpc::RPCServerWorkerPool::GetRPC (const std::string& rpc)
{
  auto got = rpcs_.find(rpc);
  if (got == rpcs_.end()) {
    throw std::invalid_argument("RPC " + rpc + " not found.");
  }

  return got->second;
}

rpc::RPCServerWorkerPool::RPCAndArgs rpc::RPCServerWorkerPool::MessageToParts (rpc::RPCMessage& struct_message)
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

std::thread* rpc::RPCServerWorkerPool::CreateServerWorker ()
{
  return new std::thread(
      [this]() {
        while (true) {
          {
            // do stuff
          }
        }
      }
  );
}