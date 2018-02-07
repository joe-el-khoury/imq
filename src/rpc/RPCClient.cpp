#include "RPCClient.hpp"
#include "../../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>

#include <memory>

#include <stdexcept>

RPCClient::RPCClient (const std::string& host, unsigned port) : host_(host), port_(port), ctx_()
{
  client_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::req);
  utils::ConnectSocket(client_socket_, host_, port_);
}

RPCClient::RPCClient (const RPCServer& rpc_server)
{
  client_socket_ = utils::CreateSocket(ctx_, zmqpp::socket_type::req);
  utils::ConnectSocket(client_socket_, rpc_server.GetHost(), rpc_server.GetPort());
}

rpc::utils::RPCResponse RPCClient::Call (const std::string& rpc, const json& args)
{
  zmqpp::message message_to_send;

  message_to_send.add(rpc);
  message_to_send.add(args.dump());

  client_socket_->send(message_to_send);

  return rpc::utils::RPCResponse(client_socket_);
}
