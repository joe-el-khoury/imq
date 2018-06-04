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

  // To get nodes in cluster.
  Cluster* cluster_ = &Cluster::GetInstance();
  
  unsigned GetCurrentTime ();

  void PerformHealthChecks ();
  void StartHealthCheckThread ();

  std::atomic<bool> running_;
  std::thread* health_check_thread_;

  json CheckHealthRPC (const json&);

  // RPC Callbacks.
  void HealthyNodeCallback (const json&);
  void TimedoutNodeCallback (const json&);

public:
  void Run ();
  void Stop ();
};

#endif // HEALTH_HPP
