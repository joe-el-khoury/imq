#ifndef RPCSERVERWORKERPOOL_HPP
#define RPCSERVERWORKERPOOL_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

#include "RPC.hpp"

#include "../json.hpp"

namespace rpc {

class RPCServerWorkerPool
{
private:
  using json = nlohmann::json;
  using RPCFunc = std::function<json(const json)>;
  using JSONResponse = json;

  std::unordered_map<std::string, RPCFunc> rpcs_;
  
  std::queue<rpc::RPCMessage> task_queue_;
  std::mutex task_queue_mutex_;
  std::condition_variable cv_;

  std::vector<std::thread*> server_workers_;

  struct RPCAndArgs
  {
    bool valid;
    
    RPCFunc rpc;
    json args;
  };
  RPCAndArgs MessageToParts (rpc::RPCMessage&);

  RPCFunc& GetRPC (const std::string&);
  JSONResponse PerformRPC (const RPCAndArgs&);

  std::thread* CreateServerWorker ();

public:
  RPCServerWorkerPool (unsigned);

  void PushMessage (rpc::RPCMessage&);
  void AddRPC (const std::string&, const RPCFunc&);
};

}

#endif // RPCSERVERWORKERPOOL_HPP
