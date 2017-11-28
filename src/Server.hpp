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

  using Request = std::function<json(const json)>;

private:
  unsigned port_;
  
  zmqpp::context ctx_;
  zmqpp::socket* server_socket_;

  std::unordered_map<std::string, Request> requests_;

public:
  Server (unsigned);
  ~Server ();

  void RunServer ();

  void AddRequest (const std::string&, const Request&);
};

#endif // SERVER_HPP
