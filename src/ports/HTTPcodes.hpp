#ifndef __UDM_PROVISIONING_VALIDATOR_PORTS_HTTP_CODES__
#define __UDM_PROVISIONING_VALIDATOR_PORTS_HTTP_CODES__

namespace port {
constexpr auto HTTP_OK = 200;
constexpr auto HTTP_BAD_REQUEST = 400;
constexpr auto HTTP_CONFLICT = 409;
constexpr auto HTTP_UNPROCESSABLE_ENTITY = 422;
constexpr auto HTTP_INTERNAL_SERVER_ERROR = 500;

}  // namespace port

#endif
