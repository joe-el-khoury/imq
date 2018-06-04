#ifndef METASTORE_HPP
#define METASTORE_HPP

#include "utils/HostAndPort.hpp"

class MetaStore
{
private:
  static utils::HostAndPort my_host_and_port_;
public:
  static void SetHost (const std::string& host) { MetaStore::my_host_and_port_.host = host; }
  static void SetPort (unsigned port) { MetaStore::my_host_and_port_.port = port; }

  static const utils::HostAndPort& GetHostAndPort () { return MetaStore::my_host_and_port_; }
  static const std::string& GetHost () { return MetaStore::GetHostAndPort().host; }
  static unsigned GetPort () { return MetaStore::GetHostAndPort().port; }
};

#endif // METASTORE_HPP
