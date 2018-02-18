#ifndef COMMON_H
#define COMMON_H

#include <string>

#include <zmqpp/zmqpp.hpp>

namespace utils
{

inline std::string ConstructTCPBindAddress (const std::string& host, unsigned port)
{
  return "tcp://" + host + ":" + std::to_string(port);
}

inline std::string ConstructTCPBindAddress (unsigned port)
{
  return "tcp://*:" + std::to_string(port);
}

inline std::string ConstructIPCBindAddress (const std::string& ipc_name)
{
  return "ipc://" + ipc_name + ".ipc";
}

inline zmqpp::socket* CreateSocket (zmqpp::context& ctx, zmqpp::socket_type socket_type)
{
  return new zmqpp::socket(ctx, socket_type);
}

inline void BindSocket (zmqpp::socket* socket, const std::string& host, unsigned port)
{
  if (socket) {
    socket->bind(ConstructTCPBindAddress(host, port));
  }
}

inline void BindSocket (zmqpp::socket* socket, unsigned port)
{
  if (socket) {
    socket->bind(ConstructTCPBindAddress(port));
  }
}

inline void BindIPCSocket (zmqpp::socket* socket, const std::string& ipc_name)
{
  if (socket) {
    socket->bind(ConstructIPCBindAddress(ipc_name));
  }
}

inline void ConnectSocket (zmqpp::socket* socket, const std::string& host, unsigned port)
{
  if (socket) {
    socket->connect(ConstructTCPBindAddress(host, port));
  }
}

}

#endif // COMMON_H
