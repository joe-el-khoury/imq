#include "Health.hpp"

#include "rpc/RPCServer.hpp"

#include "meta/MetaStore.hpp"

#include "json.hpp"

#include <memory>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

unsigned Health::GetCurrentTime ()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();
}

void Health::PerformHealthChecks ()
{
  std::string my_host = MetaStore::GetHostAndPort().host;
  unsigned my_port = MetaStore::GetHostAndPort().port;
  
  running_.store(true);
  while (running_.load()) {
    for (const HostAndPort& node : cluster_->GetNodesInCluster()) {
      if (node.host == my_host && node.port == my_port) {
        // Checking our own health doesn't make sense.
        continue;
      }

      // do stuff
    }

    std::this_thread::sleep_for(5s);
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

void Health::Stop ()
{
  running_.store(false);
  if (health_check_thread_) {
    health_check_thread_->join();
  }
}
