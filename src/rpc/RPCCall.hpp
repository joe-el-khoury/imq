#ifndef RPCCALL_HPP
#define RPCCALL_HPP

#include "../json.hpp"
#include "utils/HostAndPort.hpp"

namespace rpc {

class RPCCall
{
private:
  using json = nlohmann::json;

  using MessageCallback = std::function<void(const json&)>;
  using TimeoutCallback = std::function<void()>;

  bool message_callback_set_ = false;
  MessageCallback message_receipt_callback_;
  
  unsigned timeout_duration_ = 3000 /* default */;
  bool timeout_callback_set_ = false;
  TimeoutCallback timeout_callback_;

  std::string rpc_;
  json args_;
  bool async_;

  // Data about the node we're calling.
  HostAndPort callee_info_;

public:
  RPCCall (const std::string&, const json&, bool=false);
  RPCCall (const std::string&, bool=false);
  
  void SetMessageCallback (MessageCallback);
  void SetTimeoutCallback (unsigned, TimeoutCallback);

  bool MessageCallbackIsSet () const { return message_callback_set_; }

  unsigned GetTimeoutDuration () const { return timeout_duration_; }
  bool TimeoutIsSet () const { return timeout_callback_set_; }

  MessageCallback GetMessageCallback () const { return message_receipt_callback_; }
  TimeoutCallback GetTimeoutCallback () const { return timeout_callback_; }

  const std::string& GetRPCName () const { return rpc_; }
  const json& GetArgs () const { return args_; }
  bool IsAsync () const { return async_; }

  void SetCalleeHost (const std::string& host) { callee_info_.host = host; }
  void SetCalleePort (unsigned port) { callee_info_.port = port; }
  std::string GetCalleeHost () const { return callee_info_.host; }
  unsigned GetCalleePort () const { return callee_info_.port; }
};

} // namespace rpc

#endif // RPCCALL_HPP
