#ifndef RPCRESPONSE_HPP
#define RPCRESPONSE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

#include "../json.hpp"

class RPCResponse
{
private:
  using json = nlohmann::json;
  
  std::atomic<bool> received_;
  zmqpp::message received_message_;

  zmqpp::socket* socket_;
  std::thread* message_thread_;

  std::function<void(const json)> message_callback_;

  void CheckMessageReceipt ();

public:
  RPCResponse (zmqpp::socket*);
  ~RPCResponse ();
};

#endif // RPCRESPONSE_HPP
