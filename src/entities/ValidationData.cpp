#include "entities/ValidationData.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <regex>

#include "entities/SmsValidationData.hpp"
#include "log/logout.hpp"
#include "ports/HTTPcodes.hpp"
#include "ports/json/JsonConstants.hpp"
namespace entities {

static const auto MSCID_UDM_STATIC_DATA_REGEXP = boost::regex{
    "^/subscribers/(.*?)/udmSubscription/udmStaticAndRegistrationData/"
    "udmStaticData$"};

static const auto POS_MSCID = 1;

static const auto MSCID_SMS_STATIC_DATA_REGEXP = boost::regex{
    "^/subscribers/(.*?)/smsSubscription/smsStaticAndRegistrationData/"
    "smsStaticData$"};

static const auto UDM_SLICE_PROFILE_REGEXP =
    boost::regex{"^/profiles/udmProfiles/udmSliceProfiles/(.*?)"};

static const auto UDM_ROAMING_AREAS_REGEXP =
    boost::regex{"^/profiles/udmProfiles/udmRoamingAreas/(.*?)"};

static const auto BIT_RATE_REGEXP =
    boost::regex{"^\\d+(\\.\\d+)? (bps|Kbps|Mbps|Gbps|Tbps)$"};

static const auto SD_REGEXP = boost::regex{"^[A-Fa-f0-9]{6}$"};

static const auto IPV4_REGEXP = boost::regex{
    "^(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-"
    "9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$"};

static const auto IPV6_REGEXP_1 = boost::regex{
    "^((:|(0?|([1-9a-f][0-9a-f]{0,3}))):)((0?|([1-9a-f][0-9a-f]{0,3})):){0,6}(:"
    "|(0?|([1-9a-f][0-9a-f]{0,3})))$"};

static const auto IPV6_REGEXP_2 = boost::regex{
    "^((([^:]+:){7}([^:]+))|((([^:]+:)*[^:]+)?::(([^:]+:)*[^:]+)?))$"};

static const auto IPV6_PREFIX_REGEXP_1 = boost::regex{
    "^((:|(0?|([1-9a-f][0-9a-f]{0,3}))):)((0?|([1-9a-f][0-9a-f]{0,3})):){0,6}(:"
    "|(0?|([1-9a-f][0-9a-f]{0,3})))(\\/"
    "(([0-9])|([0-9]{2})|(1[0-1][0-9])|(12[0-8])))$"};

static const auto IPV6_PREFIX_REGEXP_2 = boost::regex{
    "^((([^:]+:){7}([^:]+))|((([^:]+:)*[^:]+)?::(([^:]+:)*[^:]+)?))(\\/.+)$"};

static const auto PLMNID_SET_IDENTIFIER_REGEXP =
    boost::regex{"^\\d{5,6}$|Default"};

static const auto THREEGPP_CHARGING_CHARACTERISTICS_REGEX =
    boost::regex{("(^[A-Fa-f0-9]{4}$)")};

static const auto DEFAULT_VALUE = "Default";

static const auto TACS_REGEXP =
    boost::regex{"(^[A-Fa-f0-9]{4}$)|(^[A-Fa-f0-9]{6}$)"};

static const auto UUID_REGEX = boost::regex{
    "^[0-9a-fA-F]{8}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{4}\\-["
    "0-9a-fA-F]{4}\\-[0-"
    "9a-fA-F]{12}$"};

static const auto UE_USAGE_TYPE_MIN = 0;
static const auto UE_USAGE_TYPE_MAX = 255;

static const auto IP_ADDRESS_DNN_MIN_SIZE = 1;
static const auto IP_ADDRESS_DNN_MAX_SIZE = 2;

static const auto DEFAULT_KEY = "default";
static const auto SINGLE_KEY = "single";

std::string ValidationData::getMscId(const std::string& resourcePath) {
  boost::smatch sm;

  if (boost::regex_search(resourcePath, sm, MSCID_UDM_STATIC_DATA_REGEXP)) {
    return sm[POS_MSCID];
  }
  return {};
}

bool ValidationData::isTac(const std::string& tac) {
  boost::cmatch cm;
  return boost::regex_match(tac.c_str(), cm, TACS_REGEXP);
}

bool ValidationData::isUdmStaticData(const std::string& resourcePath) {
  boost::cmatch cm;
  return boost::regex_match(resourcePath.c_str(), cm,
                            MSCID_UDM_STATIC_DATA_REGEXP);
}

bool ValidationData::isSmsSubscription(const std::string& resourcePath) {
  boost::cmatch cm;
  return boost::regex_match(resourcePath.c_str(), cm,
                            MSCID_SMS_STATIC_DATA_REGEXP);
}

std::string ValidationData::getSmsStaticDataMscId(
    const std::string& resourcePath) {
  boost::smatch sm;

  if (boost::regex_search(resourcePath, sm, MSCID_SMS_STATIC_DATA_REGEXP)) {
    return sm[POS_MSCID];
  }
  return {};
}

bool ValidationData::isUdmSliceProfile(const std::string& resourcePath) {
  boost::cmatch cm;
  return boost::regex_match(resourcePath.c_str(), cm, UDM_SLICE_PROFILE_REGEXP);
}

bool ValidationData::isUdmRoamingArea(const std::string& resourcePath) {
  boost::cmatch cm;
  return boost::regex_match(resourcePath.c_str(), cm, UDM_ROAMING_AREAS_REGEXP);
}

std::string ValidationData::buildProvJournalResourcePathFromMscId(
    const std::string& mscId) {
  return std::string{"/subscribers/"}.append(mscId).append(
      "/journal/provJournal");
}

entities::validation_response_t ValidationData::applyValidationRules() {
  bool ret = true;
  auto code = ::port::HTTP_OK;
  for (auto c : changes) {
    entities::validation_response_t resp{true, ::port::HTTP_OK};

    if (isUdmStaticData(c.resourcePath) &&
        (not c.operation.compare(JSON_OPERATION_CREATE))) {
      resp = checkForCreationUdmSubscriptionRules(c);
    } else if (isUdmStaticData(c.resourcePath) &&
               (not c.operation.compare(JSON_OPERATION_UPDATE))) {
      resp = checkForUpdateUdmSubscriptionRules(c);
    } else if (isUdmSliceProfile(c.resourcePath) &&
               (not c.operation.compare(JSON_OPERATION_CREATE))) {
      resp = checkForCreationUdmSliceProfileRules(c);
    } else if (isUdmSliceProfile(c.resourcePath) &&
               (not c.operation.compare(JSON_OPERATION_UPDATE))) {
      resp = checkForUpdateUdmSliceProfileRules(c);
    } else if (isUdmRoamingArea(c.resourcePath) &&
               (not c.operation.compare(JSON_OPERATION_CREATE))) {
      resp = checkForCreationUdmRoamingAreaRules(c);
    } else if (isUdmRoamingArea(c.resourcePath) &&
               (not c.operation.compare(JSON_OPERATION_UPDATE))) {
      resp = checkForUpdateUdmRoamingAreaRules(c);
    } else if (isSmsSubscription(c.resourcePath)) {
      SmsValidationData smsValidator(this, c);
      resp = smsValidator.applyValidationRules();
    } else if (c.operation.compare(JSON_OPERATION_DELETE)) {
      resp = {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
      addError("Unprocessable entity", {{"resource_path", c.resourcePath},
                                        {"description",
                                         "Validation could not be performed on "
                                         "the specified resource_path"}});
    }

    ret &= std::get<entities::VALIDATION>(resp);
    // HTTP_UNPROCESSABLE_ENTITY (422) has preference over HTTP_CONFLICT (409)
    // and HTTP_CONFLICT (409) has preference over HTTP_OK (200)
    code = std::max(code, std::get<entities::CODE>(resp));

    if (ret) {
      entities::Change responseChange = c;
      computeMutations(responseChange);
      response.changes.push_back(responseChange);
    }
  }

  return {ret, code};
}

void ValidationData::computeMutations(entities::Change& change) {
  std::size_t index = change.resourcePath.find(JSON_UDM_SLICE_PROFILES);
  if ((index != std::string::npos) &&
      ((not change.operation.compare(JSON_OPERATION_CREATE)) ||
       not change.operation.compare(JSON_OPERATION_UPDATE))) {
    for (auto& elem : change.udmSliceProfile.plmnIdSets) {
      includeNssai(elem.nssai.defaultSingleNssais);
      if (elem.fieldExistenceUdmSliceProfile.hasNssai) {
        includeNssai(elem.nssai.singleNssais);
      }
    }
  }

  // This mutation should be also apply to udmSliceData under UdmStaticData.
  index = change.resourcePath.find(JSON_UDM_STATIC_DATA);
  if (index != std::string::npos) {
    for (auto& plmnIdSet : change.udmStaticData.udmSliceData.plmnIdSets) {
      includeNssai(plmnIdSet.nssai.defaultSingleNssais);
      if (plmnIdSet.fieldExistenceUdmSliceProfile.hasNssai) {
        includeNssai(plmnIdSet.nssai.singleNssais);
      }
    }
  }
}

void ValidationData::includeNssai(entities::single_nssais_t& nssai) {
  if (nssai.size() > 0) {
    for (auto& subElem : nssai) {
      if (subElem.dnnListId.empty()) {
        subElem.dnnListId = DEFAULT_VALUE;
      }
    }
  }
}

bool ValidationData::checkForCommonUdmSubscriptionRules(
    entities::Change& change, entities::udm_slice_prof_t& sliceProfile) {
  bool ret = true;

  // Only one reference to an udmRoamingArea included in serviceAreaRestriction
  // attribute
  if (change.udmStaticData.serviceAreaRestriction.areas.size() > 1) {
    ret = false;
    addError("Constraint Violation",
             {{"resource_path", change.resourcePath},
              {"description",
               "Only one reference to an udmRoamingArea allowed in "
               "\"" +
                   std::string{JSON_SERVICE_AREA_RESTRICTION} + "\""}});
  }

  // Only one reference to an udmRoamingArea included in forbiddenAreas
  // attribute
  if (change.udmStaticData.forbiddenAreas.size() > 1) {
    ret = false;
    addError("Constraint Violation",
             {{"resource_path", change.resourcePath},
              {"description",
               "Only one reference to an udmRoamingArea allowed in "
               "\"" +
                   std::string{JSON_FORBIDDEN_AREAS} + "\""}});
  }

  // serviceAreaRestriction attribute:
  // - restrictionType is only present if attribute areas is present
  // - maxNumOfTAs is only present if restriction type is not
  // "NOT_ALLOWED_AREAS"
  // - areas can be an empty array
  if (not change.udmStaticData.serviceAreaRestriction.restrictionType.empty() &&
      not change.udmStaticData.serviceAreaRestriction.areas.size()) {
    ret = false;
    addError("Constraint Violation",
             {{"resource_path", change.resourcePath},
              {"description", "\"" + std::string{JSON_RESTRICTION_TYPE} +
                                  "\" in \"" +
                                  std::string{JSON_SERVICE_AREA_RESTRICTION} +
                                  "\" can be only present if \"" +
                                  std::string{JSON_AREAS} + "\" is present"}});
  }

  if (change.udmStaticData.fieldExistence.hasMaxNumOfTas &&
      not change.udmStaticData.serviceAreaRestriction.restrictionType.compare(
          JSON_NOT_ALLOWED_AREAS)) {
    ret = false;
    addError("Constraint Violation",
             {{"resource_path", change.resourcePath},
              {"description",
               "\"" + std::string{JSON_MAX_NUM_OF_TAS} + "\" in \"" +
                   std::string{JSON_SERVICE_AREA_RESTRICTION} +
                   "\" can be only present if \"" +
                   std::string{JSON_RESTRICTION_TYPE} + "\" is not \"" +
                   std::string{JSON_NOT_ALLOWED_AREAS} + "\""}});
  }

  if (not change.udmStaticData.serviceAreaRestriction.restrictionType.empty()) {
    if (change.udmStaticData.serviceAreaRestriction.restrictionType !=
            JSON_NOT_ALLOWED_AREAS &&
        change.udmStaticData.serviceAreaRestriction.restrictionType !=
            JSON_ALLOWED_AREAS) {
      ret = false;
      addError("Constraint Violation",
               {{"resource_path", change.resourcePath},
                {"description", "\"" + std::string{JSON_RESTRICTION_TYPE} +
                                    "\" in \"" +
                                    std::string{JSON_SERVICE_AREA_RESTRICTION} +
                                    "\" has an Invalid Value"}});
    }
  }

  if (change.udmStaticData.fieldExistence.hasSubscribedUeAmbr) {
    boost::cmatch cm;
    if (!boost::regex_match(
            change.udmStaticData.subscribedUeAmbr.uplink.c_str(), cm,
            BIT_RATE_REGEXP)) {
      ret = false;
      addError("Constraint Violation",
               {{"resource_path", change.resourcePath},
                {"description", "\"" + std::string{JSON_UPLINK} + "\" in \"" +
                                    std::string{JSON_SUBSCRIBED_UE_AMBR} +
                                    "\" has not the valid format"}});
    }

    if (!boost::regex_match(
            change.udmStaticData.subscribedUeAmbr.downlink.c_str(),
            BIT_RATE_REGEXP)) {
      ret = false;
      addError("Constraint Violation",
               {{"resource_path", change.resourcePath},
                {"description", "\"" + std::string{JSON_DOWNLINK} + "\" in \"" +
                                    std::string{JSON_SUBSCRIBED_UE_AMBR} +
                                    "\" has not the valid format"}});
    }
  }

  if (change.udmStaticData.ratRestrictions.size() > 0) {
    for (const auto& elem : change.udmStaticData.ratRestrictions) {
      if (elem != JSON_NR && elem != JSON_EUTRA && elem != JSON_WLAN &&
          elem != JSON_VIRTUAL) {
        ret = false;
        addError("Constraint Violation",
                 {{"resource_path", change.resourcePath},
                  {"description", "\"" + std::string{JSON_RAT_RESTRICTIONS} +
                                      "\"" + " has an Invalid Value"}});
      }
    }
  }

  if (change.udmStaticData.coreNetworkTypeRestrictions.size() > 0) {
    for (const auto& elem : change.udmStaticData.coreNetworkTypeRestrictions) {
      if (elem != JSON_5GC && elem != JSON_EPC) {
        ret = false;
        addError("Constraint Violation",
                 {{"resource_path", change.resourcePath},
                  {"description",
                   "\"" + std::string{JSON_CORE_NETWORK_TYPE_RESTRICTIONS} +
                       "\"" + " has an Invalid Value"}});
      }
    }
  }

  if (change.udmStaticData.fieldExistence.hasDlPacketCount) {
    if (change.udmStaticData.dlPacketCount < -1) {
      ret = false;
      addError("Constraint Violation",
               {{"resource_path", change.resourcePath},
                {"description", "\"" + std::string{JSON_DL_PACKET_COUNT} +
                                    "\"" + " has an Invalid Value"}});
    }
  }

  if (change.udmStaticData.fieldExistence.hasStaticIpAddress) {
    if (change.udmStaticData.staticIpAddress.nssaiDnnIpLists.size()) {
      for (const auto& elem :
           change.udmStaticData.staticIpAddress.nssaiDnnIpLists) {
        if (elem.snssai.sst > 255 || elem.snssai.sst < 0) {
          ret = false;
          addError("Constraint Violation",
                   {{"resource_path", change.resourcePath},
                    {"description", "\"" + std::string{JSON_SST} + "\" in " +
                                        "\"" + JSON_STATIC_IP_ADDRESS +
                                        "\" has an Invalid Value"}});
        }
        if (!elem.snssai.sd.empty()) {
          boost::cmatch cm;
          if (!boost::regex_match(elem.snssai.sd.c_str(), cm, SD_REGEXP)) {
            ret = false;
            addError("Constraint Violation",
                     {{"resource_path", change.resourcePath},
                      {"description", "\"" + std::string{JSON_SD} + "\" in " +
                                          "\"" + JSON_STATIC_IP_ADDRESS +
                                          "\" has an Invalid Value"}});
          }
        }

        for (const auto& subElem : elem.dnnIpList) {
          // check ipAddress per DNN size
          if (subElem.ipAddress.size() < IP_ADDRESS_DNN_MIN_SIZE ||
              subElem.ipAddress.size() > IP_ADDRESS_DNN_MAX_SIZE) {
            ret = false;
            addError(
                "Constraint Violation",
                {{"resource_path", change.resourcePath},
                 {"description",
                  "\"" + std::string{JSON_IPADDRESS} + "\" in " + "\"" +
                      JSON_DNN_IP_LIST +
                      "\" has an Invalid Size. It should have at least " +
                      std::to_string(IP_ADDRESS_DNN_MIN_SIZE) +
                      " and at most " +
                      std::to_string(IP_ADDRESS_DNN_MAX_SIZE) + " elements"}});
          }
          for (const auto& ip : subElem.ipAddress) {
            if (ip.ipType == entities::IpAddrType::IPv4) {
              boost::cmatch cm;
              if (!boost::regex_match(
                      boost::get<entities::ipv4_t>(ip.ip).c_str(), cm,
                      IPV4_REGEXP)) {
                ret = false;
                addError(
                    "Constraint Violation",
                    {{"resource_path", change.resourcePath},
                     {"description",
                      "\"" + std::string{JSON_IPV4_ADDR} + "\" in " + "\"" +
                          JSON_STATIC_IP_ADDRESS + "\" has an Invalid Value"}});
              }
            } else if (ip.ipType == entities::IpAddrType::IPv6) {
              boost::cmatch cm;
              if (!boost::regex_match(
                      boost::get<entities::ipv6_t>(ip.ip).c_str(), cm,
                      IPV6_REGEXP_1)) {
                ret = false;
                addError(
                    "Constraint Violation",
                    {{"resource_path", change.resourcePath},
                     {"description",
                      "\"" + std::string{JSON_IPV6_ADDR} + "\" in " + "\"" +
                          JSON_STATIC_IP_ADDRESS + "\" has an Invalid Value"}});

              } else {
                if (!boost::regex_match(
                        boost::get<entities::ipv6_t>(ip.ip).c_str(), cm,
                        IPV6_REGEXP_2)) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", change.resourcePath},
                            {"description", "\"" + std::string{JSON_IPV6_ADDR} +
                                                "\" in " + "\"" +
                                                JSON_STATIC_IP_ADDRESS +
                                                "\" has an Invalid Value"}});
                }
              }
            } else if (ip.ipType == entities::IpAddrType::IPv6Prefix) {
              boost::cmatch cm;
              if (!boost::regex_match(
                      boost::get<entities::ipv6_prefix_t>(ip.ip).c_str(), cm,
                      IPV6_PREFIX_REGEXP_1)) {
                ret = false;
                addError(
                    "Constraint Violation",
                    {{"resource_path", change.resourcePath},
                     {"description",
                      "\"" + std::string{JSON_IPV6_PREFIX} + "\" in " + "\"" +
                          JSON_STATIC_IP_ADDRESS + "\" has an Invalid Value"}});

              } else {
                if (!boost::regex_match(
                        boost::get<entities::ipv6_prefix_t>(ip.ip).c_str(), cm,
                        IPV6_PREFIX_REGEXP_2)) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", change.resourcePath},
                            {"description",
                             "\"" + std::string{JSON_IPV6_PREFIX} + "\" in " +
                                 "\"" + JSON_STATIC_IP_ADDRESS +
                                 "\" has an Invalid Value"}});
                }
              }
            }
          }
        }
      }
    }
  }

  if (not change.udmStaticData.roamingOdb.empty()) {
    if (change.udmStaticData.roamingOdb != JSON_OUTSIDE_HOME_PLMN &&
        change.udmStaticData.roamingOdb != JSON_OUTSIDE_HOME_PLMN_COUNTRY) {
      ret = false;
      addError("Constraint Violation",
               {{"resource_path", change.resourcePath},
                {"description", "\"" + std::string{JSON_ROAMING_ODB} + "\"" +
                                    " has an Invalid Value"}});
    }
  }

  if (change.udmStaticData.fieldExistence.hasUeUsageType) {
    if (change.udmStaticData.ueUsageType < UE_USAGE_TYPE_MIN ||
        change.udmStaticData.ueUsageType > UE_USAGE_TYPE_MAX) {
      ret = false;
      addError("Constraint Violation",
               {{"resource_path", change.resourcePath},
                {"description", "\"" + std::string{JSON_UE_USAGE_TYPE} + "\"" +
                                    " has an Invalid Value"}});
    }
  }

  if (change.udmStaticData.fieldExistence.hasUdmSliceData) {
    if (change.udmStaticData.udmSliceData.plmnIdSets.size() > 0) {
      ret &= checkForCommonUdmSliceProfileRules(
          change.udmStaticData.udmSliceData.plmnIdSets, false,
          change.resourcePath);
    }

    checkForRepeatedDnnsandNssais(change.udmStaticData.udmSliceData.plmnIdSets,
                                  change.resourcePath, ret);

    /* Rule
     * When updating/creating nssai element in udmSliceData: check that all
     * dnnListIds that are pointed by nssais in udmSliceData exist either in
     * udmSliceData or in associated UdmSliceProfile*/

    /* The associated UdmSliceProfile is already received as argument of this
     * checkForCommonUdmSubscriptionRules() method*/
    std::multimap<plmn_id_set_identifier_t, dnn_list_id_t>
        dnnListIdsDefinedInSliceData;
    std::multimap<plmn_id_set_identifier_t, dnn_list_id_t>
        dnnListIdsReferencedInSliceData;
    std::multimap<plmn_id_set_identifier_t, dnn_list_id_t>
        dnnListIdsInAssocSliceProfile;

    // Get dnnListIds in udmStaticData/udmSliceData/plmnIdSets/nssai. A
    for (auto plmnIdSet : change.udmStaticData.udmSliceData.plmnIdSets) {
      for (auto defSinNssai : plmnIdSet.nssai.defaultSingleNssais) {
        if (not defSinNssai.dnnListId.empty()) {
          dnnListIdsReferencedInSliceData.insert(std::make_pair(
              plmnIdSet.plmnIdSetIdentifier, defSinNssai.dnnListId));
        }
      }

      for (auto sinNssai : plmnIdSet.nssai.singleNssais) {
        if (not sinNssai.dnnListId.empty()) {
          dnnListIdsReferencedInSliceData.insert(std::make_pair(
              plmnIdSet.plmnIdSetIdentifier, sinNssai.dnnListId));
        }
      }

      // Get dnnListIds defined in
      // udmStaticData/udmSliceData/plmnIdSets/dnnLists.  B
      for (auto dnnList : plmnIdSet.dnnLists) {
        if (not dnnList.dnnListId.empty()) {
          dnnListIdsDefinedInSliceData.insert(
              std::make_pair(plmnIdSet.plmnIdSetIdentifier, dnnList.dnnListId));
        }
      }
    }

    // Get dnnListIds in associated sliceProfile. C
    for (auto plmnIdSet : sliceProfile.plmnIdSets) {
      for (auto dnnList : plmnIdSet.dnnLists) {
        dnnListIdsInAssocSliceProfile.insert(
            std::make_pair(plmnIdSet.plmnIdSetIdentifier, dnnList.dnnListId));
      }
    }

    // Verify that all elements in set A are defined in either B or C.
    for (auto nssaiDataPair : dnnListIdsReferencedInSliceData) {
      bool nssaiDefined = false;

      for (auto sliceProfilePair =
               dnnListIdsDefinedInSliceData.find(nssaiDataPair.first);
           sliceProfilePair != dnnListIdsDefinedInSliceData.end() &&
           sliceProfilePair->first == nssaiDataPair.first && not nssaiDefined;
           sliceProfilePair++) {
        if (sliceProfilePair->second == nssaiDataPair.second) {
          nssaiDefined = true;
        }
      }

      for (auto sliceProfilePair =
               dnnListIdsInAssocSliceProfile.find(nssaiDataPair.first);
           sliceProfilePair != dnnListIdsInAssocSliceProfile.end() &&
           not nssaiDefined && sliceProfilePair->first == nssaiDataPair.first;
           sliceProfilePair++) {
        if (sliceProfilePair->second == nssaiDataPair.second) {
          nssaiDefined = true;
        }
      }

      if (not nssaiDefined) {
        ret = false;
        addError("Constraint Violation",
                 {{"resource_path", change.resourcePath},
                  {"description",
                   "\"" + std::string{JSON_DNN_LIST_ID} + "\"" + "=" + "\"" +
                       nssaiDataPair.second + "\"" + " for " + "\"" +
                       std::string{JSON_PLMN_ID_SET_IDENTIFIER} + "\"" + "=" +
                       "\"" + nssaiDataPair.first + "\"" +
                       " not found neither in the subscription nor in the " +
                       "\"" + std::string{JSON_UDM_SLICE_PROFILE_ID} + "\"" +
                       "=" + "\"" + sliceProfile.udmSliceProfileId + "\""}});
      }
    }
  }

  return ret;
}

bool ValidationData::getRelatedUdmSliceProfile(
    const entities::udm_slice_profile_id_t& profId,
    entities::udm_slice_prof_t& profile) {
  for (auto const& [key, val] : relatedResources) {
    if (isUdmSliceProfile(key)) {
      if (boost::get<entities::udm_slice_prof_t>(val).udmSliceProfileId ==
          profId) {
        profile = boost::get<entities::udm_slice_prof_t>(val);
        return true;
      }
    }
  }

  return false;
}

entities::validation_response_t
ValidationData::checkForCreationUdmSubscriptionRules(entities::Change& change) {
  bool ret = true;
  auto code = ::port::HTTP_CONFLICT;
  entities::udm_slice_prof_t sliceProfile;

  std::string mscId = getMscId(change.resourcePath);
  std::string pathToFind = buildProvJournalResourcePathFromMscId(mscId);
  if (relatedResources.contains(pathToFind)) {
    entities::ProvJournal journal =
        boost::get<entities::prov_journal_t>(relatedResources.at(pathToFind));

    // There is either an IMSI defined for the subscriber
    if (journal.imsi.empty()) {
      ret = false;
      addError("Constraint Violation",
               {{"resource_path", change.resourcePath},
                {"description",
                 "There must be an IMSI defined for the subscriber mscId=" +
                     mscId}});
    }
  } else {
    addError("Unprocessable entity",
             {{"resource_path", change.resourcePath},
              {"description",
               "provJournal for subscriber mscId=" + mscId +
                   " not included. Needed to check if IMSI is defined: " +
                   pathToFind}});
    return {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
  }

  if (not checkRelatedUdmSliceProfile(
          change.udmStaticData.udmSliceProfileId.id, sliceProfile,
          getMscId(change.resourcePath), change.resourcePath)) {
    return {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
  }

  ret &= checkForCommonUdmSubscriptionRules(change, sliceProfile);

  if (ret) {
    code = ::port::HTTP_OK;
  }

  return {ret, code};
}

entities::validation_response_t
ValidationData::checkForUpdateUdmSubscriptionRules(entities::Change& change) {
  bool ret = true;
  auto code = ::port::HTTP_CONFLICT;
  auto path = change.resourcePath;
  entities::udm_slice_prof_t sliceProfile;

  if (not hasRelatedResource(path)) {
    addError(
        "Unprocessable entity",
        {{"resource_path", path},
         {"description", "There is no associated relatedResource: " + path}});
    return {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
  }

  if (not checkRelatedUdmSliceProfile(
          change.udmStaticData.udmSliceProfileId.id, sliceProfile,
          getMscId(change.resourcePath), change.resourcePath)) {
    return {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
  }

  ret &= checkForCommonUdmSubscriptionRules(change, sliceProfile);

  if (ret) {
    code = ::port::HTTP_OK;
  }

  return {ret, code};
}

bool ValidationData::checkRelatedUdmSliceProfile(
    const entities::udm_slice_profile_id_t& profId,
    entities::udm_slice_prof_t& profile, const std::string& mscId,
    const entities::resource_path_t& resourcePath) {
  if (not getRelatedUdmSliceProfile(profId, profile)) {
    addError("Unprocessable entity",
             {{"resource_path", resourcePath},
              {"description", "UdmSliceProfile with id=" + profId +
                                  " for subscriber mscId=" + mscId +
                                  " not included as relatedResource."}});
    return false;
  }

  return true;
}

entities::validation_response_t
ValidationData::checkForCreationUdmSliceProfileRules(entities::Change& change) {
  bool ret = true;
  if (change.udmSliceProfile.plmnIdSets.size() > 0) {
    ret &= checkForCommonUdmSliceProfileRules(change.udmSliceProfile.plmnIdSets,
                                              true, change.resourcePath);
  }

  ret &= checkForCreationSpecificUdmSliceProfileRules(
      change.udmSliceProfile.plmnIdSets, change.resourcePath);

  if (ret) {
    return {ret, ::port::HTTP_OK};
  }
  return {ret, ::port::HTTP_CONFLICT};
}

entities::validation_response_t
ValidationData::checkForUpdateUdmSliceProfileRules(entities::Change& change) {
  bool ret = true;
  auto code = ::port::HTTP_CONFLICT;
  auto path = change.resourcePath;

  if (not hasRelatedResource(path)) {
    addError(
        "Unprocessable entity",
        {{"resource_path", path},
         {"description", "There is no associated relatedResource: " + path}});
    return {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
  }

  if (change.udmSliceProfile.plmnIdSets.size() > 0) {
    ret &= checkForCommonUdmSliceProfileRules(change.udmSliceProfile.plmnIdSets,
                                              true, change.resourcePath);
  }

  auto udmSliceProfileRelResource =
      boost::get<entities::udm_slice_prof_t>(relatedResources.at(path));
  ret &= checkForUpdateSpecificUdmSliceProfileRules(change,
                                                    udmSliceProfileRelResource);

  if (ret) {
    code = ::port::HTTP_OK;
  }

  return {ret, code};
}

bool ValidationData::checkForCommonUdmSliceProfileRules(
    entities::plmn_id_sets_t& plmnIdSets, const bool& isSliceProfile,
    const entities::resource_path_t& resourcePath) {
  bool ret = true;
  bool plmnIdSetIdDefaultExists = false;
  for (const auto& elem : plmnIdSets) {
    std::vector<std::string> dnnVector;
    std::vector<std::string> dnnListIdVector;
    std::vector<entities::Snssai> nssaiList;

    if (not elem.plmnIdSetIdentifier.empty()) {
      boost::cmatch cm;
      if (!boost::regex_match(elem.plmnIdSetIdentifier.c_str(), cm,
                              PLMNID_SET_IDENTIFIER_REGEXP)) {
        ret = false;
        addError(
            "Constraint Violation",
            {{"resource_path", resourcePath},
             {"description", "\"" + std::string{JSON_PLMN_ID_SET_IDENTIFIER} +
                                 "\" has not the valid format"}});
      }
      if (elem.plmnIdSetIdentifier == DEFAULT_VALUE) {
        plmnIdSetIdDefaultExists = true;
      }
    }

    if (elem.nssai.defaultSingleNssais.size() > 0) {
      for (const auto& subElem : elem.nssai.defaultSingleNssais) {
        if (not subElem.dnnListId.empty()) {
          dnnVector.push_back(subElem.dnnListId);
        }

        if (subElem.nssai.sst > 255 || subElem.nssai.sst < 0) {
          ret = false;
          addError(
              "Constraint Violation",
              {{"resource_path", resourcePath},
               {"description", "\"" + std::string{JSON_SST} + "\" in " + "\"" +
                                   std::string{JSON_DEFAULT_SINGLE_NSSAIS} +
                                   "\" has an Invalid Value"}});
        }
        if (!subElem.nssai.sd.empty()) {
          boost::cmatch cm;
          if (!boost::regex_match(subElem.nssai.sd.c_str(), cm, SD_REGEXP)) {
            ret = false;
            addError(
                "Constraint Violation",
                {{"resource_path", resourcePath},
                 {"description", "\"" + std::string{JSON_SD} + "\" in " + "\"" +
                                     std::string{JSON_DEFAULT_SINGLE_NSSAIS} +
                                     "\" has an Invalid Value"}});
          }
        }
        if (!subElem.threegppChargingCharacteristics.empty()) {
          if ((!regex_match(subElem.threegppChargingCharacteristics,
                            THREEGPP_CHARGING_CHARACTERISTICS_REGEX)) ||
              subElem.threegppChargingCharacteristics.size() != 4) {
            ret = false;
            addError("Constraint Violation",
                     {{"resource_path", resourcePath},
                      {"description",
                       "\"" + std::string{JSON_3GPP_CHARGING_CHARACTERISTICS} +
                           "\"" + " in " + "\"" +
                           std::string{JSON_DEFAULT_SINGLE_NSSAIS} +
                           "\""
                           " has not the valid format"}});
          }
        }
      }
    }

    if (elem.nssai.singleNssais.size() > 0) {
      for (const auto& subElem : elem.nssai.singleNssais) {
        if (not subElem.dnnListId.empty()) {
          dnnVector.push_back(subElem.dnnListId);
        }

        if (subElem.nssai.sst > 255 || subElem.nssai.sst < 0) {
          ret = false;
          addError("Constraint Violation",
                   {{"resource_path", resourcePath},
                    {"description", "\"" + std::string{JSON_SST} + "\" in " +
                                        "\"" + JSON_SINGLE_NSSAIS +
                                        "\" has an Invalid Value"}});
        }
        if (!subElem.nssai.sd.empty()) {
          boost::cmatch cm;
          if (!boost::regex_match(subElem.nssai.sd.c_str(), cm, SD_REGEXP)) {
            ret = false;
            addError("Constraint Violation",
                     {{"resource_path", resourcePath},
                      {"description", "\"" + std::string{JSON_SD} + "\" in " +
                                          "\"" + JSON_SINGLE_NSSAIS +
                                          "\" has an Invalid Value"}});
          }
        }
        if (!subElem.threegppChargingCharacteristics.empty()) {
          if ((!regex_match(subElem.threegppChargingCharacteristics,
                            THREEGPP_CHARGING_CHARACTERISTICS_REGEX)) ||
              subElem.threegppChargingCharacteristics.size() != 4) {
            ret = false;
            addError(
                "Constraint Violation",
                {{"resource_path", resourcePath},
                 {"description",
                  "\"" + std::string{JSON_3GPP_CHARGING_CHARACTERISTICS} +
                      "\"" + " in " + "\"" + std::string{JSON_SINGLE_NSSAIS} +
                      "\""
                      " has not the valid format"}});
          }
        }
      }
    }

    if (elem.dnnLists.size() > 0) {
      bool dnnListIdDefaultExists = false;

      for (const auto& subElem : elem.dnnLists) {
        if (subElem.dnnListId == DEFAULT_VALUE) {
          dnnListIdDefaultExists = true;
        }
        if (not subElem.dnnListId.empty()) {
          dnnListIdVector.push_back(subElem.dnnListId);
        }
        bool dnnDefaulExists = false;
        if (subElem.dnnDataList.size() > 0) {
          std::vector<std::string> dnnRepeated;
          for (const auto& myElem : subElem.dnnDataList) {
            // myElem is a DnnData
            if (myElem.fieldExistence.hasDefaultDnnIndicator &&
                myElem.defaultDnnIndicator) {
              dnnDefaulExists = true;
              if (myElem.dnn == "*") {
                ret = false;
                addError("Constraint Violation",
                         {{"resource_path", resourcePath},
                          {"description",
                           "\"" + std::string{JSON_DEFAULT_DNN_INDICATOR} +
                               "\" in \"" + std::string{JSON_DNN_DATA_LIST} +
                               "\" cannot be set to true when \"" +
                               std::string{JSON_DNN} + "\" is \"*\""}});
              }
            }

            if (myElem.fieldExistence.hasSscModes) {
              std::vector<std::string> sscModesRepeated;
              if (myElem.sscModes.defaultSscMode.empty()) {
                ret = false;
                addError("Constraint Violation",
                         {{"resource_path", resourcePath},
                          {"description",
                           "\"" + std::string{JSON_DEFAULT_SSC_MODE} +
                               "\" in " + "\"" + std::string{JSON_SSC_MODES} +
                               "\" is a required Field"}});
              } else {
                if (myElem.sscModes.defaultSscMode != JSON_SSC_MODE_1 &&
                    myElem.sscModes.defaultSscMode != JSON_SSC_MODE_2 &&
                    myElem.sscModes.defaultSscMode != JSON_SSC_MODE_3) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description",
                             "\"" + std::string{JSON_DEFAULT_SSC_MODE} +
                                 "\" in " + "\"" + std::string{JSON_SSC_MODES} +
                                 "\" has an Invalid Value"}});
                } else {
                  checkSscRepeated(resourcePath, sscModesRepeated,
                                   myElem.sscModes.defaultSscMode, ret);
                }
              }

              if (myElem.sscModes.allowedSscModes.size() > 0) {
                if (myElem.sscModes.allowedSscModes.size() > 2) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description",
                             "\"" + std::string{JSON_ALLOWED_SSC_MODES} +
                                 "\" in " + "\"" + std::string{JSON_SSC_MODES} +
                                 "\" has an Invalid Size"}});
                } else {
                  for (const auto& theElem : myElem.sscModes.allowedSscModes) {
                    if (theElem != JSON_SSC_MODE_1 &&
                        theElem != JSON_SSC_MODE_2 &&
                        theElem != JSON_SSC_MODE_3) {
                      ret = false;
                      addError(
                          "Constraint Violation",
                          {{"resource_path", resourcePath},
                           {"description",
                            "\"" + std::string{JSON_ALLOWED_SSC_MODES} +
                                "\" in " + "\"" + std::string{JSON_SSC_MODES} +
                                "\" has an Invalid Value"}});
                    } else {
                      checkSscRepeated(resourcePath, sscModesRepeated, theElem,
                                       ret);
                    }
                  }
                }
              }
            }

            if (myElem.fieldExistence.hasPduSessionTypes) {
              std::vector<std::string> sessionRepeated;
              if (myElem.pduSessionTypes.defaultSessionType.empty()) {
                ret = false;
                addError(
                    "Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description",
                      "\"" + std::string{JSON_DEFAULT_SESSION_TYPE} + "\" in " +
                          "\"" + std::string{JSON_PDU_SESSION_TYPES} +
                          "\" is a required Field"}});
              } else {
                if (myElem.pduSessionTypes.defaultSessionType != JSON_IPV4 &&
                    myElem.pduSessionTypes.defaultSessionType != JSON_IPV6 &&
                    myElem.pduSessionTypes.defaultSessionType != JSON_IPV4_V6 &&
                    myElem.pduSessionTypes.defaultSessionType !=
                        JSON_UNSTRUCTURED &&
                    myElem.pduSessionTypes.defaultSessionType !=
                        JSON_ETHERNET) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description",
                             "\"" + std::string{JSON_DEFAULT_SESSION_TYPE} +
                                 "\" in " + "\"" +
                                 std::string{JSON_PDU_SESSION_TYPES} +
                                 "\" has an Invalid Value"}});
                } else {
                  checkPduSessionTypeRepeated(
                      resourcePath, sessionRepeated,
                      myElem.pduSessionTypes.defaultSessionType, ret);
                }
              }
              if (myElem.fieldExistence.hasAllowedSessionTypes) {
                if (myElem.pduSessionTypes.allowedSessionTypes.size() < 1) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description",
                             "\"" + std::string{JSON_ALLOWED_SESSION_TYPES} +
                                 "\" in " + "\"" +
                                 std::string{JSON_PDU_SESSION_TYPES} +
                                 "\" has an Invalid Size"}});
                } else {
                  for (const auto& theElem :
                       myElem.pduSessionTypes.allowedSessionTypes) {
                    if (theElem != JSON_IPV4 && theElem != JSON_IPV6 &&
                        theElem != JSON_IPV4_V6 &&
                        theElem != JSON_UNSTRUCTURED &&
                        theElem != JSON_ETHERNET) {
                      ret = false;
                      addError(
                          "Constraint Violation",
                          {{"resource_path", resourcePath},
                           {"description",
                            "\"" + std::string{JSON_ALLOWED_SESSION_TYPES} +
                                "\" in " + "\"" +
                                std::string{JSON_PDU_SESSION_TYPES} +
                                "\" has an Invalid Value"}});
                    } else {
                      checkPduSessionTypeRepeated(resourcePath, sessionRepeated,
                                                  theElem, ret);
                    }
                  }
                }
              }
            }

            if (myElem.fieldExistence.hasFivegQosProfile) {
              if (myElem.fivegQosProfile.fiveqi < 0 ||
                  myElem.fivegQosProfile.fiveqi > 255) {
                ret = false;
                addError("Constraint Violation",
                         {{"resource_path", resourcePath},
                          {"description", "\"" + std::string{JSON_5QI} +
                                              "\" in " + "\"" +
                                              std::string{JSON_5G_QOS_PROFILE} +
                                              "\" has an Invalid Value"}});
              }

              if (myElem.fivegQosProfile.fieldExistence.hasPriorityLevel) {
                if (myElem.fivegQosProfile.priorityLevel < 1 ||
                    myElem.fivegQosProfile.priorityLevel > 127) {
                  ret = false;
                  addError(
                      "Constraint Violation",
                      {{"resource_path", resourcePath},
                       {"description", "\"" + std::string{JSON_PRIORITY_LEVEL} +
                                           "\" in " + "\"" +
                                           std::string{JSON_5G_QOS_PROFILE} +
                                           "\" has an Invalid Value"}});
                }
              }

              if (myElem.fivegQosProfile.arp.priorityLevel < 1 ||
                  myElem.fivegQosProfile.arp.priorityLevel > 15) {
                ret = false;
                addError("Constraint Violation",
                         {{"resource_path", resourcePath},
                          {"description",
                           "\"" + std::string{JSON_PRIORITY_LEVEL} + "\" in " +
                               "\"" + std::string{JSON_ARP} +
                               "\" has an Invalid Value"}});
              }

              if (myElem.fivegQosProfile.arp.preemptCap != JSON_NOT_PREEMPT &&
                  myElem.fivegQosProfile.arp.preemptCap != JSON_MAY_PREEMPT) {
                ret = false;
                addError(
                    "Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description",
                      "\"" + std::string{JSON_PREEMPTCAP} + "\" in " + "\"" +
                          std::string{JSON_ARP} + "\" has an Invalid Value"}});
              }

              if (myElem.fivegQosProfile.arp.preemptVuln !=
                      JSON_NOT_PREEMPTABLE &&
                  myElem.fivegQosProfile.arp.preemptVuln != JSON_PREEMPTABLE) {
                ret = false;
                addError(
                    "Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description",
                      "\"" + std::string{JSON_PREEMPTVULN} + "\" in " + "\"" +
                          std::string{JSON_ARP} + "\" has an Invalid Value"}});
              }
            }

            if (myElem.fieldExistence.hasSessionAmbr) {
              if (not myElem.sessionAmbr.uplink.empty()) {
                boost::cmatch cm;
                if (!boost::regex_match(myElem.sessionAmbr.uplink.c_str(), cm,
                                        BIT_RATE_REGEXP)) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description",
                             "\"" + std::string{JSON_UPLINK} + "\" in \"" +
                                 std::string{JSON_SESSION_AMBR} +
                                 "\" has not the valid format"}});
                }
              }

              if (not myElem.sessionAmbr.downlink.empty()) {
                boost::cmatch cm;
                if (!boost::regex_match(myElem.sessionAmbr.downlink.c_str(), cm,
                                        BIT_RATE_REGEXP)) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description",
                             "\"" + std::string{JSON_DOWNLINK} + "\" in \"" +
                                 std::string{JSON_SESSION_AMBR} +
                                 "\" has not the valid format"}});
                }
              }
            }

            if (myElem.fieldExistence.hasUpSecurity) {
              if (not myElem.upSecurity.upIntegr.empty()) {
                if (myElem.upSecurity.upIntegr != JSON_REQUIRED &&
                    myElem.upSecurity.upIntegr != JSON_PREFERRED &&
                    myElem.upSecurity.upIntegr != JSON_NOT_NEEDED) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description",
                             "\"" + std::string{JSON_UP_INTEGR} + "\" in \"" +
                                 std::string{JSON_UP_SECURITY} +
                                 "\" has not the valid format"}});
                }
              }

              if (not myElem.upSecurity.upConfid.empty()) {
                if (myElem.upSecurity.upConfid != JSON_REQUIRED &&
                    myElem.upSecurity.upConfid != JSON_PREFERRED &&
                    myElem.upSecurity.upConfid != JSON_NOT_NEEDED) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description",
                             "\"" + std::string{JSON_UP_CONFID} + "\" in \"" +
                                 std::string{JSON_UP_SECURITY} +
                                 "\" has not the valid format"}});
                }
              }
            }

            if (myElem.fieldExistence.hasSmfList) {
              if (myElem.smfList.size() < 1) {
                ret = false;
                addError("Constraint Violation",
                         {{"resource_path", resourcePath},
                          {"description", "At least one element in \"" +
                                              std::string{JSON_SMF_LIST} +
                                              "\" must exist"}});
              }
              for (auto smfListElem : myElem.smfList) {
                boost::cmatch cm;
                if (!boost::regex_match(smfListElem.c_str(), cm, UUID_REGEX)) {
                  ret = false;
                  addError("Constraint Violation",
                           {{"resource_path", resourcePath},
                            {"description", "Element in \"" +
                                                std::string{JSON_SMF_LIST} +
                                                "\""
                                                " has an invalid format"}});
                }
              }
            }

            /* Constraint: 3GppChargingCharacteristics must be an hexadecimal
             * value of 4 symbols. */
            if (not myElem.threegppChargingCharacteristics.empty()) {
              if ((not regex_match(myElem.threegppChargingCharacteristics,
                                   THREEGPP_CHARGING_CHARACTERISTICS_REGEX)) ||
                  myElem.threegppChargingCharacteristics.size() != 4) {
                ret = false;
                addError(
                    "Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description",
                      "\"" + std::string{JSON_3GPP_CHARGING_CHARACTERISTICS} +
                          "\"" + " in " + "\"" +
                          std::string{JSON_DNN_DATA_LIST} +
                          "\""
                          " has not the valid format"}});
              }
            }
          }
        }
        if (dnnDefaulExists == false) {
          ret = false;
          addError("Constraint Violation",
                   {{"resource_path", resourcePath},
                    {"description",
                     "At least a \"" + std::string{JSON_DEFAULT_DNN_INDICATOR} +
                         "\" with \"true\" value must exist"}});
        }
      }
      if (dnnListIdDefaultExists == false and isSliceProfile) {
        ret = false;
        addError(
            "Constraint Violation",
            {{"resource_path", resourcePath},
             {"description", "At least a \"" + std::string{JSON_DNN_LIST_ID} +
                                 "\" with \"Default\" value must exist"}});
      }
    }
    // Check that DnnListId in nssaiDetails point to Dnnlists
    if (isSliceProfile) {
      for (int i = 0; i < static_cast<int>(dnnVector.size()); i++) {
        bool exists = std::find(dnnListIdVector.begin(), dnnListIdVector.end(),
                                dnnVector[i]) != dnnListIdVector.end();
        if (!exists) {
          ret = false;
          addError("Constraint Violation",
                   {{"resource_path", resourcePath},
                    {"description",
                     "\"" + std::string{JSON_DNN_LIST_ID} + "\" in " +
                         "\"nssaiDetails\" must point to existing \"" +
                         std::string{JSON_DNN_LISTS} + "\""}});
        }
      }
    }
  }
  if (plmnIdSetIdDefaultExists == false and isSliceProfile) {
    ret = false;
    addError("Constraint Violation",
             {{"resource_path", resourcePath},
              {"description", "At least a \"" +
                                  std::string{JSON_PLMN_ID_SET_IDENTIFIER} +
                                  "\" with \"Default\" value must exist"}});
  }
  return ret;
}

bool ValidationData::checkForCreationSpecificUdmSliceProfileRules(
    entities::plmn_id_sets_t& plmnIdSets,
    const entities::resource_path_t& resourcePath) {
  bool ret = true;

  checkForRepeatedDnnsandNssais(plmnIdSets, resourcePath, ret);

  return ret;
}

bool ValidationData::checkForUpdateSpecificUdmSliceProfileRules(
    entities::Change& change, entities::udm_slice_prof_t& relResource) {
  bool ret = true;

  auto dnnsRelResource = getAllDnnsUdmSliceProfile(relResource);
  auto nssaiRelResource = getAllNssaisUdmSliceProfile(relResource);
  std::multimap<plmn_id_set_identifier_t, dnn_list_id_t> dnnListIdsOnChange;

  for (const auto& elem : change.udmSliceProfile.plmnIdSets) {
    std::vector<entities::Snssai> nssaiList;

    for (const auto& subElem : elem.nssai.defaultSingleNssais) {
      // Check that nssai attributes are not repeated
      checkForRepeatedNssaisOnUpdate(change, nssaiList, nssaiRelResource,
                                     elem.plmnIdSetIdentifier, DEFAULT_KEY,
                                     subElem.nssai.sst, subElem.nssai.sd,
                                     subElem.dnnListId, ret);
    }

    for (const auto& subElem : elem.nssai.singleNssais) {
      // Check that nssai attributes are not repeated
      checkForRepeatedNssaisOnUpdate(change, nssaiList, nssaiRelResource,
                                     elem.plmnIdSetIdentifier, SINGLE_KEY,
                                     subElem.nssai.sst, subElem.nssai.sd,
                                     subElem.dnnListId, ret);
    }

    for (const auto& subElem : elem.dnnLists) {
      dnnListIdsOnChange.insert(
          std::make_pair(elem.plmnIdSetIdentifier, subElem.dnnListId));
      std::vector<std::string> dnnUpdated;
      dnnUpdated.clear();
      for (const auto& myElem : subElem.dnnDataList) {
        if (!myElem.dnn.empty()) {
          // There are no duplicated DNNs in any of the DnnLists
          auto lowDnn = boost::algorithm::to_lower_copy(myElem.dnn);
          int isRepeated =
              std::count(dnnUpdated.begin(), dnnUpdated.end(), lowDnn);
          if (isRepeated >= 1) {
            auto duplicatedInRelResource = dnnsRelResource[std::make_pair(
                elem.plmnIdSetIdentifier, subElem.dnnListId)];
            int countRep = std::count(duplicatedInRelResource.begin(),
                                      duplicatedInRelResource.end(), lowDnn);
            if (countRep > isRepeated) {
              // It is also duplicated in database. In this case, return
              // OK and log the issue
              LOG_INFO("Inconsistent data in database", "resource_path",
                       change.resourcePath, "plmnIdSetIdentifier",
                       elem.plmnIdSetIdentifier, "dnnListId", subElem.dnnListId,
                       "dnn", myElem.dnn);
            } else {
              // Not duplicated in database. Return error
              ret = false;
              addError(
                  "Constraint Violation",
                  {{"resource_path", change.resourcePath},
                   {"description", "\"" + std::string{JSON_DNN} +
                                       "\" is duplicated in \"" +
                                       std::string{JSON_DNN_LISTS} + "\""}});
            }
          }
          dnnUpdated.push_back(lowDnn);
        }
      }
    }
  }

  /* Rule:
   * Do not allow deleting a dnnListId from UdmSliceProfile. Check the
   * dnnListIds in change and the dnnListIds in relatedResource and
   * check none has been removed
   *
   */

  for (const auto& plmnIdSet : relResource.plmnIdSets) {
    for (const auto& dnnList : plmnIdSet.dnnLists) {
      bool dnnListIdDefined = false;
      for (auto it = dnnListIdsOnChange.find(plmnIdSet.plmnIdSetIdentifier);
           it != dnnListIdsOnChange.end(); it++) {
        if (it->second == dnnList.dnnListId) {
          dnnListIdDefined = true;
          break;
        }
      }

      if (not dnnListIdDefined) {
        ret = false;
        addError("Constraint Violation",
                 {{"resource_path", change.resourcePath},
                  {"description",
                   "\"" + std::string{JSON_DNN_LIST_ID} + "\"" + "=" + "\"" +
                       dnnList.dnnListId + "\"" + " cannot be deleted for " +
                       "\"" + std::string{JSON_PLMN_ID_SET_IDENTIFIER} + "\"" +
                       "=" + "\"" + plmnIdSet.plmnIdSetIdentifier + "\""}});
      }
    }
  }

  return ret;
}

std::map<entities::pair_t, std::vector<entities::dnn_t>>
ValidationData::getAllDnnsUdmSliceProfile(entities::udm_slice_prof_t& sProf) {
  std::vector<entities::dnn_t> dnns;
  std::map<entities::pair_t, std::vector<entities::dnn_t>> theMap;

  for (const auto& elem : sProf.plmnIdSets) {
    for (const auto& subElem : elem.dnnLists) {
      dnns.clear();
      for (const auto& myElem : subElem.dnnDataList) {
        if (!myElem.dnn.empty()) {
          dnns.push_back(boost::algorithm::to_lower_copy(myElem.dnn));
        }
      }
      theMap.insert({{elem.plmnIdSetIdentifier, subElem.dnnListId}, dnns});
    }
  }

  return theMap;
}

std::map<entities::pair_nssai_key_t, std::vector<entities::tuple_nssai_t>>
ValidationData::getAllNssaisUdmSliceProfile(entities::udm_slice_prof_t& sProf) {
  std::vector<entities::tuple_nssai_t> nssais;
  std::map<entities::pair_nssai_key_t, std::vector<entities::tuple_nssai_t>>
      theMap;

  for (const auto& elem : sProf.plmnIdSets) {
    nssais = getNssais(elem.nssai.defaultSingleNssais);
    theMap.insert({{elem.plmnIdSetIdentifier, DEFAULT_KEY}, nssais});

    nssais = getNssais(elem.nssai.singleNssais);
    theMap.insert({{elem.plmnIdSetIdentifier, SINGLE_KEY}, nssais});
  }

  return theMap;
}

std::vector<entities::tuple_nssai_t> ValidationData::getNssais(
    const entities::single_nssais_t& singleNssais) {
  std::vector<entities::tuple_nssai_t> nssais;

  for (const auto& subElem : singleNssais) {
    entities::Snssai theNssai;
    theNssai.sst = subElem.nssai.sst;
    theNssai.sd = boost::algorithm::to_lower_copy(subElem.nssai.sd);
    nssais.push_back(
        std::make_tuple(theNssai.sst, theNssai.sd, subElem.dnnListId));
  }

  return nssais;
}

entities::validation_response_t
ValidationData::checkForCreationUdmRoamingAreaRules(entities::Change& change) {
  bool ret = true;
  ret &= checkForCommonUdmRoamingAreaRules(change);

  if (ret) {
    return {ret, ::port::HTTP_OK};
  }
  return {ret, ::port::HTTP_CONFLICT};
}

entities::validation_response_t
ValidationData::checkForUpdateUdmRoamingAreaRules(entities::Change& change) {
  bool ret = true;
  auto code = ::port::HTTP_CONFLICT;
  auto path = change.resourcePath;

  if (not hasRelatedResource(path)) {
    addError(
        "Unprocessable entity",
        {{"resource_path", path},
         {"description", "There is no associated relatedResource: " + path}});
    return {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
  }

  ret &= checkForCommonUdmRoamingAreaRules(change);

  if (ret) {
    code = ::port::HTTP_OK;
  }

  return {ret, code};
}

bool ValidationData::checkForCommonUdmRoamingAreaRules(
    entities::Change& change) {
  bool ret = true;

  if (change.udmRoamingArea.areaList.size() < 1) {
    ret = false;
    addError("Constraint Violation",
             {{"resource_path", change.resourcePath},
              {"description", "At least one \"" + std::string{JSON_AREA_LIST} +
                                  "\" must exist"}});
  }

  for (const auto& elem : change.udmRoamingArea.areaList) {
    if (elem.area.type() == typeid(entities::tacs_t)) {
      entities::tacs_t tacs = boost::get<entities::tacs_t>(elem.area);
      if (tacs.size() < 1) {
        ret = false;
        addError(
            "Constraint Violation",
            {{"resource_path", change.resourcePath},
             {"description", "At least one element in \"" +
                                 std::string{JSON_TACS} + "\" must exist"}});
      }
      for (const auto& tac : tacs) {
        if (not isTac(tac)) {
          ret = false;
          addError("Constraint Violation",
                   {{"resource_path", change.resourcePath},
                    {"description", "\"" + std::string{JSON_TACS} + "\" in \"" +
                                        std::string{JSON_AREA_LIST} +
                                        "\" has an invalid format"}});
        }
      }
    }
  }

  return ret;
}

void ValidationData::addError(
    std::string message,
    std::initializer_list<std::pair<std::string, std::string>> args) {
  entities::Error err;
  err.errorMessage = message;
  for (auto& i : args) {
    err.errorDetails.insert(i);
  }
  response.errors.push_back(err);
}

void ValidationData::checkForRepeatedDnnsandNssais(
    entities::plmn_id_sets_t& plmnIdSets,
    const entities::resource_path_t& resourcePath, bool& ret) {
  for (const auto& elem : plmnIdSets) {
    std::vector<entities::Snssai> nssaiList;

    for (const auto& subElem : elem.nssai.defaultSingleNssais) {
      // Check that nssai attributes are not repeated
      checkForRepeatedNssais(resourcePath, nssaiList, subElem.nssai.sst,
                             subElem.nssai.sd, ret);
    }

    for (const auto& subElem : elem.nssai.singleNssais) {
      // Check that nssai attributes are not repeated
      checkForRepeatedNssais(resourcePath, nssaiList, subElem.nssai.sst,
                             subElem.nssai.sd, ret);
    }

    for (const auto& subElem : elem.dnnLists) {
      std::vector<std::string> dnnRepeated;
      dnnRepeated.clear();
      for (const auto& myElem : subElem.dnnDataList) {
        if (!myElem.dnn.empty()) {
          // There are no duplicated DNNs in any of the DnnLists
          auto lowDnn = boost::algorithm::to_lower_copy(myElem.dnn);
          int isRepeated =
              std::count(dnnRepeated.begin(), dnnRepeated.end(), lowDnn);
          if (isRepeated >= 1) {
            ret = false;
            addError("Constraint Violation",
                     {{"resource_path", resourcePath},
                      {"description", "\"" + std::string{JSON_DNN} +
                                          "\" is duplicated in \"" +
                                          std::string{JSON_DNN_LISTS} + "\""}});
          } else {
            dnnRepeated.push_back(lowDnn);
          }
        }
      }
    }
  }
}

void ValidationData::checkForRepeatedNssais(
    const entities::resource_path_t& resourcePath,
    std::vector<entities::Snssai>& nssaiList, int sst, std::string sd,
    bool& ret) {
  entities::Snssai theNssai;
  theNssai.sst = sst;
  theNssai.sd = boost::algorithm::to_lower_copy(sd);
  int numNssai = std::count_if(
      nssaiList.begin(), nssaiList.end(), [&theNssai](const auto& nssai) {
        return (theNssai.sst == nssai.sst &&
                theNssai.sd == boost::algorithm::to_lower_copy(nssai.sd));
      });
  if (numNssai >= 1) {
    ret = false;
    addError("Constraint Violation",
             {{"resource_path", resourcePath},
              {"description", "There are \"" + std::string{JSON_NSSAI} +
                                  "\" repeated in " + "the \"" +
                                  std::string{JSON_NSSAI} + "\" attribute"}});
  } else {
    nssaiList.push_back(theNssai);
  }
}

void ValidationData::checkForRepeatedNssaisOnUpdate(
    entities::Change& change, std::vector<entities::Snssai>& nssaiList,
    std::map<entities::pair_nssai_key_t, std::vector<entities::tuple_nssai_t>>
        nssaisListRelResource,
    const entities::plmn_id_set_identifier_t& plmnId, const std::string& key,
    int sst, std::string sd, const entities::dnn_list_id_t& dnnListId,
    bool& ret) {
  entities::Snssai theNssai;
  theNssai.sst = sst;
  theNssai.sd = boost::algorithm::to_lower_copy(sd);
  int numNssai = std::count_if(
      nssaiList.begin(), nssaiList.end(), [&theNssai](const auto& nssai) {
        return (theNssai.sst == nssai.sst &&
                theNssai.sd == boost::algorithm::to_lower_copy(nssai.sd));
      });
  if (numNssai >= 1) {
    auto duplicatedInRelResource =
        nssaisListRelResource[std::make_pair(plmnId, key)];
    auto found = std::find_if(
        duplicatedInRelResource.begin(), duplicatedInRelResource.end(),
        [&theNssai, &dnnListId](const entities::tuple_nssai_t& nssai) {
          return (theNssai.sst == std::get<0>(nssai) &&
                  theNssai.sd == std::get<1>(nssai) &&
                  dnnListId.compare(std::get<2>(nssai)) == 0);
        });
    if (found != duplicatedInRelResource.end()) {
      // It is also duplicated in database. Return OK and log the issue
      LOG_INFO("Inconsistent data in database", "resource_path",
               change.resourcePath, "plmnIdSetIdentifier", plmnId, "nssai",
               key == DEFAULT_KEY ? "defaultSingleNssais" : "singleNssais",
               "sst", sst, "sd", sd);
    } else {
      // Not duplicated in database. Return error
      ret = false;
      addError("Constraint Violation",
               {{"resource_path", change.resourcePath},
                {"description", "There are \"" + std::string{JSON_NSSAI} +
                                    "\" repeated in " + "the \"" +
                                    std::string{JSON_NSSAI} + "\" attribute"}});
    }
  } else {
    nssaiList.push_back(theNssai);
  }
}

void ValidationData::checkSscRepeated(
    const entities::resource_path_t& resourcePath,
    std::vector<std::string>& sscList, std::string elem, bool& ret) {
  int sscRepeated = std::count(sscList.begin(), sscList.end(), elem);
  if (sscRepeated >= 1) {
    ret = false;
    addError(
        "Constraint Violation",
        {{"resource_path", resourcePath},
         {"description", "There are duplicated SSC modes between in \"" +
                             std::string{JSON_DEFAULT_SSC_MODE} + "\" and \"" +
                             std::string{JSON_ALLOWED_SSC_MODES} + "\""}});
  } else {
    sscList.push_back(elem);
  }
}

void ValidationData::checkPduSessionTypeRepeated(
    const entities::resource_path_t& resourcePath,
    std::vector<std::string>& sessionList, std::string elem, bool& ret) {
  int sessionRepeated =
      std::count(sessionList.begin(), sessionList.end(), elem);
  if (sessionRepeated >= 1) {
    ret = false;
    addError("Constraint Violation",
             {{"resource_path", resourcePath},
              {"description",
               "There are duplicated PDU session types between in \"" +
                   std::string{JSON_DEFAULT_SESSION_TYPE} + "\" and \"" +
                   std::string{JSON_ALLOWED_SESSION_TYPES} + "\""}});
  } else {
    sessionList.push_back(elem);
  }
}

bool ValidationData::hasRelatedResource(entities::resource_path_t& path) {
  if (relatedResources.contains(path)) {
    return true;
  }
  return false;
}

}  // namespace entities
