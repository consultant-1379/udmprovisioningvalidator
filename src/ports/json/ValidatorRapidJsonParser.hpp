#ifndef __UDM_PROVISIONING_VALIDATOR_RAPIDJSON_PARSER_HPP__
#define __UDM_PROVISIONING_VALIDATOR_RAPIDJSON_PARSER_HPP__

#include "entities/ValidationData.hpp"
#include "rapidjson/document.h"

namespace port {
namespace secondary {
namespace json {

class ValidatorRapidJsonParser final {
 public:
  ValidatorRapidJsonParser() = default;
  explicit ValidatorRapidJsonParser(const std::string&);
  ValidatorRapidJsonParser(ValidatorRapidJsonParser&&) = delete;
  ~ValidatorRapidJsonParser() = default;
  inline bool error() const;
  inline const std::string errorString() const;
  bool getValidationData(entities::ValidationData&);
  void getRelatedResources(entities::ValidationData&);

 private:
  bool getChangesToValidate(entities::ValidationData&);
  void getUdmStaticData(const rapidjson::Value&, entities::UdmStaticData&,
                        entities::ValidationData&, const std::string&);
  void getProvJournal(const rapidjson::Value&, entities::ProvJournal&);
  void getUdmSliceProfile(const rapidjson::Value&, entities::udm_slice_prof_t&,
                          entities::ValidationData&, const std::string&);
  entities::plmn_id_sets_t getPlmnIdSets(const rapidjson::Value&,
                                         entities::ValidationData&,
                                         const std::string&, const bool&);
  void getUdmRoamingAreas(const rapidjson::Value&,
                          entities::udm_roaming_area_t&,
                          entities::ValidationData&, const std::string&);
  inline void setParsingError(const std::string&);
  inline void setEmptyFieldError(const std::string&);
  inline void setUnfoundMandatoryFieldError(const std::string&);
  inline void setUnfoundMandatoryFieldError(const std::string&,
                                            const std::string&);
  std::string getString(const rapidjson::Value&, const char*);
  rapidjson::Document rJsonDoc;
  bool rJsonParseError;
  std::string rJsonParseErrorStr;
};

bool ValidatorRapidJsonParser::error() const { return rJsonParseError; }

const std::string ValidatorRapidJsonParser::errorString() const {
  return rJsonParseErrorStr;
}

void ValidatorRapidJsonParser::setParsingError(const std::string& err) {
  rJsonParseError = true;
  rJsonParseErrorStr = err;
}

void ValidatorRapidJsonParser::setEmptyFieldError(const std::string& field) {
  setParsingError("Field:[" + field + "] is empty");
}

void ValidatorRapidJsonParser::setUnfoundMandatoryFieldError(
    const std::string& field) {
  setParsingError("Mandatory field:[" + field + "] not found");
}

void ValidatorRapidJsonParser::setUnfoundMandatoryFieldError(
    const std::string& field, const std::string& parent) {
  setParsingError("Mandatory field:[" + field + "] not found as child of:[" +
                  parent + "]");
}

}  // namespace json
}  // namespace secondary
}  // namespace port

#endif  //__UDM_PROVISIONING_VALIDATOR_RAPIDJSON_PARSER_HPP__
