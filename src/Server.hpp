#ifndef SERVER_HPP
#define SERVER_HPP

#include "json.hpp"

#include <zmqpp/zmqpp.hpp>

#include <vector>

class Server
{
private:
  unsigned port_;
  
  zmqpp::context ctx_;
  zmqpp::socket* server_socket_;

public:
  Server (unsigned);
  ~Server ();

  void RunServer ();
};

#endif // SERVER_HPP
