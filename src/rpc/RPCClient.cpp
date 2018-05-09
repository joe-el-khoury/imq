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
  SetHostAndPort(host, port);
  
  InitSockets(host, port);
}

rpc::RPCClient::~RPCClient ()
{
  ctx_.terminate();
  if (client_socket_) {
    delete client_socket_;
    client_socket_ = nullptr;
  }
}

rpc::RPCResponse rpc::RPCClient::Call (RPCCall& rpc_call)
{
  zmqpp::message message_to_send;

  // Set the rpc call's metadata about the callee.
  rpc_call.SetCalleeHost(GetHost());
  rpc_call.SetCalleePort(GetPort());

  const std::string& rpc = rpc_call.GetRPCName();
  const json& args = rpc_call.GetArgs();

  message_to_send.add(rpc);
  message_to_send.add(args.dump());

  client_socket_->send(message_to_send);

  return rpc::RPCResponse(client_socket_, rpc_call);
}
