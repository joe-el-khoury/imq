#ifndef RPC_ERRORS_HPP
#define RPC_ERRORS_HPP

#include "json.hpp"

#include <string>

namespace rpc {
namespace except {

class RPCException
{
private:
  using json = nlohmann::json;

  std::string name_;

protected:
  RPCException (const std::string& name) : name_(name)
  {}

public:
  json AsJson ()
  {
    return {{"error", name_}};
  }
};

class TimeoutException : public RPCException
{
public:
  TimeoutException ()
    : RPCException("TimeoutException")
  {}
};

class InternalException : public RPCException
{
public:
  InternalException ()
    : RPCException("InternalException")
  {}
};

} // namespace error
} // namespace rpc

#endif // RPC_ERRORS_HPP
