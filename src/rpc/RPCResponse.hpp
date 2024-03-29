#ifndef RPCRESPONSE_HPP
#define RPCRESPONSE_HPP

#include "RPCCall.hpp"

#include "../json.hpp"

#include <zmqpp/zmqpp.hpp>

#include <thread>
#include <atomic>

namespace rpc {

class RPCResponse
{
private:
  using json = nlohmann::json;
  
  std::atomic<bool> received_;
  zmqpp::message received_message_;

  std::atomic<bool> timedout_;

  zmqpp::socket* socket_;
  std::thread* message_thread_;

  // The RPC call we are responding to. It provides context we need to respond, like callbacks.
  const rpc::RPCCall& rpc_call_;
  
  unsigned start_time_;
  unsigned CurrentTime ();
  bool HasTimedOut ();

  // Adds callee info to the json.
  void AddResponseMetadata (json&);

  void DoMessageCallback (zmqpp::message&);
  void DoTimeoutCallback ();

  std::atomic<bool> running_;
  void MessageReceiptMainWorkLoop (bool);

public:
  RPCResponse (zmqpp::socket*, const rpc::RPCCall&);
  RPCResponse (const RPCResponse&);
  ~RPCResponse ();

  json Get ();
};

}

#endif // RPCRESPONSE_HPP
