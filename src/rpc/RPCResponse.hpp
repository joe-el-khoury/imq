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

  zmqpp::socket* socket_;
  std::thread* message_thread_;

  // The RPC call we are responding to. It provides context we need to respond, like callbacks.
  const rpc::RPCCall& rpc_call_;
  
  unsigned start_time_;
  unsigned CurrentTime ();
  bool HasTimedOut ();

  void DoMessageCallback (zmqpp::message&);
  void DoTimeoutCallback ();

  void MessageReceiptMainWorkLoop ();

  std::atomic<bool> running_;

public:
  RPCResponse (zmqpp::socket*, const rpc::RPCCall&);
  RPCResponse (const RPCResponse&);
  ~RPCResponse ();

  json Get ();
};

}

#endif // RPCRESPONSE_HPP
