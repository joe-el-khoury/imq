#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "Cluster.hpp"

#include "rpc/store/RPCServerStore.hpp"

#include "json.hpp"

#include <thread>

class Health
{
private:
  using json = nlohmann::json;

  Cluster* cluster = &Cluster::GetInstance();
  rpc::RPCServerStore rpc_server_store_;

  unsigned GetCurrentTime ();

  void PerformHealthChecks ();
  void StartHealthCheckThread ();

  std::thread* health_check_thread_;

  json CheckHealthRPC (const json&);

public:
  void Run ();
};

#endif // HEALTH_HPP
