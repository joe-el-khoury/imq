#include "Heartbeater.hpp"
#include "../utils/Common.hpp"

Heartbeater::Heartbeater (unsigned heartbeat_port)
{
  heartbeat_socket_ = utils::GetInitSocket(ctx_, zmqpp::socket_type::rep);
}

void Heartbeater::SendHeartbeatsTo (const std::string& address)
{
  utils::ConnectSocketTo(heartbeat_socket_, address);
}

void Heartbeater::Start ()
{
  // start sending heartbeats
}
