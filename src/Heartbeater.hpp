#ifndef HEARTBEATER_HPP
#define HEARTBEATER_HPP

#include <string>

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

#include <chrono>

namespace c = std::chrono;

using duration_t  = c::duration<double>;
using timepoint_t = c::time_point<c::steady_clock, duration_t>;

class Heartbeater
{
private:
  unsigned heartbeat_port_;
  
  zmqpp::context ctx_;
  zmqpp::socket* heartbeat_socket_;

  std::atomic<bool> running_;

  // How long to wait between heartbeats.
  c::duration<double> HEARTBEAT_LIVENESS_S = c::seconds(3);

  timepoint_t send_heartbeat_at_;
  timepoint_t Now ();

  void SetSendHeartbeatAt ();
  
  bool ShouldSendHeartbeat ();

public:
  Heartbeater (unsigned);

  void StartSending ();
};

#endif // HEARTBEATER_HPP
