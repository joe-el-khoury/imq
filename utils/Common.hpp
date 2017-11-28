#ifndef COMMON_H
#define COMMON_H

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

}

#endif // COMMON_H
