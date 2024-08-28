#ifndef __VALIDATOR_SMS_SUBS_PARSER__
#define __VALIDATOR_SMS_SUBS_PARSER__

#include "JsonConstants.hpp"
#include "entities/ValidationData.hpp"
#include "entities/types.hpp"

namespace port {
namespace secondary {
namespace json {

class ValidatorSmsSubsParser final {
 private:
  ::entities::ValidationData* data;
  ::entities::resource_path_t resourcePath;

  ::entities::existance_t setBooleanField(const rapidjson::Value& attrData,
                                          bool& value, const char* fieldName);

  ::entities::existance_t setOptionalBooleanField(
      const rapidjson::Value& attrData, bool& value, const char* fieldName);

  ::entities::existance_t getSmsSubscriptionData(
      const rapidjson::Value& attrData,
      ::entities::sms_static_data_t& smsStaticData);

  ::entities::existance_t getSmsManagementSubscriptionData(
      const rapidjson::Value& attrData,
      ::entities::sms_static_data_t& smsStaticData);

 public:
  ValidatorSmsSubsParser(::entities::ValidationData& data,
                         const ::entities::resource_path_t resourcePath);

  void getOptionalSmsStaticData(const rapidjson::Value& attrData,
                                ::entities::sms_static_data_t& smsStaticData);
};

}  // namespace json
}  // namespace secondary
}  // namespace port

#endif  // !__VALIDATOR_SMS_SUBS_PARSER__