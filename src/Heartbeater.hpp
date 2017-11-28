#ifndef HEARTBEATER_HPP
#define HEARTBEATER_HPP

#include <string>

#include <zmqpp/zmqpp.hpp>

class Heartbeater
{
private:
  zmqpp::context ctx_;
  zmqpp::socket* heartbeat_socket_;

public:
  Heartbeater (unsigned);

  void SendHeartbeatsTo (const std::string&);
  void Start ();
};

#endif // HEARTBEATER_HPP
