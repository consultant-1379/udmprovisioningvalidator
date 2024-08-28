#include "ValidatorSmsSubsParser.hpp"

#include "ValidatorVendorSpecificParser.hpp"

namespace port {
namespace secondary {
namespace json {

::entities::existance_t ValidatorSmsSubsParser::setBooleanField(
    const rapidjson::Value& attrData, bool& value, const char* fieldName) {
  if (attrData[fieldName].IsBool()) {
    value = attrData[fieldName].GetBool();
    return true;
  }
  data->addError("Constraint Violation",
                 {{"resource_path", this->resourcePath},
                  {"description", data->notBoolFieldError(fieldName)}});
  return false;
}

::entities::existance_t ValidatorSmsSubsParser::setOptionalBooleanField(
    const rapidjson::Value& attrData, bool& value, const char* fieldName) {
  if (attrData.HasMember(fieldName)) {
    return this->setBooleanField(attrData, value, fieldName);
  }
  return false;
}

ValidatorSmsSubsParser::ValidatorSmsSubsParser(
    ::entities::ValidationData& data,
    const ::entities::resource_path_t resourcePath) {
  this->data = &data;
  this->resourcePath = resourcePath;
}

::entities::existance_t ValidatorSmsSubsParser::getSmsSubscriptionData(
    const rapidjson::Value& attrData,
    ::entities::sms_static_data_t& smsStaticData) {
  /* Optional field. */
  if (attrData.HasMember(JSON_SMS_SUBSCRIPTION_DATA)) {
    const rapidjson::Value& smsSubscriptionData =
        attrData[JSON_SMS_SUBSCRIPTION_DATA];
    if (smsSubscriptionData.IsObject()) {
      smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed =
          this->setOptionalBooleanField(
              smsSubscriptionData,
              smsStaticData.smsSubscriptionData.smsSubscribed,
              JSON_SMS_SUBSCRIBED);

      ValidatorVendorSpecificParser::getVendorSpecific(
          attrData[JSON_SMS_SUBSCRIPTION_DATA],
          smsStaticData.smsSubscriptionData.vendorSpecific);

      return true;

    } else {
      data->addError("Constraint Violation",
                     {{"resource_path", this->resourcePath},
                      {"description",
                       data->notObjectFieldError(JSON_SMS_SUBSCRIPTION_DATA)}});
    }
  }
  return false;
}

::entities::existance_t
ValidatorSmsSubsParser::getSmsManagementSubscriptionData(
    const rapidjson::Value& attrData,
    ::entities::sms_static_data_t& smsStaticData) {
  /* Optional field*/
  if (attrData.HasMember(JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA)) {
    if (attrData[JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA].IsObject()) {
      smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed =
          this->setOptionalBooleanField(
              attrData[JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA],
              smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed,
              JSON_MT_SMS_SUBSCRIBED);

      smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll =
          this->setOptionalBooleanField(
              attrData[JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA],
              smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll,
              JSON_MT_SMS_BARRING_ALL);

      smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
          this->setOptionalBooleanField(
              attrData[JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA],
              smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming,
              JSON_MT_SMS_BARRING_ROAMING);

      smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed =
          this->setOptionalBooleanField(
              attrData[JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA],
              smsStaticData.smsManagementSubscriptionData.moSmsSubscribed,
              JSON_MO_SMS_SUBSCRIBED);

      smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll =
          this->setOptionalBooleanField(
              attrData[JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA],
              smsStaticData.smsManagementSubscriptionData.moSmsBarringAll,
              JSON_MO_SMS_BARRING_ALL);

      smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
          this->setOptionalBooleanField(
              attrData[JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA],
              smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming,
              JSON_MO_SMS_BARRING_ROAMING);

      ValidatorVendorSpecificParser::getVendorSpecific(
          attrData[JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA],
          smsStaticData.smsManagementSubscriptionData.vendorSpecific);

      return true;
    } else {
      data->addError(
          "Constraint Violation",
          {{"resource_path", this->resourcePath},
           {"description",
            data->notObjectFieldError(JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA)}});
    }
  }
  return false;
}

void ValidatorSmsSubsParser::getOptionalSmsStaticData(
    const rapidjson::Value& attrData,
    ::entities::sms_static_data_t& smsStaticData) {
  ValidatorVendorSpecificParser::getVendorSpecific(
      attrData, smsStaticData.vendorSpecific);

  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      this->getSmsSubscriptionData(attrData, smsStaticData);

  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      this->getSmsManagementSubscriptionData(attrData, smsStaticData);
}

}  // namespace json
}  // namespace secondary
}  // namespace port
