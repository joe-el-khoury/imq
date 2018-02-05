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

  std::function<void(const json&)> message_callback_;
  std::atomic<bool> did_callback_;

  void DoCallback (zmqpp::message&);
  void CheckMessageReceipt ();

public:
  RPCResponse (zmqpp::socket*);
  ~RPCResponse ();

  // Register a callback for when a message is received.
  void OnMessageReceipt (std::function<void(const json&)>);
};

#endif // RPCRESPONSE_HPP
