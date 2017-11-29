#ifndef HEARTBEATSENDER_HPP
#define HEARTBEATSENDER_HPP

#include <atomic>
#include <thread>

#include "Heartbeater.hpp"

namespace c = std::chrono;
using duration_t  = c::duration<double>;
using timepoint_t = c::time_point<c::steady_clock, duration_t>;

class HeartbeatSender : public Heartbeater
{
private:
  std::atomic<bool> running_;

  timepoint_t send_heartbeat_at_;
  timepoint_t Now();

  void SetSendHeartbeatAt ();
  
  bool ShouldSendHeartbeat ();

public:
  void Start ();
};

#endif // HEARTBEATSENDER_HPP
