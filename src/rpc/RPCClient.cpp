#include "RPCClient.hpp"
#include "../../utils/Common.hpp"

#include <zmqpp/zmqpp.hpp>

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

RPCClient::Response RPCClient::Call (const std::string& rpc, const json& args)
{
  zmqpp::message message_to_send;

  message_to_send.add(rpc);
  message_to_send.add(args.dump());

  client_socket_->send(message_to_send);

  // synchronously receive a message for now.
  zmqpp::message received_message;
  client_socket_->receive(received_message);
  
  std::string string_response;
  received_message >> string_response;
  RPCClient::Response json_response = json::parse(string_response);

  return json_response;
}
