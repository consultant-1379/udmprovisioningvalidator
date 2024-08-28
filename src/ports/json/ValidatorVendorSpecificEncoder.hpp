#ifndef __UDM_PROVISIONING_VALIDATOR_VENDOR_SPECIFIC_ENCODER_HPP__
#define __UDM_PROVISIONING_VALIDATOR_VENDOR_SPECIFIC_ENCODER_HPP__

#include "entities/types.hpp"
#include "rapidjson/document.h"

namespace port {
namespace secondary {
namespace json {

class ValidatorVendorSpecificEncoder final {
 public:
  static void vendorSpecificToJson(rapidjson::Value &,
                                   const entities::vendorSpecific_t &,
                                   rapidjson::Document::AllocatorType &);

 private:
  static rapidjson::Document getVendorSpecific(
      const rapidjson::Value &, rapidjson::Document::AllocatorType &);
};

}  // namespace json
}  // namespace secondary
}  // namespace port

#endif  //__UDM_PROVISIONING_VALIDATOR_VENDOR_SPECIFIC_ENCODER_HPP__
