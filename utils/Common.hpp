#ifndef COMMON_H
#define COMMON_H

#include <string>

#include <zmqpp/zmqpp.hpp>

namespace utils
{

inline std::string ConstructTCPBindAddress (unsigned port)
{
  return "tcp://*:" + std::to_string(port);
}

inline zmqpp::socket* GetInitSocket (zmqpp::context& ctx, zmqpp::socket_type socket_type)
{
  return new zmqpp::socket(ctx, socket_type);
}

inline void BindSocket (zmqpp::socket* socket, unsigned port_to_bind_to)
{
  if (socket) {
    socket->bind(ConstructTCPBindAddress(port_to_bind_to));
  }
}

inline void ConnectSocketTo (zmqpp::socket* socket, const std::string& address)
{
  if (socket) {
    socket->connect(address);
  }
}

}

#endif // COMMON_H
