#ifndef HOSTANDPORT_HPP
#define HOSTANDPORT_HPP

#include <string>

namespace utils {

struct HostAndPort
{
  std::string host;
  unsigned port;

  HostAndPort ()
  {}
  
  HostAndPort (const std::string& host_, unsigned port_)
  {
    host = host_;
    port = port_;
  }

  bool operator== (const HostAndPort& other) const
  {
    return host == other.host && port == other.port;
  }
};

} // namespace utils

namespace std
{
template<>
struct hash<utils::HostAndPort>
{
  std::size_t operator() (const utils::HostAndPort& host_and_port) const
  {
    const std::string& host = host_and_port.host;
    unsigned port = host_and_port.port;
    
    return std::hash<std::string>()(host + std::to_string(port));
  }
};
} // namespace std

#endif // HOSTANDPORT_HPP
