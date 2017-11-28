#ifndef NODE_HPP
#define NODE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

class IMQNode
{
private:
  // The port we're currently listening on.
  unsigned server_port_;

  zmqpp::context ctx_;
  // The client socket sends reqs to other nodes, while the server socket
  // responds to reqs from other nodes.
  zmqpp::socket* client_socket_;
  zmqpp::socket* server_socket_;

  std::atomic<bool> running_;

  void Run ();

public:
  IMQNode (unsigned);
};

#endif // NODE_HPP
