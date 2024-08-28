#ifndef __UDM_PROVISIONING_VALIDATOR_VENDOR_SPECIFIC_PARSER_HPP__
#define __UDM_PROVISIONING_VALIDATOR_VENDOR_SPECIFIC_PARSER_HPP__

#include "entities/types.hpp"
#include "rapidjson/document.h"

namespace port {
namespace secondary {
namespace json {

class ValidatorVendorSpecificParser final {
 public:
  static void getVendorSpecific(const rapidjson::Value&,
                                entities::vendorSpecific_t&);

 private:
  static bool isVendorSpecific(const std::string&);
};

}  // namespace json
}  // namespace secondary
}  // namespace port

#endif  //__UDM_PROVISIONING_VALIDATOR_VENDOR_SPECIFIC_PARSER_HPP__
