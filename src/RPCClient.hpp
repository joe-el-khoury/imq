#ifndef RPCCLIENT_HPP
#define RPCCLIENT_HPP

#include "json.hpp"

#include "RPCServer.hpp"

#include <zmqpp/zmqpp.hpp>

class RPCClient
{
private:
  using json = nlohmann::json;

  using RPCFunc = std::function<json(const json)>;
  using Response = json;

  std::string host_;
  unsigned port_;
  
  zmqpp::context ctx_;
  zmqpp::socket* client_socket_;

public:
  RPCClient (const std::string&, unsigned);
  RPCClient (const RPCServer&);
  ~RPCClient ();

  Response Call (const std::string&, const json&);
};

#endif // RPCCLIENT_HPP
