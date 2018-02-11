#ifndef RPCSERVERWORKERPOOL_HPP
#define RPCSERVERWORKERPOOL_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

#include "RPC.hpp"

namespace rpc {

class RPCServerWorkerPool
{
private:
  std::queue<rpc::RPCMessage> task_queue_;
  std::mutex task_queue_mutex_;
  std::condition_variable cv_;

  std::vector<std::thread*> server_workers_;

  std::thread* CreateServerWorker ();

public:
  RPCServerWorkerPool (unsigned);
};

}

#endif // RPCSERVERWORKERPOOL_HPP
