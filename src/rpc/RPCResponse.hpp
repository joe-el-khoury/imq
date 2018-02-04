#ifndef RPCRESPONSE_HPP
#define RPCRESPONSE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

class RPCResponse
{
private:
  std::atomic<bool> received_;
  zmqpp::message receieved_message_;

  zmqpp::socket* socket_;
  std::thread* message_thread_;

  void CheckMessageReceipt ();

public:
  RPCResponse (zmqpp::socket*);
  ~RPCResponse ();
};

#endif // RPCRESPONSE_HPP
