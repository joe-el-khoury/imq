#ifndef SERVER_HPP
#define SERVER_HPP

#include "json.hpp"

#include <zmqpp/zmqpp.hpp>

#include <vector>
#include <string>
#include <unordered_map>

#include <thread>
#include <atomic>

class Server
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

  std::vector<std::string> MessageIntoParts (zmqpp::message&);
  void RunServer ();
  
  std::atomic<bool> running_;
  std::thread* server_thread_;

public:
  void AddRPC (const std::string&, const RPCFunc&);
  Response PerformRPC (const std::string&, const json&);

  void Run ();
  
  Server (const std::string&, unsigned);
  Server (unsigned);
  ~Server ();

  std::string GetHost () { return host_; }
  unsigned GetPort () { return port_; }
};

#endif // SERVER_HPP
