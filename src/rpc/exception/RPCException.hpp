#ifndef RPC_ERRORS_HPP
#define RPC_ERRORS_HPP

#include "json.hpp"

namespace rpc {
namespace except {

using json = nlohmann::json;

json TimeoutExceptionAsJson ()
{
  return {{"error", "TimeoutException"}};
}

json InternalExceptionAsJson ()
{
  return {{"error", "InternalException"}};
}

} // namespace error
} // namespace rpc

#endif // RPC_ERRORS_HPP
