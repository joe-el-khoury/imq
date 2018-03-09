#include "RPCCall.hpp"

rpc::RPCCall::RPCCall (const std::string& rpc, const json& args)
  : rpc_(rpc), args_(args)
{}

void rpc::RPCCall::OnMessageReceipt (MessageCallback message_receipt_callback)
{
  message_receipt_callback_ = message_receipt_callback;
}

void rpc::RPCCall::OnTimeout (unsigned timeout_duration, TimeoutCallback timeout_callback)
{
  timeout_duration_ = timeout_duration;
  timeout_callback_ = timeout_callback;
  timeout_callback_set_ = true;
}

rpc::RPCCall::MessageCallback rpc::RPCCall::GetMessageCallback () const
{
  return message_receipt_callback_;
}

rpc::RPCCall::TimeoutCallback rpc::RPCCall::GetTimeoutCallback () const
{
  return timeout_callback_;
}

unsigned rpc::RPCCall::GetTimeoutDuration () const
{
  return timeout_duration_;
}

bool rpc::RPCCall::TimeoutIsSet () const
{
  return timeout_callback_set_;
}

const std::string& rpc::RPCCall::GetRPCName () const
{
  return rpc_;
}

const rpc::RPCCall::json& rpc::RPCCall::GetArgs () const
{
  return args_;
}
