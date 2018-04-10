#ifndef RPCCLIENT_HPP
#define RPCCLIENT_HPP

#include "../json.hpp"

#include "RPCObject.hpp"
#include "RPCServer.hpp"
#include "RPCResponse.hpp"

#include "RPCCall.hpp"

#include <zmqpp/zmqpp.hpp>

namespace rpc {

class RPCClient : public RPCObject
{
private:
  using json = nlohmann::json;

  zmqpp::context ctx_;
  zmqpp::socket* client_socket_;
  void InitSockets (const std::string&, unsigned);

public:
  RPCClient (const std::string&, unsigned);
  RPCClient (const RPCClient&) = delete;
  ~RPCClient ();

  rpc::RPCResponse Call (const RPCCall&);
};

} // namespace rpc

#endif // RPCCLIENT_HPP
