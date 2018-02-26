#ifndef RPCCLIENT_HPP
#define RPCCLIENT_HPP

#include "../json.hpp"

#include "RPCServer.hpp"
#include "RPCResponse.hpp"

#include <memory>

#include <zmqpp/zmqpp.hpp>

class RPCClient
{
private:
  using json = nlohmann::json;
  
  zmqpp::context ctx_;
  zmqpp::socket* client_socket_;
  void InitSockets (const std::string&, unsigned);

public:
  RPCClient (const std::string&, unsigned);
  RPCClient (const RPCServer&);
  ~RPCClient ();

  rpc::RPCResponse Call (const std::string&, const json&);
};

#endif // RPCCLIENT_HPP
