#ifndef RPCSERVER_HPP
#define RPCSERVER_HPP

#include "../json.hpp"

#include "RPC.hpp"
#include "RPCServerWorkerPool.hpp"

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

namespace rpc {

class RPCServer
{
private:
  using json = nlohmann::json;
  using RPCFunc = std::function<json(const json)>;

  std::string host_ = "*";
  unsigned port_;

  std::string backend_ipc_name_ = "backend";
  
  zmqpp::context ctx_;
  zmqpp::socket* frontend_;
  zmqpp::socket* backend_;
  void InitSockets ();

  // the number of workers can be tweaked later.
  const unsigned num_workers_ = 4;
  rpc::RPCServerWorkerPool* worker_pool_;
  void InitWorkerPool ();

  void RunServer ();
  
  std::atomic<bool> running_;
  std::thread* server_thread_;

public:
  RPCServer (const std::string&, unsigned);
  RPCServer (unsigned);
  ~RPCServer ();
  
  const std::string& GetHost () const { return host_; }
  unsigned GetPort () const { return port_; }
  
  void AddRPC (const std::string&, const RPCFunc&);

  void Run ();
};

} // namespace rpc

#endif // RPCSERVER_HPP
