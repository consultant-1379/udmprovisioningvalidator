#ifndef __UDM_PROVISIONING_VALIDATOR_VALIDATION_DATA__
#define __UDM_PROVISIONING_VALIDATOR_VALIDATION_DATA__

#include "entities/types.hpp"

namespace entities {

auto constexpr VALIDATION = 0;
auto constexpr CODE = 1;

class ValidationData final {
 public:
  ValidationData() = default;
  ValidationData(const changes_t &chs, const related_resources_t &relRsrcs)
      : changes{chs}, relatedResources{relRsrcs} {};
  ~ValidationData() = default;

  entities::validation_response_t applyValidationRules();
  void addError(std::string,
                std::initializer_list<std::pair<std::string, std::string>>);
  inline std::string emptyFieldError(const std::string &);
  inline std::string unfoundMandatoryFieldError(const std::string &);
  inline std::string unfoundMandatoryFieldError(const std::string &,
                                                const std::string &);
  inline std::string unfoundMandatoryFieldSliceDataError(const std::string &,
                                                         const std::string &);
  inline std::string notObjectFieldError(const std::string &);
  inline std::string notArrayFieldError(const std::string &);
  inline std::string notStringFieldError(const std::string &);
  inline std::string notIntFieldError(const std::string &);
  inline std::string notBoolFieldError(const std::string &);
  std::string getMscId(const std::string &);
  std::string buildProvJournalResourcePathFromMscId(const std::string &);
  bool isTac(const std::string &);
  bool hasRelatedResource(entities::resource_path_t &);
  std::string getSmsStaticDataMscId(const std::string &resourcePath);

  changes_t changes;
  related_resources_t relatedResources;
  response_t response;

 private:
  entities::validation_response_t checkForCreationUdmSubscriptionRules(
      entities::Change &);
  entities::validation_response_t checkForUpdateUdmSubscriptionRules(
      entities::Change &);
  bool checkForCommonUdmSubscriptionRules(entities::Change &,
                                          entities::udm_slice_prof_t &);
  bool checkForCommonUdmSliceProfileRules(
      entities::plmn_id_sets_t &plmnIdSets, const bool &isSliceProfile,
      const entities::resource_path_t &resourcePath);
  entities::validation_response_t checkForCreationUdmSliceProfileRules(
      entities::Change &);
  entities::validation_response_t checkForUpdateUdmSliceProfileRules(
      entities::Change &);
  bool checkForCreationSpecificUdmSliceProfileRules(
      entities::plmn_id_sets_t &plmnIdSets,
      const entities::resource_path_t &resourcePath);
  bool checkForUpdateSpecificUdmSliceProfileRules(entities::Change &,
                                                  entities::udm_slice_prof_t &);
  bool checkForCommonUdmRoamingAreaRules(entities::Change &);
  entities::validation_response_t checkForCreationUdmRoamingAreaRules(
      entities::Change &);
  entities::validation_response_t checkForUpdateUdmRoamingAreaRules(
      entities::Change &);
  void checkForRepeatedDnnsandNssais(
      entities::plmn_id_sets_t &plmnIdSets,
      const entities::resource_path_t &resourcePath, bool &);
  void checkForRepeatedNssais(const entities::resource_path_t &resourcePath,
                              std::vector<entities::Snssai> &, int, std::string,
                              bool &);
  void checkForRepeatedNssaisOnUpdate(
      entities::Change &, std::vector<entities::Snssai> &,
      std::map<entities::pair_nssai_key_t,
               std::vector<entities::tuple_nssai_t>>,
      const entities::plmn_id_set_identifier_t &, const std::string &, int,
      std::string, const entities::dnn_list_id_t &, bool &);
  void checkSscRepeated(const entities::resource_path_t &resourcePath,
                        std::vector<std::string> &, std::string, bool &);
  void checkPduSessionTypeRepeated(
      const entities::resource_path_t &resourcePath, std::vector<std::string> &,
      std::string, bool &);
  void computeMutations(entities::Change &);
  void includeNssai(entities::single_nssais_t &);
  bool isUdmStaticData(const std::string &);
  bool isUdmSliceProfile(const std::string &);
  bool isUdmRoamingArea(const std::string &);
  bool isSmsSubscription(const std::string &);
  std::map<entities::pair_t, std::vector<entities::dnn_t>>
  getAllDnnsUdmSliceProfile(entities::udm_slice_prof_t &);
  std::map<entities::pair_nssai_key_t, std::vector<entities::tuple_nssai_t>>
  getAllNssaisUdmSliceProfile(entities::udm_slice_prof_t &);
  std::vector<entities::tuple_nssai_t> getNssais(
      const entities::single_nssais_t &);
  bool getRelatedUdmSliceProfile(const entities::udm_slice_profile_id_t &,
                                 entities::udm_slice_prof_t &);
  bool checkRelatedUdmSliceProfile(const entities::udm_slice_profile_id_t &,
                                   entities::udm_slice_prof_t &,
                                   const std::string &,
                                   const entities::resource_path_t &);
};

std::string ValidationData::emptyFieldError(const std::string &field) {
  return "Field:[" + field + "] is empty";
}

std::string ValidationData::unfoundMandatoryFieldError(
    const std::string &field) {
  return "Mandatory field:[" + field + "] not found";
}

std::string ValidationData::unfoundMandatoryFieldError(
    const std::string &field, const std::string &parent) {
  return "Mandatory field:[" + field + "] not found as child of:[" + parent +
         "]";
}

std::string ValidationData::unfoundMandatoryFieldSliceDataError(
    const std::string &field, const std::string &parent) {
  return "When \"" + parent +
         "\" is included in \"udmSliceData\", it must have at least one "
         "element in \"" +
         field + "\"";
}

std::string ValidationData::notObjectFieldError(const std::string &field) {
  return "Field:[" + field + "] is not an object";
}

std::string ValidationData::notArrayFieldError(const std::string &field) {
  return "Field:[" + field + "] is not an array";
}

std::string ValidationData::notStringFieldError(const std::string &field) {
  return "Field:[" + field + "] is not string";
}

std::string ValidationData::notIntFieldError(const std::string &field) {
  return "Field:[" + field + "] is not integer";
}

std::string ValidationData::notBoolFieldError(const std::string &field) {
  return "Field:[" + field + "] is not boolean";
}
}  // namespace entities

#endif
