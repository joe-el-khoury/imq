#include "RPCServerWorkerPool.hpp"

rpc::RPCServerWorkerPool::RPCServerWorkerPool (unsigned num_workers)
{
  for (unsigned i = 0; i < num_workers; ++i) {
    server_workers_.push_back(CreateServerWorker());
  }
}

std::thread* rpc::RPCServerWorkerPool::CreateServerWorker ()
{
}
