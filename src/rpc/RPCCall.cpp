#include "RPCCall.hpp"

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

rpc::RPCCall::MessageCallback rpc::RPCCall::GetMessageCallback ()
{
  return message_receipt_callback_;
}

rpc::RPCCall::TimeoutCallback rpc::RPCCall::GetTimeoutCallback ()
{
  return timeout_callback_;
}

unsigned rpc::RPCCall::GetTimeoutDuration ()
{
  return timeout_duration_;
}

bool rpc::RPCCall::TimeoutIsSet ()
{
  return timeout_callback_set_;
}
