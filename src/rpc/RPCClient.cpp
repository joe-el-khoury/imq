#include "RPCClient.hpp"
#include "../../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>

void rpc::RPCClient::InitSockets (const std::string& host, unsigned port)
{
  client_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::dealer);
  utils::ConnectSocket(client_socket_, host, port);
}

rpc::RPCClient::RPCClient (const std::string& host, unsigned port)
{
  InitSockets(host, port);
}

rpc::RPCClient::RPCClient (const RPCServer& rpc_server)
{
  InitSockets(rpc_server.GetHost(), rpc_server.GetPort());
}

rpc::RPCResponse rpc::RPCClient::Call (const RPCCall& rpc_call)
{
  zmqpp::message message_to_send;

  const std::string& rpc = rpc_call.GetRPCName();
  const json& args = rpc_call.GetArgs();

  message_to_send.add(rpc);
  message_to_send.add(args.dump());

  client_socket_->send(message_to_send);

  return rpc::RPCResponse(client_socket_, rpc_call);
}
