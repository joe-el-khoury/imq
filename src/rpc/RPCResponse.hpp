#ifndef RPCRESPONSE_HPP
#define RPCRESPONSE_HPP

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

#include "RPCCall.hpp"

#include "../json.hpp"

namespace rpc {

class RPCResponse
{
private:
  using json = nlohmann::json;
  
  std::atomic<bool> received_;
  zmqpp::message received_message_;

  zmqpp::socket* socket_;
  std::thread* message_thread_;

  // The RPC call we are responding to. It provides context we need to respond, like callbacks.
  const rpc::RPCCall& rpc_call_;
  
  unsigned start_time_;
  unsigned CurrentTime ();
  bool HasTimedOut ();

  void DoMessageCallback (zmqpp::message&);
  void DoTimeoutCallback ();

  void CheckMessageReceipt ();

public:
  RPCResponse (zmqpp::socket*, const rpc::RPCCall&);
  RPCResponse (const RPCResponse&);
  ~RPCResponse ();

  // Register a callback for when a message is received.
  void OnMessageReceipt (std::function<void(const json&)>);

  // Register a callback for when we time out.
  void OnTimeout (unsigned, std::function<void()>);
};

}

#endif // RPCRESPONSE_HPP
