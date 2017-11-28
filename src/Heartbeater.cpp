#include "Heartbeater.hpp"
#include "../utils/Common.hpp"

Heartbeater::Heartbeater (unsigned heartbeat_port) : heartbeat_port_(heartbeat_port)
{
}

c::time_point<c::steady_clock> Now ()
{
  return c::steady_clock::now();
}

void Heartbeater::Start ()
{
  running_.store(true);
  while (running_.load()) {
    // Send heartbeats.
  }
}
