#ifndef RPCRESPONSE_HPP
#define RPCRESPONSE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

#include "../json.hpp"

namespace rpc {

class RPCResponse
{
private:
  using json = nlohmann::json;
  
  std::atomic<bool> received_;

  zmqpp::socket* socket_;
  std::thread* message_thread_;

  std::function<void(const json&)> message_callback_;
  std::atomic<bool> did_message_callback_;
  void DoMessageCallback (zmqpp::message&);
  
  unsigned start_time_;
  unsigned timeout_duration_ = 0;
  unsigned CurrentTime ();
  bool HasTimedOut ();
  
  std::function<void()> timeout_callback_;
  bool timeout_callback_set_ = false;
  void DoTimeoutCallback ();

  void CheckMessageReceipt ();

public:
  RPCResponse (zmqpp::socket*);
  RPCResponse (const RPCResponse&);
  ~RPCResponse ();

  // Register a callback for when a message is received.
  void OnMessageReceipt (std::function<void(const json&)>);

  // Register a callback for when we time out.
  void OnTimeout (unsigned, std::function<void()>);
};

}

#endif // RPCRESPONSE_HPP
