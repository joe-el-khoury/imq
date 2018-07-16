#ifndef HOSTANDPORT_HPP
#define HOSTANDPORT_HPP

#include <string>
#include <iostream>
#include <sstream>

namespace utils {

struct HostAndPort
{
  std::string host = "";
  unsigned port = 0;

  HostAndPort ()
  {}
  
  HostAndPort (const std::string& host_, unsigned port_)
  {
    host = host_;
    port = port_;
  }

  std::string ToString ()
  {
    return host + ":" + std::to_string(port);
  }

  static HostAndPort FromString (const std::string& s)
  {
    utils::HostAndPort ret;

    unsigned colon_idx = s.find(':');
    ret.host = s.substr(0, colon_idx);
    ret.port = std::stoi(s.substr(colon_idx+1), nullptr);

    return ret;
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
