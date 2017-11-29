#include "Heartbeater.hpp"
#include "../utils/Common.hpp"

#include <ratio>

Heartbeater::Heartbeater (unsigned heartbeat_port) : heartbeat_port_(heartbeat_port)
{
}

timepoint_t Heartbeater::Now ()
{
  return c::steady_clock::now();
}

void Heartbeater::SetSendHeartbeatAt ()
{
  send_heartbeat_at_ = Now() + HEARTBEAT_LIVENESS_S;
}

bool Heartbeater::ShouldSendHeartbeat ()
{
  return Now() >= send_heartbeat_at_;
}

void Heartbeater::StartSending ()
{
  SetSendHeartbeatAt();
  
  running_.store(true);
  while (running_.load()) {
    if (ShouldSendHeartbeat()) {
      // send heartbeat
      SetSendHeartbeatAt();
    }
  }
}
