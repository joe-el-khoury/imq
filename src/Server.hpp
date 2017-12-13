#ifndef SERVER_HPP
#define SERVER_HPP

#include "json.hpp"

#include <zmqpp/zmqpp.hpp>

#include <vector>
#include <string>
#include <unordered_map>

class Server
{
protected:
  using json = nlohmann::json;

  using RequestFunc = std::function<json(const json)>;
  using Response = json;

  void AddRequest (const std::string&, const RequestFunc&);

private:
  std::string host_ = "*";
  unsigned port_;
  
  zmqpp::context ctx_;
  zmqpp::socket* server_socket_;

  std::unordered_map<std::string, RequestFunc> requests_;

public:
  Server (const std::string&, unsigned);
  Server (unsigned);
  ~Server ();

  std::string GetHost () { return host_; }
  unsigned GetPort () { return port_; }

  void Run ();

  Response PerformRequest (const std::string&, const json&);
};

#endif // SERVER_HPP
