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
private:
  unsigned heartbeat_port_;
  
  zmqpp::context ctx_;
  zmqpp::socket* heartbeat_socket_;

  std::atomic<bool> running_;

  // If more than 3 seconds we assume the master is dead.
  const unsigned HEARTBEAT_LIVENESS_S = 3;

  c::time_point<c::steady_clock> Now ();

public:
  Heartbeater (unsigned);

  void Start ();
};

#endif // HEARTBEATER_HPP
