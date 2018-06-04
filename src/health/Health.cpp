#include "Health.hpp"

#include "rpc/RPCCall.hpp"

#include "rpc/RPCServer.hpp"
#include "rpc/store/RPCServerStore.hpp"

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

void Health::HealthyNodeCallback (const json& response)
{
  std::string callee_host = response["callee_info"]["host"].get<std::string>();
  unsigned callee_port = response["callee_info"]["port"].get<unsigned>();
}

void Health::TimedoutNodeCallback (const json& response)
{
  std::string callee_host = response["callee_info"]["host"].get<std::string>();
  unsigned callee_port = response["callee_info"]["port"].get<unsigned>();
}

void Health::PerformHealthChecks ()
{
  std::string my_host = MetaStore::GetHostAndPort().host;
  unsigned my_port = MetaStore::GetHostAndPort().port;
  
  running_.store(true);
  while (running_.load()) {
    // The call we'll use for each node.
    rpc::RPCCall rpc_call("CheckHealth", /* async */ true);
    rpc_call.SetMessageCallback(std::bind(&Health::HealthyNodeCallback, this, std::placeholders::_1));
    rpc_call.SetTimeoutCallback(3000, std::bind(&Health::TimedoutNodeCallback, this, std::placeholders::_1));
    
    for (const utils::HostAndPort& node : cluster_->GetNodesInCluster()) {
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

  rpc::RPCServerStore rpc_server_store;
  (rpc_server_store.GetRPCServer(host, port))->AddRPC(
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
