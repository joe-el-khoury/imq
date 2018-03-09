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

public:
  RPCCall (const std::string&, const json&);
  
  void OnMessageReceipt (MessageCallback);
  void OnTimeout (unsigned, TimeoutCallback);

  MessageCallback GetMessageCallback () const;
  TimeoutCallback GetTimeoutCallback () const;

  unsigned GetTimeoutDuration () const;
  bool TimeoutIsSet () const;

  const std::string& GetRPCName () const;
  const json& GetArgs () const;
};

} // namespace rpc

#endif // RPCCALL_HPP
