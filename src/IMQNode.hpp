#ifndef NODE_HPP
#define NODE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>
#include <string>

#include "rpc/RPCServer.hpp"

#include "Cluster.hpp"

class IMQNode
{
private:
  rpc::RPCServer* rpc_server_;

  Cluster* cluster;

public:
  IMQNode (const std::string&, unsigned);
  ~IMQNode ();

  void Run ();
};

#endif // NODE_HPP
