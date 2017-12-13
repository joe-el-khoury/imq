#ifndef NODE_HPP
#define NODE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>
#include <string>

#include "Server.hpp"

class IMQNode
{
private:
  Server* server_;

  void Run ();

public:
  IMQNode (const std::string&, unsigned);
  IMQNode (unsigned);
};

#endif // NODE_HPP
