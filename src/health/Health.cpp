#include "Health.hpp"

#include "rpc/RPCServer.hpp"

#include "meta/MetaStore.hpp"

#include "json.hpp"

#include <memory>
#include <chrono>
#include <thread>

unsigned Health::GetCurrentTime ()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();
}

void Health::PerformHealthChecks ()
{
  running_.store(true);
  while (running_.load()) {
    // do stuff
  }
}

void Health::StartHealthCheckThread ()
{
  health_check_thread_ = new std::thread(std::bind(&Health::PerformHealthChecks, this));
}

Health::json Health::CheckHealthRPC (const json& j)
{
  return {{"health", 1}};
}

void Health::Run ()
{
  // Add the health RPC to the server.
  
  std::string host = MetaStore::GetHost();
  unsigned port = MetaStore::GetPort();

  (rpc_server_store_.GetRPCServer(host, port))->AddRPC(
      "CheckHealth", std::bind(&Health::CheckHealthRPC, this, std::placeholders::_1));

  StartHealthCheckThread();
}
