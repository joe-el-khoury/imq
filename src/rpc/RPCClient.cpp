#include "RPCClient.hpp"
#include "../../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>

void RPCClient::InitSockets (const std::string& host, unsigned port)
{
  client_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::req);
  utils::ConnectSocket(client_socket_, host, port);
}

RPCClient::RPCClient (const std::string& host, unsigned port)
{
  InitSockets(host, port);
}

RPCClient::RPCClient (const RPCServer& rpc_server)
{
  InitSockets(rpc_server.GetHost(), rpc_server.GetPort());
}

rpc::RPCResponse RPCClient::Call (const std::string& rpc, const json& args)
{
  zmqpp::message message_to_send;

  message_to_send.add(rpc);
  message_to_send.add(args.dump());

  client_socket_->send(message_to_send);

  return rpc::RPCResponse(client_socket_);
}
