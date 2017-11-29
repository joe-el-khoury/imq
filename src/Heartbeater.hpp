#ifndef HEARTBEATER_HPP
#define HEARTBEATER_HPP

#include <string>

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

#include <chrono>

namespace c = std::chrono;

class Heartbeater
{
protected:
  unsigned heartbeat_port_;
  
  zmqpp::context ctx_;
  zmqpp::socket* heartbeat_socket_;

  std::atomic<bool> running_;

  // How long to wait between heartbeats.
  c::duration<double> HEARTBEAT_LIVENESS_S = c::seconds(3);

public:
  virtual void Start () {}
};

#endif // HEARTBEATER_HPP
