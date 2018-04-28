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
  using RPCResult = json;

  std::mutex rpcs_mutex_;
  std::unordered_map<std::string, RPCFunc> rpcs_;

  std::string ipc_name_;

  zmqpp::context ctx_;
  std::vector<std::thread*> server_workers_;
  std::vector<zmqpp::socket*> worker_sockets_;

  struct RPCAndArgs
  {
    bool valid;

    std::string client_addr;
    
    RPCFunc rpc;
    json args;
  };
  RPCAndArgs MessageToParts (rpc::RPCMessage&);

  RPCFunc& GetRPC (const std::string&);
  RPCResult PerformRPC (const RPCAndArgs&);

  zmqpp::message ConstructMessage (const std::string&, const RPCResult&);
  zmqpp::message ConstructErrorMessage (const std::string&);
  
  void Work (zmqpp::socket*);
  std::thread* CreateServerWorker (zmqpp::socket*);
  zmqpp::socket* CreateWorkerSocket ();

public:
  RPCServerWorkerPool (unsigned, const std::string&);

  void AddRPC (const std::string&, const RPCFunc&);
  
  void Start ();
};

}

#endif // RPCSERVERWORKERPOOL_HPP
