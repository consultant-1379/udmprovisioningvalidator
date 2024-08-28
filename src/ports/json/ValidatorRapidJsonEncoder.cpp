#include "ValidatorRapidJsonEncoder.hpp"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "JsonConstants.hpp"
#include "ValidatorVendorSpecificEncoder.hpp"

namespace port {
namespace secondary {
namespace json {

ValidatorRapidJsonEncoder::ValidatorRapidJsonEncoder()
    : rJsonDoc{}, prettyFormat{false} {}

std::ostringstream ValidatorRapidJsonEncoder::validatorResponseToJson(
    const ::entities::ValidationData& data) {
  rJsonDoc.SetObject();
  rapidjson::Document::AllocatorType& allocator = rJsonDoc.GetAllocator();

  // Errors
  if (data.response.errors.size()) {
    rapidjson::Value errors(rapidjson::kArrayType);

    for (const auto& elem : data.response.errors) {
      rapidjson::Value error(rapidjson::kObjectType);
      rapidjson::Value keyErrorMessage(JSON_ERROR_MESSAGE, allocator);
      rapidjson::Value valueErrorMessage(elem.errorMessage.c_str(), allocator);
      error.AddMember(keyErrorMessage, valueErrorMessage, allocator);

      rapidjson::Value details(rapidjson::kObjectType);
      for (auto const& it : elem.errorDetails) {
        rapidjson::Value key(it.first.c_str(), allocator);
        rapidjson::Value val(it.second.c_str(), allocator);
        details.AddMember(key, val, allocator);
      }
      error.AddMember(rapidjson::StringRef(JSON_ERROR_DETAILS), details,
                      allocator);

      errors.PushBack(error, allocator);
    }

    rJsonDoc.AddMember(rapidjson::StringRef(JSON_ERRORS), errors, allocator);
  } else {  // Changes
    if (data.response.changes.size()) {
      rapidjson::Value changes(rapidjson::kArrayType);

      for (const auto& elem : data.response.changes) {
        rapidjson::Value change(rapidjson::kObjectType);
        rapidjson::Value keyOperation(JSON_OPERATION, allocator);
        auto op = elem.operation;
        rapidjson::Value valueOperation(op.c_str(), allocator);
        change.AddMember(keyOperation, valueOperation, allocator);

        rapidjson::Value keyResourcePath(JSON_RESOURCE_PATH, allocator);
        rapidjson::Value valueResourcePath(elem.resourcePath.c_str(),
                                           allocator);
        change.AddMember(keyResourcePath, valueResourcePath, allocator);

        if (op.compare(JSON_OPERATION_DELETE)) {
          rapidjson::Value dataToEncode(rapidjson::kObjectType);

          std::size_t resourceUdmStaticData =
              elem.resourcePath.find(JSON_UDM_STATIC_DATA);
          if (resourceUdmStaticData != std::string::npos) {
            udmStaticDataToJson(dataToEncode, elem.udmStaticData, allocator);
          } else {
            std::size_t resourceUdmSliceProf =
                elem.resourcePath.find(JSON_UDM_SLICE_PROFILES);
            if (resourceUdmSliceProf != std::string::npos) {
              udmSliceProfileToJson(dataToEncode, elem.udmSliceProfile,
                                    allocator);
            } else {
              std::size_t resourceUdmRoamingArea =
                  elem.resourcePath.find(JSON_UDM_ROAMING_AREAS);
              if (resourceUdmRoamingArea != std::string::npos) {
                udmRoamingAreaToJson(dataToEncode, elem.udmRoamingArea,
                                     allocator);
              } else {
                std::size_t resourceSmsStaticData =
                    elem.resourcePath.find(JSON_SMS_STATIC_DATA);
                if (resourceSmsStaticData != std::string::npos) {
                  smsStaticDataToJson(dataToEncode, elem.smsStaticData,
                                      allocator);
                }
              }
            }
          }

          change.AddMember(rapidjson::StringRef(JSON_DATA), dataToEncode,
                           allocator);
        }

        changes.PushBack(change, allocator);
      }

      rJsonDoc.AddMember(rapidjson::StringRef(JSON_CHANGES), changes,
                         allocator);
    }
  }

  std::ostringstream os;
  if (!rJsonDoc.IsNull()) {
    acceptWriter(os);
  } else {
    os << "{}";
  }

  return os;
}

std::ostringstream ValidatorRapidJsonEncoder::errorResponseToJson(
    const ::entities::Error& err) {
  rJsonDoc.SetObject();
  rapidjson::Document::AllocatorType& allocator = rJsonDoc.GetAllocator();

  rapidjson::Value keyErrorMessage(JSON_ERROR_MESSAGE, allocator);
  rapidjson::Value valueErrorMessage(err.errorMessage.c_str(), allocator);
  rJsonDoc.AddMember(keyErrorMessage, valueErrorMessage, allocator);

  rapidjson::Value details(rapidjson::kObjectType);
  for (auto const& it : err.errorDetails) {
    rapidjson::Value key(it.first.c_str(), allocator);
    rapidjson::Value val(it.second.c_str(), allocator);
    details.AddMember(key, val, allocator);
  }
  rJsonDoc.AddMember(rapidjson::StringRef(JSON_ERROR_DETAILS), details,
                     allocator);

  std::ostringstream os;
  if (!rJsonDoc.IsNull()) {
    acceptWriter(os);
  } else {
    os << "{}";
  }
  return os;
}

void ValidatorRapidJsonEncoder::udmStaticDataToJson(
    rapidjson::Value& data, const entities::udm_static_data_t& udmStaticData,
    rapidjson::Document::AllocatorType& allocator) {
  rapidjson::Value sliceProfileId(rapidjson::kObjectType);
  strToJson(sliceProfileId, udmStaticData.udmSliceProfileId.id, JSON_ID,
            allocator);
  ValidatorVendorSpecificEncoder::vendorSpecificToJson(
      sliceProfileId, udmStaticData.udmSliceProfileId.vendorSpecific,
      allocator);
  data.AddMember(rapidjson::StringRef(JSON_UDM_SLICE_PROFILE_ID),
                 sliceProfileId, allocator);

  ValidatorVendorSpecificEncoder::vendorSpecificToJson(
      data, udmStaticData.vendorSpecific, allocator);

  if (udmStaticData.fieldExistence.hasSubscribedUeAmbr) {
    rapidjson::Value ambr(rapidjson::kObjectType);
    strToJson(ambr, udmStaticData.subscribedUeAmbr.uplink, JSON_UPLINK,
              allocator);
    strToJson(ambr, udmStaticData.subscribedUeAmbr.downlink, JSON_DOWNLINK,
              allocator);

    ValidatorVendorSpecificEncoder::vendorSpecificToJson(
        ambr, udmStaticData.subscribedUeAmbr.vendorSpecific, allocator);

    data.AddMember(rapidjson::StringRef(JSON_SUBSCRIBED_UE_AMBR), ambr,
                   allocator);
  }

  if (udmStaticData.ratRestrictions.size()) {
    rapidjson::Value ratRestrictions(rapidjson::kArrayType);
    for (const auto& elem : udmStaticData.ratRestrictions) {
      rapidjson::Value val(elem.c_str(), allocator);
      ratRestrictions.PushBack(val, allocator);
    }
    data.AddMember(rapidjson::StringRef(JSON_RAT_RESTRICTIONS), ratRestrictions,
                   allocator);
  }

  if (udmStaticData.forbiddenAreas.size()) {
    rapidjson::Value forbiddenAreas(rapidjson::kArrayType);
    for (const auto& elem : udmStaticData.forbiddenAreas) {
      rapidjson::Value area(rapidjson::kObjectType);
      strToJson(area, elem.id, JSON_ID, allocator);
      ValidatorVendorSpecificEncoder::vendorSpecificToJson(
          area, elem.vendorSpecific, allocator);
      forbiddenAreas.PushBack(area, allocator);
    }
    data.AddMember(rapidjson::StringRef(JSON_FORBIDDEN_AREAS), forbiddenAreas,
                   allocator);
  }

  if (udmStaticData.fieldExistence.hasServiceAreaRestriction) {
    rapidjson::Value area(rapidjson::kObjectType);
    strToJson(area, udmStaticData.serviceAreaRestriction.restrictionType,
              JSON_RESTRICTION_TYPE, allocator);
    if (udmStaticData.serviceAreaRestriction.areas.size()) {
      rapidjson::Value areas(rapidjson::kArrayType);
      for (const auto& elem : udmStaticData.serviceAreaRestriction.areas) {
        rapidjson::Value roamingArea(rapidjson::kObjectType);
        strToJson(roamingArea, elem.id, JSON_ID, allocator);
        ValidatorVendorSpecificEncoder::vendorSpecificToJson(
            roamingArea, elem.vendorSpecific, allocator);
        areas.PushBack(roamingArea, allocator);
      }
      area.AddMember(rapidjson::StringRef(JSON_AREAS), areas, allocator);
    }
    if (udmStaticData.fieldExistence.hasMaxNumOfTas) {
      intToJson(area, udmStaticData.serviceAreaRestriction.maxNumOfTas,
                JSON_MAX_NUM_OF_TAS, allocator);
    }
    ValidatorVendorSpecificEncoder::vendorSpecificToJson(
        area, udmStaticData.serviceAreaRestriction.vendorSpecific, allocator);
    data.AddMember(rapidjson::StringRef(JSON_SERVICE_AREA_RESTRICTION), area,
                   allocator);
  }

  if (udmStaticData.coreNetworkTypeRestrictions.size()) {
    rapidjson::Value typeRestrictions(rapidjson::kArrayType);
    for (const auto& elem : udmStaticData.coreNetworkTypeRestrictions) {
      rapidjson::Value val(elem.c_str(), allocator);
      typeRestrictions.PushBack(val, allocator);
    }
    data.AddMember(rapidjson::StringRef(JSON_CORE_NETWORK_TYPE_RESTRICTIONS),
                   typeRestrictions, allocator);
  }

  if (udmStaticData.fieldExistence.hasMpsPriority) {
    boolToJson(data, udmStaticData.mpsPriority, JSON_MPS_PRIORITY, allocator);
  }

  if (udmStaticData.fieldExistence.hasMcsPriority) {
    boolToJson(data, udmStaticData.mcsPriority, JSON_MCS_PRIORITY, allocator);
  }

  if (udmStaticData.fieldExistence.hasDlPacketCount) {
    intToJson(data, udmStaticData.dlPacketCount, JSON_DL_PACKET_COUNT,
              allocator);
  }

  if (udmStaticData.fieldExistence.hasMicoAllowed) {
    boolToJson(data, udmStaticData.micoAllowed, JSON_MICO_ALLOWED, allocator);
  }

  if (udmStaticData.fieldExistence.hasStaticIpAddress) {
    rapidjson::Value staticIpAddr(rapidjson::kObjectType);
    if (udmStaticData.staticIpAddress.nssaiDnnIpLists.size()) {
      rapidjson::Value nssaiList(rapidjson::kArrayType);
      for (const auto& elem : udmStaticData.staticIpAddress.nssaiDnnIpLists) {
        rapidjson::Value nssaiDnn(rapidjson::kObjectType);
        rapidjson::Value nssai(rapidjson::kObjectType);
        intToJson(nssai, elem.snssai.sst, JSON_SST, allocator);
        strToJson(nssai, elem.snssai.sd, JSON_SD, allocator);
        ValidatorVendorSpecificEncoder::vendorSpecificToJson(
            nssai, elem.snssai.vendorSpecific, allocator);
        nssaiDnn.AddMember(rapidjson::StringRef(JSON_NSSAI), nssai, allocator);

        rapidjson::Value ipList(rapidjson::kArrayType);
        for (const auto& subElem : elem.dnnIpList) {
          rapidjson::Value dnnObj(rapidjson::kObjectType);
          strToJson(dnnObj, subElem.dnn, JSON_DNN, allocator);
          rapidjson::Value singleIp(rapidjson::kArrayType);
          for (const auto& ip : subElem.ipAddress) {
            rapidjson::Value addr(rapidjson::kObjectType);
            if (ip.ipType == entities::IpAddrType::IPv4) {
              strToJson(addr, boost::get<entities::ipv4_t>(ip.ip),
                        JSON_IPV4_ADDR, allocator);
            } else if (ip.ipType == entities::IpAddrType::IPv6) {
              strToJson(addr, boost::get<entities::ipv6_t>(ip.ip),
                        JSON_IPV6_ADDR, allocator);
            } else if (ip.ipType == entities::IpAddrType::IPv6Prefix) {
              strToJson(addr, boost::get<entities::ipv6_prefix_t>(ip.ip),
                        JSON_IPV6_PREFIX, allocator);
            }
            ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                addr, ip.vendorSpecific, allocator);
            singleIp.PushBack(addr, allocator);
          }
          dnnObj.AddMember(rapidjson::StringRef(JSON_IPADDRESS), singleIp,
                           allocator);

          ValidatorVendorSpecificEncoder::vendorSpecificToJson(
              dnnObj, subElem.vendorSpecific, allocator);

          ipList.PushBack(dnnObj, allocator);
        }
        nssaiDnn.AddMember(rapidjson::StringRef(JSON_DNN_IP_LIST), ipList,
                           allocator);
        ValidatorVendorSpecificEncoder::vendorSpecificToJson(
            nssaiDnn, elem.vendorSpecific, allocator);
        nssaiList.PushBack(nssaiDnn, allocator);
      }
      staticIpAddr.AddMember(rapidjson::StringRef(JSON_NSS_AI_DNN_IP_LISTS),
                             nssaiList, allocator);
    }

    ValidatorVendorSpecificEncoder::vendorSpecificToJson(
        staticIpAddr, udmStaticData.staticIpAddress.vendorSpecific, allocator);

    data.AddMember(rapidjson::StringRef(JSON_STATIC_IP_ADDRESS), staticIpAddr,
                   allocator);
  }

  strToJson(data, udmStaticData.roamingOdb, JSON_ROAMING_ODB, allocator);

  if (udmStaticData.fieldExistence.hasUeUsageType) {
    intToJson(data, udmStaticData.ueUsageType, JSON_UE_USAGE_TYPE, allocator);
  }

  if (udmStaticData.fieldExistence.hasRfspIndex) {
    intToJson(data, udmStaticData.rfspIndex, JSON_RFS_INDEX, allocator);
  }

  if (udmStaticData.fieldExistence.hasSubsRegTimer) {
    intToJson(data, udmStaticData.subsRegTimer, JSON_SUBS_REG_TIMER, allocator);
  }

  if (udmStaticData.fieldExistence.hasUdmSliceData) {
    rapidjson::Value udmSliceData(rapidjson::kObjectType);

    plmnIdsToJson(udmSliceData, udmStaticData.udmSliceData.plmnIdSets,
                  allocator);

    data.AddMember(rapidjson::StringRef(JSON_UDM_SLICE_DATA), udmSliceData,
                   allocator);
  }
}

void ValidatorRapidJsonEncoder::smsStaticDataToJson(
    rapidjson::Value& data, const entities::sms_static_data_t& smsStaticData,
    rapidjson::Document::AllocatorType& allocator) {
  ValidatorVendorSpecificEncoder::vendorSpecificToJson(
      data, smsStaticData.vendorSpecific, allocator);

  if (smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData) {
    rapidjson::Value smsSubscriptionData(rapidjson::kObjectType);
    if (smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed) {
      boolToJson(smsSubscriptionData,
                 smsStaticData.smsSubscriptionData.smsSubscribed,
                 JSON_SMS_SUBSCRIBED, allocator);
    }

    ValidatorVendorSpecificEncoder::vendorSpecificToJson(
        smsSubscriptionData, smsStaticData.smsSubscriptionData.vendorSpecific,
        allocator);

    data.AddMember(rapidjson::StringRef(JSON_SMS_SUBSCRIPTION_DATA),
                   smsSubscriptionData, allocator);
  }
  if (smsStaticData.fieldExistenceSmsStaticData
          .hasSmsManagementSubscriptionData) {
    rapidjson::Value smsManagementSubscriptionData(rapidjson::kObjectType);

    if (smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed) {
      boolToJson(smsManagementSubscriptionData,
                 smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed,
                 JSON_MT_SMS_SUBSCRIBED, allocator);
    }
    if (smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll) {
      boolToJson(smsManagementSubscriptionData,
                 smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll,
                 JSON_MT_SMS_BARRING_ALL, allocator);
    }
    if (smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming) {
      boolToJson(
          smsManagementSubscriptionData,
          smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming,
          JSON_MT_SMS_BARRING_ROAMING, allocator);
    }
    if (smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed) {
      boolToJson(smsManagementSubscriptionData,
                 smsStaticData.smsManagementSubscriptionData.moSmsSubscribed,
                 JSON_MO_SMS_SUBSCRIBED, allocator);
    }
    if (smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll) {
      boolToJson(smsManagementSubscriptionData,
                 smsStaticData.smsManagementSubscriptionData.moSmsBarringAll,
                 JSON_MO_SMS_BARRING_ALL, allocator);
    }
    if (smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming) {
      boolToJson(
          smsManagementSubscriptionData,
          smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming,
          JSON_MO_SMS_BARRING_ROAMING, allocator);
    }

    ValidatorVendorSpecificEncoder::vendorSpecificToJson(
        smsManagementSubscriptionData,
        smsStaticData.smsManagementSubscriptionData.vendorSpecific, allocator);

    data.AddMember(rapidjson::StringRef(JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA),
                   smsManagementSubscriptionData, allocator);
  }
}

void ValidatorRapidJsonEncoder::udmSliceProfileToJson(
    rapidjson::Value& data, const entities::udm_slice_prof_t& udmSliceProfile,
    rapidjson::Document::AllocatorType& allocator) {
  ValidatorVendorSpecificEncoder::vendorSpecificToJson(
      data, udmSliceProfile.vendorSpecific, allocator);

  strToJson(data, udmSliceProfile.udmSliceProfileId, JSON_UDM_SLICE_PROFILE_ID,
            allocator);

  plmnIdsToJson(data, udmSliceProfile.plmnIdSets, allocator);
}

void ValidatorRapidJsonEncoder::plmnIdsToJson(
    rapidjson::Value& data, const entities::plmn_id_sets_t& plmns,
    rapidjson::Document::AllocatorType& allocator) {
  if (plmns.size()) {
    rapidjson::Value plmnIdSets(rapidjson::kArrayType);
    for (const auto& elem : plmns) {
      rapidjson::Value plmnId(rapidjson::kObjectType);

      ValidatorVendorSpecificEncoder::vendorSpecificToJson(
          plmnId, elem.vendorSpecific, allocator);

      strToJson(plmnId, elem.plmnIdSetIdentifier, JSON_PLMN_ID_SET_IDENTIFIER,
                allocator);

      if (elem.fieldExistenceUdmSliceProfile.hasAnyNssai) {
        rapidjson::Value nssai(rapidjson::kObjectType);

        ValidatorVendorSpecificEncoder::vendorSpecificToJson(
            nssai, elem.nssai.vendorSpecific, allocator);

        if (elem.nssai.defaultSingleNssais.size() > 0) {
          rapidjson::Value defaultSingleNssais(rapidjson::kArrayType);
          for (const auto& defaultNssai : elem.nssai.defaultSingleNssais) {
            rapidjson::Value obj1(rapidjson::kObjectType);
            ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                obj1, defaultNssai.vendorSpecific, allocator);
            strToJson(obj1, defaultNssai.dnnListId, JSON_DNN_LIST_ID,
                      allocator);
            strToJson(obj1, defaultNssai.threegppChargingCharacteristics,
                      JSON_3GPP_CHARGING_CHARACTERISTICS, allocator);
            rapidjson::Value obj2(rapidjson::kObjectType);
            ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                obj2, defaultNssai.nssai.vendorSpecific, allocator);
            intToJson(obj2, defaultNssai.nssai.sst, JSON_SST, allocator);
            strToJson(obj2, defaultNssai.nssai.sd, JSON_SD, allocator);
            obj1.AddMember(rapidjson::StringRef(JSON_NSSAI), obj2, allocator);
            defaultSingleNssais.PushBack(obj1, allocator);
          }
          nssai.AddMember(rapidjson::StringRef(JSON_DEFAULT_SINGLE_NSSAIS),
                          defaultSingleNssais, allocator);
        }

        if (elem.fieldExistenceUdmSliceProfile.hasNssai) {
          if (elem.nssai.singleNssais.size() > 0) {
            rapidjson::Value singleNssais(rapidjson::kArrayType);
            for (const auto& nssaiId : elem.nssai.singleNssais) {
              rapidjson::Value obj1(rapidjson::kObjectType);
              ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                  obj1, nssaiId.vendorSpecific, allocator);
              strToJson(obj1, nssaiId.dnnListId, JSON_DNN_LIST_ID, allocator);
              strToJson(obj1, nssaiId.threegppChargingCharacteristics,
                        JSON_3GPP_CHARGING_CHARACTERISTICS, allocator);
              rapidjson::Value obj2(rapidjson::kObjectType);
              ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                  obj2, nssaiId.nssai.vendorSpecific, allocator);
              intToJson(obj2, nssaiId.nssai.sst, JSON_SST, allocator);
              strToJson(obj2, nssaiId.nssai.sd, JSON_SD, allocator);
              obj1.AddMember(rapidjson::StringRef(JSON_NSSAI), obj2, allocator);
              singleNssais.PushBack(obj1, allocator);
            }
            nssai.AddMember(rapidjson::StringRef(JSON_SINGLE_NSSAIS),
                            singleNssais, allocator);
          }
        }
        plmnId.AddMember(rapidjson::StringRef(JSON_NSSAI), nssai, allocator);
      }

      if (elem.dnnLists.size() > 0) {
        rapidjson::Value dnnLists(rapidjson::kArrayType);
        for (const auto& dnnElem : elem.dnnLists) {
          rapidjson::Value obj1(rapidjson::kObjectType);
          ValidatorVendorSpecificEncoder::vendorSpecificToJson(
              obj1, dnnElem.vendorSpecific, allocator);
          strToJson(obj1, dnnElem.dnnListId, JSON_DNN_LIST_ID, allocator);
          if (dnnElem.dnnDataList.size() > 0) {
            rapidjson::Value dnnDataList(rapidjson::kArrayType);
            for (const auto& dnnDataElem : dnnElem.dnnDataList) {
              rapidjson::Value obj2(rapidjson::kObjectType);
              ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                  obj2, dnnDataElem.vendorSpecific, allocator);
              strToJson(obj2, dnnDataElem.dnn, JSON_DNN, allocator);
              if (dnnDataElem.fieldExistence.hasDefaultDnnIndicator) {
                boolToJson(obj2, dnnDataElem.defaultDnnIndicator,
                           JSON_DEFAULT_DNN_INDICATOR, allocator);
              }
              if (dnnDataElem.fieldExistence.hasLboRoamingAllowed) {
                boolToJson(obj2, dnnDataElem.lboRoamingAllowed,
                           JSON_LBO_ROAMING_ALLOWED, allocator);
              }
              if (dnnDataElem.fieldExistence.hasIwkEpsInd) {
                boolToJson(obj2, dnnDataElem.iwkEpsInd, JSON_IWK_EPS_IND,
                           allocator);
              }
              if (dnnDataElem.fieldExistence.hasLadnIndicator) {
                boolToJson(obj2, dnnDataElem.ladnIndicator, JSON_LADN_INDICATOR,
                           allocator);
              }

              if (dnnDataElem.fieldExistence.hasPduSessionTypes) {
                rapidjson::Value obj3(rapidjson::kObjectType);
                ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                    obj3, dnnDataElem.pduSessionTypes.vendorSpecific,
                    allocator);
                strToJson(obj3, dnnDataElem.pduSessionTypes.defaultSessionType,
                          JSON_DEFAULT_SESSION_TYPE, allocator);
                if (dnnDataElem.pduSessionTypes.allowedSessionTypes.size() >
                    0) {
                  rapidjson::Value obj4(rapidjson::kArrayType);
                  for (const auto& allowedSessElem :
                       dnnDataElem.pduSessionTypes.allowedSessionTypes) {
                    rapidjson::Value val(allowedSessElem.c_str(), allocator);
                    obj4.PushBack(val, allocator);
                  }
                  obj3.AddMember(
                      rapidjson::StringRef(JSON_ALLOWED_SESSION_TYPES), obj4,
                      allocator);
                }
                obj2.AddMember(rapidjson::StringRef(JSON_PDU_SESSION_TYPES),
                               obj3, allocator);
              }

              if (dnnDataElem.fieldExistence.hasSscModes) {
                rapidjson::Value sscModes(rapidjson::kObjectType);
                ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                    sscModes, dnnDataElem.sscModes.vendorSpecific, allocator);
                strToJson(sscModes, dnnDataElem.sscModes.defaultSscMode,
                          JSON_DEFAULT_SSC_MODE, allocator);
                if (dnnDataElem.sscModes.allowedSscModes.size() > 0) {
                  rapidjson::Value allowedSscModes(rapidjson::kArrayType);
                  for (const auto& allowedSscElem :
                       dnnDataElem.sscModes.allowedSscModes) {
                    rapidjson::Value val(allowedSscElem.c_str(), allocator);
                    allowedSscModes.PushBack(val, allocator);
                  }
                  sscModes.AddMember(
                      rapidjson::StringRef(JSON_ALLOWED_SSC_MODES),
                      allowedSscModes, allocator);
                }
                obj2.AddMember(rapidjson::StringRef(JSON_SSC_MODES), sscModes,
                               allocator);
              }

              if (dnnDataElem.fieldExistence.hasFivegQosProfile) {
                rapidjson::Value fivegQosProfile(rapidjson::kObjectType);
                ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                    fivegQosProfile, dnnDataElem.fivegQosProfile.vendorSpecific,
                    allocator);
                intToJson(fivegQosProfile, dnnDataElem.fivegQosProfile.fiveqi,
                          JSON_5QI, allocator);
                rapidjson::Value arp(rapidjson::kObjectType);
                ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                    arp, dnnDataElem.fivegQosProfile.arp.vendorSpecific,
                    allocator);
                intToJson(arp, dnnDataElem.fivegQosProfile.arp.priorityLevel,
                          JSON_PRIORITY_LEVEL, allocator);
                strToJson(arp, dnnDataElem.fivegQosProfile.arp.preemptCap,
                          JSON_PREEMPTCAP, allocator);
                strToJson(arp, dnnDataElem.fivegQosProfile.arp.preemptVuln,
                          JSON_PREEMPTVULN, allocator);
                fivegQosProfile.AddMember(rapidjson::StringRef(JSON_ARP), arp,
                                          allocator);
                if (dnnDataElem.fivegQosProfile.fieldExistence
                        .hasPriorityLevel) {
                  intToJson(fivegQosProfile,
                            dnnDataElem.fivegQosProfile.priorityLevel,
                            JSON_PRIORITY_LEVEL, allocator);
                }
                obj2.AddMember(rapidjson::StringRef(JSON_5G_QOS_PROFILE),
                               fivegQosProfile, allocator);
              }

              if (dnnDataElem.fieldExistence.hasSessionAmbr) {
                rapidjson::Value sessionAmbr(rapidjson::kObjectType);
                ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                    sessionAmbr, dnnDataElem.sessionAmbr.vendorSpecific,
                    allocator);
                strToJson(sessionAmbr, dnnDataElem.sessionAmbr.uplink,
                          JSON_UPLINK, allocator);
                strToJson(sessionAmbr, dnnDataElem.sessionAmbr.downlink,
                          JSON_DOWNLINK, allocator);
                obj2.AddMember(rapidjson::StringRef(JSON_SESSION_AMBR),
                               sessionAmbr, allocator);
              }

              strToJson(obj2, dnnDataElem.threegppChargingCharacteristics,
                        JSON_3GPP_CHARGING_CHARACTERISTICS, allocator);

              if (dnnDataElem.fieldExistence.hasUpSecurity) {
                rapidjson::Value upSecurity(rapidjson::kObjectType);
                ValidatorVendorSpecificEncoder::vendorSpecificToJson(
                    upSecurity, dnnDataElem.upSecurity.vendorSpecific,
                    allocator);
                strToJson(upSecurity, dnnDataElem.upSecurity.upIntegr,
                          JSON_UP_INTEGR, allocator);
                strToJson(upSecurity, dnnDataElem.upSecurity.upConfid,
                          JSON_UP_CONFID, allocator);
                obj2.AddMember(rapidjson::StringRef(JSON_UP_SECURITY),
                               upSecurity, allocator);
              }

              if (dnnDataElem.fieldExistence.hasSmfList) {
                rapidjson::Value smfList(rapidjson::kArrayType);
                for (const auto& smfListElem : dnnDataElem.smfList) {
                  rapidjson::Value val(smfListElem.c_str(), allocator);
                  smfList.PushBack(val, allocator);
                }
                obj2.AddMember(rapidjson::StringRef(JSON_SMF_LIST), smfList,
                               allocator);
              }
              if (dnnDataElem.fieldExistence.hasSameSmfInd) {
                boolToJson(obj2, dnnDataElem.sameSmfInd, JSON_SAME_SMF_IND,
                           allocator);
              }
              dnnDataList.PushBack(obj2, allocator);
            }
            obj1.AddMember(rapidjson::StringRef(JSON_DNN_DATA_LIST),
                           dnnDataList, allocator);
          }
          dnnLists.PushBack(obj1, allocator);
        }
        plmnId.AddMember(rapidjson::StringRef(JSON_DNN_LISTS), dnnLists,
                         allocator);
      }

      plmnIdSets.PushBack(plmnId, allocator);
    }
    data.AddMember(rapidjson::StringRef(JSON_PLMN_ID_SETS), plmnIdSets,
                   allocator);
  }
}

void ValidatorRapidJsonEncoder::udmRoamingAreaToJson(
    rapidjson::Value& data, const entities::udm_roaming_area_t& udmRoamingArea,
    rapidjson::Document::AllocatorType& allocator) {
  ValidatorVendorSpecificEncoder::vendorSpecificToJson(
      data, udmRoamingArea.vendorSpecific, allocator);

  strToJson(data, udmRoamingArea.udmRoamingAreaId, JSON_UDM_ROAMING_AREA_ID,
            allocator);

  if (udmRoamingArea.areaList.size() > 0) {
    rapidjson::Value areaList(rapidjson::kArrayType);
    for (const auto& elem : udmRoamingArea.areaList) {
      rapidjson::Value areaObj(rapidjson::kObjectType);
      ValidatorVendorSpecificEncoder::vendorSpecificToJson(
          areaObj, elem.vendorSpecific, allocator);
      if (elem.area.type() == typeid(entities::tacs_t)) {
        entities::tacs_t tacs = boost::get<entities::tacs_t>(elem.area);
        if (tacs.size() > 0) {
          rapidjson::Value tacsList(rapidjson::kArrayType);
          for (const auto& tac : tacs) {
            rapidjson::Value val(tac.c_str(), allocator);
            tacsList.PushBack(val, allocator);
          }
          areaObj.AddMember(rapidjson::StringRef(JSON_TACS), tacsList,
                            allocator);
        }
      } else {
        strToJson(areaObj, boost::get<std::string>(elem.area), JSON_AREA_CODE,
                  allocator);
      }
      areaList.PushBack(areaObj, allocator);
    }
    data.AddMember(rapidjson::StringRef(JSON_AREA_LIST), areaList, allocator);
  }
}

void ValidatorRapidJsonEncoder::strToJson(
    rapidjson::Value& data, const std::string& str, const std::string& keyName,
    rapidjson::Document::AllocatorType& allocator) {
  if (not str.empty()) {
    rapidjson::Value key(keyName.c_str(), allocator);
    rapidjson::Value val(str.c_str(), allocator);
    data.AddMember(key, val, allocator);
  }
}

void ValidatorRapidJsonEncoder::boolToJson(
    rapidjson::Value& data, const bool& attr, const std::string& keyName,
    rapidjson::Document::AllocatorType& allocator) {
  rapidjson::Value key(keyName.c_str(), allocator);
  data.AddMember(key, rapidjson::Value().SetBool(attr), allocator);
}

void ValidatorRapidJsonEncoder::intToJson(
    rapidjson::Value& data, const int& attr, const std::string& keyName,
    rapidjson::Document::AllocatorType& allocator) {
  rapidjson::Value key(keyName.c_str(), allocator);
  data.AddMember(key, rapidjson::Value().SetInt(attr), allocator);
}

void ValidatorRapidJsonEncoder::acceptWriter(std::ostringstream& os) const {
  rapidjson::StringBuffer buffer;
  if (prettyFormat) {
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    rJsonDoc.Accept(writer);
  } else {
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    rJsonDoc.Accept(writer);
  }
  os << buffer.GetString();
}

}  // namespace json
}  // namespace secondary
}  // namespace port
