#ifndef COMMON_H
#define COMMON_H

#include <zmqpp/zmqpp.hpp>

namespace utils
{

std::string ConstructTCPBindAddress (unsigned port)
{
  return "tcp://*:" + std::to_string(port);
}

void InitAndBindSocket (zmqpp::context& ctx, zmqpp::socket* socket, zmqpp::socket_type socket_type, unsigned port_to_bind_to)
{
  socket = new zmqpp::socket(ctx, socket_type);
  std::string address = ConstructTCPBindAddress(port_to_bind_to);
  socket->bind(address);
}

void InitAndBindSocket (zmqpp::context& ctx, zmqpp::socket* socket, zmqpp::socket_type socket_type, const std::string& address)
{
  socket = new zmqpp::socket(ctx, socket_type);
  socket->bind(address);
}

}

#endif // COMMON_H
