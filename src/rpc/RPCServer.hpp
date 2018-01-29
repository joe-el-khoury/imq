#ifndef RPCSERVER_HPP
#define RPCSERVER_HPP

#include "../json.hpp"

#include <zmqpp/zmqpp.hpp>

#include <vector>
#include <string>
#include <unordered_map>

#include <thread>
#include <atomic>

class RPCServer
{
private:
  using json = nlohmann::json;

  using RPCFunc = std::function<json(const json)>;
  using Response = json;

  std::string host_ = "*";
  unsigned port_;
  
  zmqpp::context ctx_;
  zmqpp::socket* server_socket_;

  std::unordered_map<std::string, RPCFunc> rpcs_;

  RPCFunc& GetRPC (const std::string&);

  struct RPCAndArgs
  {
    bool valid;
    
    RPCFunc rpc;
    json args;
  };

  struct Message
  {
    bool received;
    zmqpp::message message;
  };
  
  Response PerformRPC (const RPCAndArgs&);
  
  RPCAndArgs MessageToParts (Message&);

  Message ReceiveMessage (zmqpp::socket*);
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

#endif // RPCSERVER_HPP
