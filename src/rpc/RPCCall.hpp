#ifndef RPCCALL_HPP
#define RPCCALL_HPP

#include "../json.hpp"

namespace rpc {

class RPCCall
{
private:
  using json = nlohmann::json;

  using MessageCallback = std::function<void(const json&)>;
  using TimeoutCallback = std::function<void()>;

  MessageCallback message_receipt_callback_;
  
  unsigned timeout_duration_ = 0;
  bool timeout_callback_set_ = false;
  TimeoutCallback timeout_callback_;

  const std::string& rpc_;
  const json& args_;

  const std::string& GetRPCName ();
  const json& GetArgs ();

public:
  RPCCall (const std::string&, const json&);
  
  void OnMessageReceipt (MessageCallback);
  void OnTimeout (unsigned, TimeoutCallback);

  MessageCallback GetMessageCallback ();
  TimeoutCallback GetTimeoutCallback ();

  unsigned GetTimeoutDuration ();
  bool TimeoutIsSet ();
};

} // namespace rpc

#endif // RPCCALL_HPP
