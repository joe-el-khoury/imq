#include "HeartbeatSender.hpp"

timepoint_t HeartbeatSender::Now ()
{
  return c::steady_clock::now();
}

void HeartbeatSender::SetSendHeartbeatAt ()
{
  send_heartbeat_at_ = Now() + Heartbeater::HEARTBEAT_LIVENESS_S;
}

bool HeartbeatSender::ShouldSendHeartbeat ()
{
  return Now() >= send_heartbeat_at_;
}

void HeartbeatSender::Start ()
{
  SetSendHeartbeatAt();

  running_.store(true);
  while (running_.load()) {
    if (ShouldSendHeartbeat()) {
      // send heartbeat
      std::cout << "Sending!" << std::endl;
      SetSendHeartbeatAt();
    }
  }
}
