#ifndef __UDM_PROVISIONING_VALIDATOR_RAPIDJSON_ENCODER_HPP__
#define __UDM_PROVISIONING_VALIDATOR_RAPIDJSON_ENCODER_HPP__

#include <sstream>

#include "entities/ProblemDetails.hpp"
#include "entities/ValidationData.hpp"
#include "rapidjson/document.h"

namespace port {
namespace secondary {
namespace json {

class ValidatorRapidJsonEncoder final {
 public:
  ValidatorRapidJsonEncoder();
  ValidatorRapidJsonEncoder(ValidatorRapidJsonEncoder &&) = delete;
  ~ValidatorRapidJsonEncoder() noexcept = default;
  std::string toStringJson(const ::entities::ProblemDetails &);
  std::ostringstream toJson(const ::entities::ProblemDetails &);
  std::ostringstream validatorResponseToJson(
      const ::entities::ValidationData &);
  std::ostringstream errorResponseToJson(const ::entities::Error &);
  inline void enablePrettyFormat();

 private:
  rapidjson::Document rJsonDoc;
  bool prettyFormat;

  void acceptWriter(std::ostringstream &) const;
  void udmStaticDataToJson(rapidjson::Value &,
                           const entities::udm_static_data_t &,
                           rapidjson::Document::AllocatorType &);
  void smsStaticDataToJson(rapidjson::Value &,
                           const entities::sms_static_data_t &,
                           rapidjson::Document::AllocatorType &);
  void udmSliceProfileToJson(rapidjson::Value &,
                             const entities::udm_slice_prof_t &,
                             rapidjson::Document::AllocatorType &);
  void plmnIdsToJson(rapidjson::Value &, const entities::plmn_id_sets_t &,
                     rapidjson::Document::AllocatorType &);
  void udmRoamingAreaToJson(rapidjson::Value &,
                            const entities::udm_roaming_area_t &,
                            rapidjson::Document::AllocatorType &);
  void vendorSpecificToJson(rapidjson::Value &,
                            const entities::vendorSpecific_t &,
                            rapidjson::Document::AllocatorType &);
  rapidjson::Document getVendorSpecific(const rapidjson::Value &,
                                        rapidjson::Document::AllocatorType &);
  void strToJson(rapidjson::Value &, const std::string &, const std::string &,
                 rapidjson::Document::AllocatorType &);
  void boolToJson(rapidjson::Value &, const bool &, const std::string &,
                  rapidjson::Document::AllocatorType &);
  void intToJson(rapidjson::Value &, const int &, const std::string &,
                 rapidjson::Document::AllocatorType &);
};

void ValidatorRapidJsonEncoder::enablePrettyFormat() { prettyFormat = true; }

}  // namespace json
}  // namespace secondary
}  // namespace port

#endif  //__UDM_PROVISIONING_VALIDATOR_RAPIDJSON_ENCODER_HPP__
