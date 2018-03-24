#include "RPCCall.hpp"

rpc::RPCCall::RPCCall (const std::string& rpc, const json& args, bool async)
  : rpc_(rpc), args_(args), async_(async)
{}

void rpc::RPCCall::OnMessageReceipt (MessageCallback message_receipt_callback)
{
  message_receipt_callback_ = message_receipt_callback;
  message_callback_set_ = true;
}

void rpc::RPCCall::OnTimeout (unsigned timeout_duration, TimeoutCallback timeout_callback)
{
  timeout_duration_ = timeout_duration;
  timeout_callback_ = timeout_callback;
  timeout_callback_set_ = true;
}
