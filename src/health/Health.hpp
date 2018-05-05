#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "Cluster.hpp"

#include "rpc/store/RPCServerStore.hpp"

#include "json.hpp"

#include <thread>
#include <atomic>

class Health
{
private:
  using json = nlohmann::json;

  Cluster* cluster_ = &Cluster::GetInstance();
  rpc::RPCServerStore rpc_server_store_;

  unsigned GetCurrentTime ();

  void PerformHealthChecks ();
  void StartHealthCheckThread ();

  std::atomic<bool> running_;
  std::thread* health_check_thread_;

  json CheckHealthRPC (const json&);

public:
  void Run ();
  void Stop ();
};

#endif // HEALTH_HPP
