#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "Cluster.hpp"

#include "rpc/store/RPCServerStore.hpp"

#include "json.hpp"

#include <thread>
#include <atomic>
#include <unordered_set>
#include <mutex>
#include <thread>

class Health
{
private:
  using json = nlohmann::json;

  // To get nodes in cluster.
  Cluster* cluster_ = &Cluster::GetInstance();

  // How many times to keep checking on a node before marking it fully failed.
  const int REMOVE_AFTER_NUM_RETRIES = 3;
  
  // Map from the address of the potentially failed node and the number of retries.
  std::unordered_map<utils::HostAndPort, int> potentially_failed_nodes_and_num_retires_;
  std::mutex map_mutex;

  void MarkAsPotentiallyFailing (const std::string& host, unsigned port)
  {
    std::lock_guard<std::mutex> lk(map_mutex);
    
    utils::HostAndPort hap(host, port);
    potentially_failed_nodes_and_num_retires_.insert({hap, 0});
  }
  void UnmarkAsPotentiallyFailing (const std::string& host, unsigned port)
  {
    std::lock_guard<std::mutex> lk(map_mutex);
    
    utils::HostAndPort hap(host, port);
    auto iter = potentially_failed_nodes_and_num_retires_.find(hap);
    if (iter != potentially_failed_nodes_and_num_retires_.end()) {
      potentially_failed_nodes_and_num_retires_.erase(iter);
    }
  }
  bool IsPotentiallyFailing (const std::string& host, unsigned port)
  {
    std::lock_guard<std::mutex> lk(map_mutex);
    
    utils::HostAndPort hap(host, port);
    return potentially_failed_nodes_and_num_retires_.find(hap) 
      != potentially_failed_nodes_and_num_retires_.end();
  }
  unsigned GetNumberOfRetriesForNode (const std::string& host, unsigned port)
  {
    std::lock_guard<std::mutex> lk(map_mutex);
    
    utils::HostAndPort hap(host, port);
    if (IsPotentiallyFailing(host, port)) {
      return potentially_failed_nodes_and_num_retires_[hap];
    } else {
      return 0;
    }
  }
  void IncrementNumberOfRetriesForNode (const std::string& host, unsigned port)
  {
    std::lock_guard<std::mutex> lk(map_mutex);
    
    utils::HostAndPort hap(host, port);
    potentially_failed_nodes_and_num_retires_[hap]++;
  }
  
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
