#include "ValidatorRapidJsonParser.hpp"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <boost/regex.hpp>

#include "JsonConstants.hpp"
#include "ValidatorSmsSubsParser.hpp"
#include "ValidatorVendorSpecificParser.hpp"
#include "ports/ports.hpp"

namespace port {
namespace secondary {
namespace json {

ValidatorRapidJsonParser::ValidatorRapidJsonParser(
    const std::string& json_string)
    : rJsonDoc{}, rJsonParseError{false}, rJsonParseErrorStr{} {
  if (not(rapidjson::ParseResult) rJsonDoc.Parse(json_string.c_str())) {
    setParsingError("wrong json format");
  }
}

bool ValidatorRapidJsonParser::getValidationData(
    entities::ValidationData& data) {
  if (error()) {
    return false;
  }

  if (not getChangesToValidate(data)) {
    return false;
  }

  getRelatedResources(data);

  return true;
}

bool ValidatorRapidJsonParser::getChangesToValidate(
    entities::ValidationData& data) {
  if (rJsonDoc.HasMember(JSON_CHANGES)) {
    rapidjson::Value& changesList = rJsonDoc[JSON_CHANGES];

    if (not changesList.IsArray()) {
      setParsingError(std::string{JSON_CHANGES} + " is not a list");
      return false;
    }

    for (rapidjson::Value::ConstValueIterator itr = changesList.Begin();
         itr != changesList.End(); ++itr) {
      const rapidjson::Value& attribute = *itr;

      if (not attribute.IsObject()) {
        setParsingError("change is not an object");
        return false;
      }

      entities::Change change;

      for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin();
           itr2 != attribute.MemberEnd(); ++itr2) {
        if (not itr2->name.IsString()) {
          setParsingError("attribute name is not string");
          return false;
        }
        std::string attr = itr2->name.GetString();

        if (attr == JSON_OPERATION) {
          if (not itr2->value.IsString()) {
            setParsingError(std::string{JSON_OPERATION} + " is not string");
            return false;
          }
          change.operation = itr2->value.GetString();
          if (change.operation.empty()) {
            setEmptyFieldError(JSON_OPERATION);
            return false;
          }
          if (change.operation != JSON_OPERATION_CREATE &&
              change.operation != JSON_OPERATION_UPDATE &&
              change.operation != JSON_OPERATION_DELETE) {
            setParsingError(
                "operation value is not allowed. It should be either: CREATE, "
                "UPDATE or DELETE");
            return false;
          }
        } else if (attr == JSON_RESOURCE_PATH) {
          if (not itr2->value.IsString()) {
            setParsingError(std::string{JSON_RESOURCE_PATH} + " is not string");
            return false;
          }
          change.resourcePath = itr2->value.GetString();
          if (change.resourcePath.empty()) {
            setEmptyFieldError(JSON_RESOURCE_PATH);
            return false;
          }
        } else if (attr == JSON_DATA) {
          const rapidjson::Value& attrData = itr2->value;

          if (not attrData.IsObject()) {
            setParsingError(std::string{JSON_DATA} + " is not an object");
            return false;
          }

          std::size_t smsStaticData =
              change.resourcePath.find(JSON_SMS_STATIC_DATA);
          if (smsStaticData != std::string::npos) {
            ValidatorSmsSubsParser smsStaticDataParser(data,
                                                       change.resourcePath);
            smsStaticDataParser.getOptionalSmsStaticData(attrData,
                                                         change.smsStaticData);
          }

          std::size_t resource = change.resourcePath.find(JSON_UDM_STATIC_DATA);
          if (resource != std::string::npos) {
            getUdmStaticData(attrData, change.udmStaticData, data,
                             change.resourcePath);
          }
          std::size_t resourceProfile =
              change.resourcePath.find(JSON_UDM_SLICE_PROFILES);

          if (resourceProfile != std::string::npos) {
            getUdmSliceProfile(attrData, change.udmSliceProfile, data,
                               change.resourcePath);
          }

          std::size_t resourceRoaming =
              change.resourcePath.find(JSON_UDM_ROAMING_AREAS);

          if (resourceRoaming != std::string::npos) {
            getUdmRoamingAreas(attrData, change.udmRoamingArea, data,
                               change.resourcePath);
          }
        }
      }

      data.changes.push_back(change);
    }
  }

  return true;
}

void ValidatorRapidJsonParser::getUdmStaticData(
    const rapidjson::Value& attrData, entities::UdmStaticData& udmStaticData,
    entities::ValidationData& data, const std::string& resourcePath) {
  ValidatorVendorSpecificParser::getVendorSpecific(
      attrData, udmStaticData.vendorSpecific);

  if (attrData.HasMember(JSON_SUBSCRIBED_UE_AMBR)) {
    if (not attrData[JSON_SUBSCRIBED_UE_AMBR].IsObject()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notObjectFieldError(JSON_SUBSCRIBED_UE_AMBR)}});
    } else {
      ValidatorVendorSpecificParser::getVendorSpecific(
          attrData[JSON_SUBSCRIBED_UE_AMBR],
          udmStaticData.subscribedUeAmbr.vendorSpecific);
      if (attrData[JSON_SUBSCRIBED_UE_AMBR].HasMember(JSON_UPLINK)) {
        if (not attrData[JSON_SUBSCRIBED_UE_AMBR][JSON_UPLINK].IsString()) {
          data.addError(
              "Constraint Violation",
              {{"resource_path", resourcePath},
               {"description", data.notStringFieldError(JSON_UPLINK)}});
        } else {
          udmStaticData.subscribedUeAmbr.uplink =
              attrData[JSON_SUBSCRIBED_UE_AMBR][JSON_UPLINK].GetString();

          if (udmStaticData.subscribedUeAmbr.uplink.empty()) {
            data.addError("Constraint Violation",
                          {{"resource_path", resourcePath},
                           {"description", data.emptyFieldError(JSON_UPLINK)}});
          }
        }
      } else {
        data.addError(
            "Constraint Violation",
            {{"resource_path", resourcePath},
             {"description", data.unfoundMandatoryFieldError(
                                 JSON_UPLINK, JSON_SUBSCRIBED_UE_AMBR)}});
      }

      if (attrData[JSON_SUBSCRIBED_UE_AMBR].HasMember(JSON_DOWNLINK)) {
        if (not attrData[JSON_SUBSCRIBED_UE_AMBR][JSON_DOWNLINK].IsString()) {
          data.addError(
              "Constraint Violation",
              {{"resource_path", resourcePath},
               {"description", data.notStringFieldError(JSON_DOWNLINK)}});
        } else {
          udmStaticData.subscribedUeAmbr.downlink =
              attrData[JSON_SUBSCRIBED_UE_AMBR][JSON_DOWNLINK].GetString();

          if (udmStaticData.subscribedUeAmbr.downlink.empty()) {
            data.addError(
                "Constraint Violation",
                {{"resource_path", resourcePath},
                 {"description", data.emptyFieldError(JSON_DOWNLINK)}});
          }
        }
      } else {
        data.addError(
            "Constraint Violation",
            {{"resource_path", resourcePath},
             {"description", data.unfoundMandatoryFieldError(
                                 JSON_DOWNLINK, JSON_SUBSCRIBED_UE_AMBR)}});
      }
    }
    udmStaticData.fieldExistence.hasSubscribedUeAmbr = true;
  } else {
    udmStaticData.fieldExistence.hasSubscribedUeAmbr = false;
  }

  if (attrData.HasMember(JSON_RAT_RESTRICTIONS)) {
    if (not attrData[JSON_RAT_RESTRICTIONS].IsArray()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notArrayFieldError(JSON_RAT_RESTRICTIONS)}});
    } else {
      for (auto& v : attrData[JSON_RAT_RESTRICTIONS].GetArray()) {
        if (not v.IsString()) {
          data.addError("Constraint Violation",
                        {{"resource_path", resourcePath},
                         {"description",
                          data.notStringFieldError(JSON_RAT_RESTRICTIONS)}});
        } else {
          udmStaticData.ratRestrictions.push_back(v.GetString());
        }
      }
    }
  }

  if (attrData.HasMember(JSON_FORBIDDEN_AREAS)) {
    if (not attrData[JSON_FORBIDDEN_AREAS].IsArray()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notArrayFieldError(JSON_FORBIDDEN_AREAS)}});
    } else {
      for (auto& v : attrData[JSON_FORBIDDEN_AREAS].GetArray()) {
        entities::UdmRoamingAreaReference udmRoamingAreaReference;
        auto obj = v.GetObject();
        ValidatorVendorSpecificParser::getVendorSpecific(
            v, udmRoamingAreaReference.vendorSpecific);
        rapidjson::Value::ConstMemberIterator iterator =
            obj.FindMember(JSON_ID);
        if (iterator != obj.MemberEnd()) {
          if (not iterator->value.IsString()) {
            data.addError("Constraint Violation",
                          {{"resource_path", resourcePath},
                           {"description", data.notStringFieldError(JSON_ID)}});
          } else {
            udmRoamingAreaReference.id = iterator->value.GetString();
            udmStaticData.forbiddenAreas.push_back(udmRoamingAreaReference);
          }
        } else {
          data.addError("Constraint Violation",
                        {{"resource_path", resourcePath},
                         {"description", data.unfoundMandatoryFieldError(
                                             JSON_ID, JSON_FORBIDDEN_AREAS)}});
        }
      }
    }
  }

  if (attrData.HasMember(JSON_SERVICE_AREA_RESTRICTION)) {
    if (not attrData[JSON_SERVICE_AREA_RESTRICTION].IsObject()) {
      data.addError("Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description", data.notObjectFieldError(
                                         JSON_SERVICE_AREA_RESTRICTION)}});
    } else {
      ValidatorVendorSpecificParser::getVendorSpecific(
          attrData[JSON_SERVICE_AREA_RESTRICTION],
          udmStaticData.serviceAreaRestriction.vendorSpecific);
      if (attrData[JSON_SERVICE_AREA_RESTRICTION].HasMember(
              JSON_RESTRICTION_TYPE)) {
        if (not attrData[JSON_SERVICE_AREA_RESTRICTION][JSON_RESTRICTION_TYPE]
                    .IsString()) {
          data.addError("Constraint Violation",
                        {{"resource_path", resourcePath},
                         {"description",
                          data.notStringFieldError(JSON_RESTRICTION_TYPE)}});
        } else {
          udmStaticData.serviceAreaRestriction.restrictionType =
              attrData[JSON_SERVICE_AREA_RESTRICTION][JSON_RESTRICTION_TYPE]
                  .GetString();
        }
      }
      if (attrData[JSON_SERVICE_AREA_RESTRICTION].HasMember(JSON_AREAS)) {
        if (not attrData[JSON_SERVICE_AREA_RESTRICTION][JSON_AREAS].IsArray()) {
          data.addError("Constraint Violation",
                        {{"resource_path", resourcePath},
                         {"description", data.notArrayFieldError(JSON_AREAS)}});
        } else {
          for (auto& v :
               attrData[JSON_SERVICE_AREA_RESTRICTION][JSON_AREAS].GetArray()) {
            entities::UdmRoamingAreaReference udmRoamingAreaReference;
            if (not v.IsObject()) {
              data.addError(
                  "Constraint Violation",
                  {{"resource_path", resourcePath},
                   {"description", data.notObjectFieldError(JSON_AREAS)}});
            } else {
              auto obj = v.GetObject();
              ValidatorVendorSpecificParser::getVendorSpecific(
                  v, udmRoamingAreaReference.vendorSpecific);
              rapidjson::Value::ConstMemberIterator iterator =
                  obj.FindMember(JSON_ID);
              if (iterator != obj.MemberEnd()) {
                if (not iterator->value.IsString()) {
                  data.addError(
                      "Constraint Violation",
                      {{"resource_path", resourcePath},
                       {"description", data.notStringFieldError(JSON_ID)}});
                } else {
                  udmRoamingAreaReference.id = iterator->value.GetString();
                  udmStaticData.serviceAreaRestriction.areas.push_back(
                      udmRoamingAreaReference);
                }
              } else {
                data.addError("Constraint Violation",
                              {{"resource_path", resourcePath},
                               {"description", data.unfoundMandatoryFieldError(
                                                   JSON_ID, JSON_AREAS)}});
              }
            }
          }
        }
      }
      if (attrData[JSON_SERVICE_AREA_RESTRICTION].HasMember(
              JSON_MAX_NUM_OF_TAS)) {
        if (not attrData[JSON_SERVICE_AREA_RESTRICTION][JSON_MAX_NUM_OF_TAS]
                    .IsInt()) {
          data.addError(
              "Constraint Violation",
              {{"resource_path", resourcePath},
               {"description", data.notIntFieldError(JSON_MAX_NUM_OF_TAS)}});
        } else {
          udmStaticData.serviceAreaRestriction.maxNumOfTas =
              attrData[JSON_SERVICE_AREA_RESTRICTION][JSON_MAX_NUM_OF_TAS]
                  .GetInt();
          udmStaticData.fieldExistence.hasMaxNumOfTas = true;
        }
      } else {
        udmStaticData.fieldExistence.hasMaxNumOfTas = false;
      }

      udmStaticData.fieldExistence.hasServiceAreaRestriction = true;
    }
  } else {
    udmStaticData.fieldExistence.hasServiceAreaRestriction = false;
  }

  if (attrData.HasMember(JSON_CORE_NETWORK_TYPE_RESTRICTIONS)) {
    if (not attrData[JSON_CORE_NETWORK_TYPE_RESTRICTIONS].IsArray()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description",
            data.notArrayFieldError(JSON_CORE_NETWORK_TYPE_RESTRICTIONS)}});
    } else {
      for (auto& v : attrData[JSON_CORE_NETWORK_TYPE_RESTRICTIONS].GetArray()) {
        if (not v.IsString()) {
          data.addError(
              "Constraint Violation",
              {{"resource_path", resourcePath},
               {"description", data.notStringFieldError(
                                   JSON_CORE_NETWORK_TYPE_RESTRICTIONS)}});
        } else {
          udmStaticData.coreNetworkTypeRestrictions.push_back(v.GetString());
        }
      }
    }
  }

  if (attrData.HasMember(JSON_MPS_PRIORITY)) {
    if (not attrData[JSON_MPS_PRIORITY].IsBool()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notBoolFieldError(JSON_MPS_PRIORITY)}});
    } else {
      udmStaticData.mpsPriority = attrData[JSON_MPS_PRIORITY].GetBool();
      udmStaticData.fieldExistence.hasMpsPriority = true;
    }
  } else {
    udmStaticData.fieldExistence.hasMpsPriority = false;
  }

  if (attrData.HasMember(JSON_MCS_PRIORITY)) {
    if (not attrData[JSON_MCS_PRIORITY].IsBool()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notBoolFieldError(JSON_MCS_PRIORITY)}});
    } else {
      udmStaticData.mcsPriority = attrData[JSON_MCS_PRIORITY].GetBool();
      udmStaticData.fieldExistence.hasMcsPriority = true;
    }
  } else {
    udmStaticData.fieldExistence.hasMcsPriority = false;
  }

  if (attrData.HasMember(JSON_DL_PACKET_COUNT)) {
    if (not attrData[JSON_DL_PACKET_COUNT].IsInt()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notIntFieldError(JSON_DL_PACKET_COUNT)}});
    } else {
      udmStaticData.dlPacketCount = attrData[JSON_DL_PACKET_COUNT].GetInt();
      udmStaticData.fieldExistence.hasDlPacketCount = true;
    }
  } else {
    udmStaticData.fieldExistence.hasDlPacketCount = false;
  }

  if (attrData.HasMember(JSON_MICO_ALLOWED)) {
    if (not attrData[JSON_MICO_ALLOWED].IsBool()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notBoolFieldError(JSON_MICO_ALLOWED)}});
    } else {
      udmStaticData.micoAllowed = attrData[JSON_MICO_ALLOWED].GetBool();
      udmStaticData.fieldExistence.hasMicoAllowed = true;
    }
  } else {
    udmStaticData.fieldExistence.hasMicoAllowed = false;
  }

  if (attrData.HasMember(JSON_UDM_SLICE_PROFILE_ID)) {
    if (attrData[JSON_UDM_SLICE_PROFILE_ID].IsObject()) {
      ValidatorVendorSpecificParser::getVendorSpecific(
          attrData[JSON_UDM_SLICE_PROFILE_ID],
          udmStaticData.udmSliceProfileId.vendorSpecific);
      if (attrData[JSON_UDM_SLICE_PROFILE_ID].HasMember(JSON_ID)) {
        if (not attrData[JSON_UDM_SLICE_PROFILE_ID][JSON_ID].IsString()) {
          data.addError("Constraint Violation",
                        {{"resource_path", resourcePath},
                         {"description", data.notStringFieldError(JSON_ID)}});
        } else {
          udmStaticData.udmSliceProfileId.id =
              attrData[JSON_UDM_SLICE_PROFILE_ID][JSON_ID].GetString();

          if (udmStaticData.udmSliceProfileId.id.empty()) {
            data.addError("Constraint Violation",
                          {{"resource_path", resourcePath},
                           {"description",
                            data.emptyFieldError(JSON_UDM_SLICE_PROFILE_ID)}});
          }
        }
      } else {
        data.addError(
            "Constraint Violation",
            {{"resource_path", resourcePath},
             {"description", data.unfoundMandatoryFieldError(
                                 JSON_ID, JSON_UDM_SLICE_PROFILE_ID)}});
      }
    } else {
      data.addError("Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description",
                      data.notObjectFieldError(JSON_UDM_SLICE_PROFILE_ID)}});
    }
  } else {
    data.addError("Constraint Violation",
                  {{"resource_path", resourcePath},
                   {"description", data.unfoundMandatoryFieldError(
                                       JSON_UDM_SLICE_PROFILE_ID)}});
  }

  if (attrData.HasMember(JSON_STATIC_IP_ADDRESS)) {
    ValidatorVendorSpecificParser::getVendorSpecific(
        attrData[JSON_STATIC_IP_ADDRESS],
        udmStaticData.staticIpAddress.vendorSpecific);

    if (not attrData[JSON_STATIC_IP_ADDRESS][JSON_NSS_AI_DNN_IP_LISTS]
                .IsArray()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notArrayFieldError(JSON_NSS_AI_DNN_IP_LISTS)}});
    } else {
      for (auto& v : attrData[JSON_STATIC_IP_ADDRESS][JSON_NSS_AI_DNN_IP_LISTS]
                         .GetArray()) {
        entities::NssaiDnnIps nssaiDnnIps;
        entities::Snssai snssai;
        entities::DnnIpList_t ddnlist;
        ValidatorVendorSpecificParser::getVendorSpecific(
            v, nssaiDnnIps.vendorSpecific);
        for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin();
             itr != v.MemberEnd(); ++itr) {
          if (std::string(itr->name.GetString()).compare(JSON_NSSAI) == 0) {
            if (not itr->value.IsObject()) {
              data.addError(
                  "Constraint Violation",
                  {{"resource_path", resourcePath},
                   {"description", data.notObjectFieldError(JSON_NSSAI)}});
            } else {
              auto obj = itr->value.GetObject();
              ValidatorVendorSpecificParser::getVendorSpecific(
                  itr->value, snssai.vendorSpecific);
              if (obj.HasMember(JSON_SST)) {
                if (not obj[JSON_SST].IsInt()) {
                  data.addError(
                      "Constraint Violation",
                      {{"resource_path", resourcePath},
                       {"description", data.notIntFieldError(JSON_SST)}});
                } else {
                  snssai.sst = obj[JSON_SST].GetInt();
                }
              } else {
                data.addError("Constraint Violation",
                              {{"resource_path", resourcePath},
                               {"description", data.unfoundMandatoryFieldError(
                                                   JSON_SST, JSON_NSSAI)}});
              }
              if (obj.HasMember(JSON_SD)) {
                if (not obj[JSON_SD].IsString()) {
                  data.addError(
                      "Constraint Violation",
                      {{"resource_path", resourcePath},
                       {"description", data.notStringFieldError(JSON_SD)}});
                } else {
                  snssai.sd = obj[JSON_SD].GetString();
                }
              }
            }
          }

          if (std::string(itr->name.GetString()).compare(JSON_DNN_IP_LIST) ==
              0) {
            entities::dnn_t dnn;

            if (not itr->value.IsArray()) {
              data.addError(
                  "ConstraintViolation",
                  {{"resource_path", resourcePath},
                   {"description", data.notArrayFieldError(JSON_DNN_IP_LIST)}});
            } else {
              for (auto& w : itr->value.GetArray()) {
                entities::ip_addr_t ipaddress;
                if (not w.IsObject()) {
                  data.addError("Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notObjectFieldError(JSON_DNN_IP_LIST)}});
                } else {
                  auto obj = w.GetObject();
                  if (obj.HasMember(JSON_DNN)) {
                    if (not obj[JSON_DNN].IsString()) {
                      data.addError("Constraint Violation",
                                    {{"resource_path", resourcePath},
                                     {"description",
                                      data.notStringFieldError(JSON_DNN)}});
                    } else {
                      dnn = obj[JSON_DNN].GetString();
                    }
                  }
                  if (obj.HasMember(JSON_IPADDRESS)) {
                    if (not obj[JSON_IPADDRESS].IsArray()) {
                      data.addError("Constraint Violation",
                                    {{"resource_path", resourcePath},
                                     {"description", data.notArrayFieldError(
                                                         JSON_IPADDRESS)}});
                    } else {
                      for (auto& z : obj[JSON_IPADDRESS].GetArray()) {
                        entities::IpAddr ipElem;
                        auto var = z.GetObject();
                        ValidatorVendorSpecificParser::getVendorSpecific(
                            z, ipElem.vendorSpecific);
                        if (var.HasMember(JSON_IPV4_ADDR)) {
                          ipElem.ipType = entities::IpAddrType::IPv4;
                          if (not var[JSON_IPV4_ADDR].IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notStringFieldError(JSON_IPV4_ADDR)}});
                          } else {
                            ipElem.ip = var[JSON_IPV4_ADDR].GetString();
                            ipaddress.push_back(ipElem);
                          }
                        } else if (var.HasMember(JSON_IPV6_ADDR)) {
                          ipElem.ipType = entities::IpAddrType::IPv6;
                          if (not var[JSON_IPV6_ADDR].IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notStringFieldError(JSON_IPV6_ADDR)}});
                          } else {
                            ipElem.ip = var[JSON_IPV6_ADDR].GetString();
                            ipaddress.push_back(ipElem);
                          }
                        } else if (var.HasMember(JSON_IPV6_PREFIX)) {
                          ipElem.ipType = entities::IpAddrType::IPv6Prefix;
                          if (not var[JSON_IPV6_PREFIX].IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notStringFieldError(JSON_IPV6_PREFIX)}});
                          } else {
                            ipElem.ip = var[JSON_IPV6_PREFIX].GetString();
                            ipaddress.push_back(ipElem);
                          }
                        }
                      }
                    }
                  }
                  entities::DnnIps dnnIps;
                  ValidatorVendorSpecificParser::getVendorSpecific(
                      w, dnnIps.vendorSpecific);
                  dnnIps.dnn = dnn;
                  dnnIps.ipAddress = ipaddress;
                  ddnlist.push_back(dnnIps);
                }
              }
            }
          }
          nssaiDnnIps.snssai = snssai;
          nssaiDnnIps.dnnIpList = ddnlist;
        }
        udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(nssaiDnnIps);
      }
    }

    udmStaticData.fieldExistence.hasStaticIpAddress = true;
  } else {
    udmStaticData.fieldExistence.hasStaticIpAddress = false;
  }

  if (attrData.HasMember(JSON_ROAMING_ODB)) {
    if (not attrData[JSON_ROAMING_ODB].IsString()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notStringFieldError(JSON_ROAMING_ODB)}});
    } else {
      udmStaticData.roamingOdb = attrData[JSON_ROAMING_ODB].GetString();
      if (udmStaticData.roamingOdb.empty()) {
        data.addError(
            "Constraint Violation",
            {{"resource_path", resourcePath},
             {"description", data.emptyFieldError(JSON_ROAMING_ODB)}});
      }
    }
  }

  if (attrData.HasMember(JSON_UE_USAGE_TYPE)) {
    if (not attrData[JSON_UE_USAGE_TYPE].IsInt()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notIntFieldError(JSON_UE_USAGE_TYPE)}});
    } else {
      udmStaticData.ueUsageType = attrData[JSON_UE_USAGE_TYPE].GetInt();
      udmStaticData.fieldExistence.hasUeUsageType = true;
    }
  } else {
    udmStaticData.fieldExistence.hasUeUsageType = false;
  }

  if (attrData.HasMember(JSON_RFS_INDEX)) {
    if (not attrData[JSON_RFS_INDEX].IsInt()) {
      data.addError("Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description", data.notIntFieldError(JSON_RFS_INDEX)}});
    } else {
      udmStaticData.rfspIndex = attrData[JSON_RFS_INDEX].GetInt();
      udmStaticData.fieldExistence.hasRfspIndex = true;
    }
  } else {
    udmStaticData.fieldExistence.hasRfspIndex = false;
  }

  if (attrData.HasMember(JSON_SUBS_REG_TIMER)) {
    if (not attrData[JSON_SUBS_REG_TIMER].IsInt()) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.notIntFieldError(JSON_SUBS_REG_TIMER)}});
    } else {
      udmStaticData.subsRegTimer = attrData[JSON_SUBS_REG_TIMER].GetInt();
      udmStaticData.fieldExistence.hasSubsRegTimer = true;
    }
  } else {
    udmStaticData.fieldExistence.hasSubsRegTimer = false;
  }

  if (attrData.HasMember(JSON_UDM_SLICE_DATA)) {
    ValidatorVendorSpecificParser::getVendorSpecific(
        attrData[JSON_UDM_SLICE_DATA],
        udmStaticData.udmSliceData.vendorSpecific);

    if (attrData[JSON_UDM_SLICE_DATA].HasMember(JSON_PLMN_ID_SETS)) {
      if (attrData[JSON_UDM_SLICE_DATA][JSON_PLMN_ID_SETS].IsArray()) {
        udmStaticData.udmSliceData.plmnIdSets =
            getPlmnIdSets(attrData[JSON_UDM_SLICE_DATA][JSON_PLMN_ID_SETS],
                          data, resourcePath, /*isSliceData=*/true);
      } else {
        data.addError(
            "ConstraintViolation",
            {{"resource_path", resourcePath},
             {"description", data.notArrayFieldError(JSON_PLMN_ID_SETS)}});
      }
    }

    udmStaticData.fieldExistence.hasUdmSliceData = true;
  } else {
    udmStaticData.fieldExistence.hasUdmSliceData = false;
  }
}

void ValidatorRapidJsonParser::getProvJournal(
    const rapidjson::Value& attrData, entities::ProvJournal& provJournal) {
  provJournal.notifRef = getString(attrData, JSON_NOTIF_REF);

  provJournal.imsi = getString(attrData, JSON_IMSI);

  provJournal.imsiMask = getString(attrData, JSON_IMSI_MASK);

  provJournal.imsiExtMask = getString(attrData, JSON_IMSI_EXT_MASK);

  provJournal.msisdn = getString(attrData, JSON_MSISDN);

  provJournal.msisdnMask = getString(attrData, JSON_MSISDN_MASK);

  provJournal.msisdnExtMask = getString(attrData, JSON_MSISDN_EXT_MASK);

  provJournal.imsiAux = getString(attrData, JSON_IMSI_AUX);

  provJournal.imsiAuxMask = getString(attrData, JSON_IMSI_AUX_MASK);

  provJournal.imsiAuxExtMask = getString(attrData, JSON_IMSI_AUX_EXT_MASK);

  provJournal.impi = getString(attrData, JSON_IMPI);

  provJournal.impiMask = getString(attrData, JSON_IMPI_MASK);

  provJournal.impiExtMask = getString(attrData, JSON_IMPI_EXT_MASK);

  provJournal.secImpi = getString(attrData, JSON_SEC_IMPI);

  provJournal.impiAux = getString(attrData, JSON_IMPI_AUX);

  provJournal.username = getString(attrData, JSON_USERNAME);

  provJournal.usernameMask = getString(attrData, JSON_USERNAME_MASK);

  provJournal.usernameExtMask = getString(attrData, JSON_USERNAME_EXT_MASK);

  if (attrData.HasMember(JSON_IMSICHO_STATUS)) {
    if (attrData[JSON_IMSICHO_STATUS].IsInt()) {
      provJournal.imsiChoStatus = attrData[JSON_IMSICHO_STATUS].GetInt();
    }
  }

  provJournal.imsiExpiryDate = getString(attrData, JSON_IMSI_EXPIRY_DATE);

  provJournal.imsiChoExec = getString(attrData, JSON_IMSICHO_EXEC);

  if (attrData.HasMember(JSON_IMPUCHO_IDS)) {
    if (attrData[JSON_IMPUCHO_IDS].IsArray()) {
      for (auto& v : attrData[JSON_IMPUCHO_IDS].GetArray()) {
        if (v.IsString()) {
          provJournal.impuChoIds.push_back(v.GetString());
        }
      }
    }
  }

  provJournal.mscIdAux = getString(attrData, JSON_MSC_ID_AUX);

  provJournal.notifInfo = getString(attrData, JSON_NOTIF_INFO);

  provJournal.ueFunctionMask = getString(attrData, JSON_UE_FUNCTION_MASK);

  if (attrData.HasMember(JSON_EXT_ID_LIST)) {
    if (attrData[JSON_EXT_ID_LIST].IsArray()) {
      for (auto& v : attrData[JSON_EXT_ID_LIST].GetArray()) {
        if (v.IsString()) {
          provJournal.extIdList.push_back(v.GetString());
        }
      }
    }
  }

  provJournal.nai = getString(attrData, JSON_NAI);

  provJournal.naiMask = getString(attrData, JSON_NAI_MASK);

  provJournal.naiExtMask = getString(attrData, JSON_NAI_EXT_MASK);

  if (attrData.HasMember(JSON_SUBS_ID_LIST)) {
    if (attrData[JSON_SUBS_ID_LIST].IsArray()) {
      for (auto& v : attrData[JSON_SUBS_ID_LIST].GetArray()) {
        if (v.IsObject()) {
          entities::SubscriberIdentitiesId subsId;
          rapidjson::Value::ConstMemberIterator itr =
              v.FindMember(JSON_SUBS_ID);
          if (itr != v.MemberEnd()) {
            subsId.id = itr->value.GetString();
          }
          itr = v.FindMember(JSON_SUBS_PREFIX);
          if (itr != v.MemberEnd()) {
            subsId.prefix = itr->value.GetString();
          }
          provJournal.subsIdList.push_back(subsId);
        }
      }
    }
  }
}

void ValidatorRapidJsonParser::getRelatedResources(
    entities::ValidationData& data) {
  if (rJsonDoc.HasMember(JSON_RELATED_RESOURCES)) {
    rapidjson::Value& resourcesList = rJsonDoc[JSON_RELATED_RESOURCES];

    for (auto it = resourcesList.MemberBegin(); it != resourcesList.MemberEnd();
         ++it) {
      std::string resourcePath = it->name.GetString();

      std::size_t resourceProfile = resourcePath.find(JSON_UDM_SLICE_PROFILES);
      std::size_t resourceRoaming = resourcePath.find(JSON_UDM_ROAMING_AREAS);
      std::size_t smsStaticData = resourcePath.find(JSON_SMS_STATIC_DATA);

      if (resourcePath.ends_with(JSON_UDM_STATIC_DATA)) {
        entities::UdmStaticData udmStaticData;

        getUdmStaticData(it->value, udmStaticData, data, resourcePath);

        data.relatedResources.insert({resourcePath, udmStaticData});
      } else if (resourcePath.ends_with(JSON_PROV_JOURNAL)) {
        entities::ProvJournal provJournal;

        getProvJournal(it->value, provJournal);

        data.relatedResources.insert({resourcePath, provJournal});
      } else if (resourceProfile != std::string::npos) {
        entities::udm_slice_prof_t udmSliceProfile;

        getUdmSliceProfile(it->value, udmSliceProfile, data, resourcePath);

        data.relatedResources.insert({resourcePath, udmSliceProfile});
      } else if (resourceRoaming != std::string::npos) {
        entities::udm_roaming_area_t udmRoamingArea;

        getUdmRoamingAreas(it->value, udmRoamingArea, data, resourcePath);

        data.relatedResources.insert({resourcePath, udmRoamingArea});
      } else if (smsStaticData != std::string::npos) {
        entities::sms_static_data_t smsStaticData;
        ValidatorSmsSubsParser smsStaticDataParser(data, resourcePath);
        smsStaticDataParser.getOptionalSmsStaticData(it->value, smsStaticData);

        data.relatedResources.insert({resourcePath, smsStaticData});
      }
    }
  }
}

void ValidatorRapidJsonParser::getUdmSliceProfile(
    const rapidjson::Value& attrData,
    entities::udm_slice_prof_t& udmSliceProfile, entities::ValidationData& data,
    const std::string& resourcePath) {
  ValidatorVendorSpecificParser::getVendorSpecific(
      attrData, udmSliceProfile.vendorSpecific);

  if (attrData.HasMember(JSON_UDM_SLICE_PROFILE_ID)) {
    if (attrData[JSON_UDM_SLICE_PROFILE_ID].IsString()) {
      udmSliceProfile.udmSliceProfileId =
          attrData[JSON_UDM_SLICE_PROFILE_ID].GetString();
    } else {
      data.addError("Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description",
                      data.notStringFieldError(JSON_UDM_SLICE_PROFILE_ID)}});
    }
  }

  if (attrData.HasMember(JSON_PLMN_ID_SETS)) {
    if (attrData[JSON_PLMN_ID_SETS].IsArray()) {
      udmSliceProfile.plmnIdSets =
          getPlmnIdSets(attrData[JSON_PLMN_ID_SETS], data, resourcePath,
                        /*isSliceData=*/false);
    } else {
      setParsingError("data is not an Array");
    }
  }
}

entities::plmn_id_sets_t ValidatorRapidJsonParser::getPlmnIdSets(
    const rapidjson::Value& attrData, entities::ValidationData& data,
    const std::string& resourcePath, const bool& isSliceData) {
  entities::plmn_id_sets_t allPlmnIdSets;

  for (auto& v : attrData.GetArray()) {
    entities::PlmnIdSets plmnIdSet;
    auto obj = v.GetObject();
    ValidatorVendorSpecificParser::getVendorSpecific(v,
                                                     plmnIdSet.vendorSpecific);
    rapidjson::Value::ConstMemberIterator iterator =
        obj.FindMember(JSON_PLMN_ID_SET_IDENTIFIER);
    if (iterator != obj.MemberEnd()) {
      if (iterator->value.IsString()) {
        plmnIdSet.plmnIdSetIdentifier = iterator->value.GetString();
      } else {
        data.addError("Constraint Violation",
                      {{"resource_path", resourcePath},
                       {"description", data.notStringFieldError(
                                           JSON_PLMN_ID_SET_IDENTIFIER)}});
      }
    } else {
      data.addError("Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description", data.unfoundMandatoryFieldError(
                                         JSON_PLMN_ID_SET_IDENTIFIER)}});
    }

    rapidjson::Value::ConstMemberIterator iterator2 =
        obj.FindMember(JSON_NSSAI);
    plmnIdSet.fieldExistenceUdmSliceProfile.hasAnyNssai = false;
    if (iterator2 != obj.MemberEnd()) {
      plmnIdSet.fieldExistenceUdmSliceProfile.hasAnyNssai = true;
      if (iterator2->value.IsObject()) {
        auto ob = iterator2->value.GetObject();
        ValidatorVendorSpecificParser::getVendorSpecific(
            obj[JSON_NSSAI], plmnIdSet.nssai.vendorSpecific);
        if (ob.HasMember(JSON_DEFAULT_SINGLE_NSSAIS)) {
          if (not ob[JSON_DEFAULT_SINGLE_NSSAIS].IsArray()) {
            data.addError("Constraint Violation",
                          {{"resource_path", resourcePath},
                           {"description", data.notArrayFieldError(
                                               JSON_DEFAULT_SINGLE_NSSAIS)}});
          } else {
            for (auto& z : ob[JSON_DEFAULT_SINGLE_NSSAIS].GetArray()) {
              entities::NssaiDetails nssaiDetails;
              if (z.IsObject()) {
                auto var1 = z.GetObject();

                ValidatorVendorSpecificParser::getVendorSpecific(
                    z, nssaiDetails.vendorSpecific);

                if (var1.HasMember(JSON_DNN_LIST_ID)) {
                  if (not var1[JSON_DNN_LIST_ID].IsString()) {
                    data.addError("Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description", data.notStringFieldError(
                                                       JSON_DNN_LIST_ID)}});
                  } else {
                    nssaiDetails.dnnListId = var1[JSON_DNN_LIST_ID].GetString();
                  }
                }
                if (var1.HasMember(JSON_3GPP_CHARGING_CHARACTERISTICS)) {
                  if (not var1[JSON_3GPP_CHARGING_CHARACTERISTICS].IsString()) {
                    data.addError("Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description",
                                    data.notStringFieldError(
                                        JSON_3GPP_CHARGING_CHARACTERISTICS)}});
                  } else {
                    nssaiDetails.threegppChargingCharacteristics =
                        var1[JSON_3GPP_CHARGING_CHARACTERISTICS].GetString();
                  }
                }
                if (var1.HasMember(JSON_NSSAI)) {
                  if (not var1[JSON_NSSAI].IsObject()) {
                    data.addError("Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description",
                                    data.notObjectFieldError(JSON_NSSAI)}});
                  } else {
                    auto myobject = var1[JSON_NSSAI].GetObject();
                    ValidatorVendorSpecificParser::getVendorSpecific(
                        var1[JSON_NSSAI], nssaiDetails.nssai.vendorSpecific);
                    if (myobject.HasMember(JSON_SST)) {
                      if (not myobject[JSON_SST].IsInt()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description", data.notIntFieldError(JSON_SST)}});
                      } else {
                        nssaiDetails.nssai.sst = myobject[JSON_SST].GetInt();
                      }
                    } else {
                      data.addError(
                          "Constraint Violation",
                          {{"resource_path", resourcePath},
                           {"description", data.unfoundMandatoryFieldError(
                                               JSON_SST, JSON_NSSAI)}});
                    }
                    if (myobject.HasMember(JSON_SD)) {
                      if (not myobject[JSON_SD].IsString()) {
                        data.addError("Constraint Violation",
                                      {{"resource_path", resourcePath},
                                       {"description",
                                        data.notStringFieldError(JSON_SD)}});
                      } else {
                        nssaiDetails.nssai.sd = myobject[JSON_SD].GetString();
                      }
                    }
                  }
                } else {
                  data.addError(
                      "Constraint Violation",
                      {{"resource_path", resourcePath},
                       {"description",
                        data.unfoundMandatoryFieldError(
                            JSON_NSSAI, JSON_DEFAULT_SINGLE_NSSAIS)}});
                }

                plmnIdSet.nssai.defaultSingleNssais.push_back(nssaiDetails);
              } else {
                data.addError(
                    "Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description",
                      data.notObjectFieldError(JSON_DEFAULT_SINGLE_NSSAIS)}});
              }
            }
          }
        }

        if (ob.HasMember(JSON_SINGLE_NSSAIS)) {
          plmnIdSet.fieldExistenceUdmSliceProfile.hasNssai = true;
          if (not ob[JSON_SINGLE_NSSAIS].IsArray()) {
            data.addError(
                "Constraint Violation",
                {{"resource_path", resourcePath},
                 {"description", data.notArrayFieldError(JSON_SINGLE_NSSAIS)}});
          } else {
            for (auto& z : ob[JSON_SINGLE_NSSAIS].GetArray()) {
              entities::NssaiDetails nssaiDetails;
              if (z.IsObject()) {
                auto var2 = z.GetObject();
                ValidatorVendorSpecificParser::getVendorSpecific(
                    z, nssaiDetails.vendorSpecific);
                if (var2.HasMember(JSON_DNN_LIST_ID)) {
                  if (not var2[JSON_DNN_LIST_ID].IsString()) {
                    data.addError("Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description", data.notStringFieldError(
                                                       JSON_DNN_LIST_ID)}});
                  } else {
                    nssaiDetails.dnnListId = var2[JSON_DNN_LIST_ID].GetString();
                  }
                }
                if (var2.HasMember(JSON_3GPP_CHARGING_CHARACTERISTICS)) {
                  if (not var2[JSON_3GPP_CHARGING_CHARACTERISTICS].IsString()) {
                    data.addError("Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description",
                                    data.notStringFieldError(
                                        JSON_3GPP_CHARGING_CHARACTERISTICS)}});
                  } else {
                    nssaiDetails.threegppChargingCharacteristics =
                        var2[JSON_3GPP_CHARGING_CHARACTERISTICS].GetString();
                  }
                }
                if (var2.HasMember(JSON_NSSAI)) {
                  if (not var2[JSON_NSSAI].IsObject()) {
                    data.addError("Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description",
                                    data.notObjectFieldError(JSON_NSSAI)}});
                  } else {
                    auto myobject = var2[JSON_NSSAI].GetObject();
                    ValidatorVendorSpecificParser::getVendorSpecific(
                        var2[JSON_NSSAI], nssaiDetails.nssai.vendorSpecific);
                    if (myobject.HasMember(JSON_SST)) {
                      if (not myobject[JSON_SST].IsInt()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description", data.notIntFieldError(JSON_SST)}});
                      } else {
                        nssaiDetails.nssai.sst = myobject[JSON_SST].GetInt();
                      }
                    } else {
                      data.addError(
                          "Constraint Violation",
                          {{"resource_path", resourcePath},
                           {"description", data.unfoundMandatoryFieldError(
                                               JSON_SST, JSON_NSSAI)}});
                    }
                    if (myobject.HasMember(JSON_SD)) {
                      if (not myobject[JSON_SD].IsString()) {
                        data.addError("Constraint Violation",
                                      {{"resource_path", resourcePath},
                                       {"description",
                                        data.notStringFieldError(JSON_SD)}});
                      } else {
                        nssaiDetails.nssai.sd = myobject[JSON_SD].GetString();
                      }
                    }
                  }
                } else {
                  data.addError(
                      "Constraint Violation",
                      {{"resource_path", resourcePath},
                       {"description", data.unfoundMandatoryFieldError(
                                           JSON_NSSAI, JSON_SINGLE_NSSAIS)}});
                }

                plmnIdSet.nssai.singleNssais.push_back(nssaiDetails);
              } else {
                data.addError("Constraint Violation",
                              {{"resource_path", resourcePath},
                               {"description",
                                data.notObjectFieldError(JSON_SINGLE_NSSAIS)}});
              }
            }
          }
        } else {
          plmnIdSet.fieldExistenceUdmSliceProfile.hasNssai = false;
        }
      } else {
        data.addError("Constraint Violation",
                      {{"resource_path", resourcePath},
                       {"description", data.notObjectFieldError(JSON_NSSAI)}});
      }
    }

    if (plmnIdSet.fieldExistenceUdmSliceProfile.hasAnyNssai &&
        plmnIdSet.nssai.defaultSingleNssais.size() == 0) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description",
            "When \"" + std::string{JSON_NSSAI} +
                "\" is included, it must have at least one element in \"" +
                std::string{JSON_DEFAULT_SINGLE_NSSAIS} + "\""}});
    }

    rapidjson::Value::ConstMemberIterator iterator3 =
        obj.FindMember(JSON_DNN_LISTS);

    if (iterator3 != obj.MemberEnd()) {
      if (not iterator3->value.IsArray()) {
        data.addError(
            "Constraint Violation",
            {{"resource_path", resourcePath},
             {"description", data.notArrayFieldError(JSON_DNN_LISTS)}});
      } else {
        // check size of the array is at least one. Otherwise, addError
        // valid both for udmSliceData and slice profiles
        for (auto& z : iterator3->value.GetArray()) {
          entities::DnnList dnnList;
          if (z.IsObject()) {
            auto var3 = z.GetObject();
            ValidatorVendorSpecificParser::getVendorSpecific(
                z, dnnList.vendorSpecific);
            if (var3.HasMember(JSON_DNN_LIST_ID)) {
              if (not var3[JSON_DNN_LIST_ID].IsString()) {
                data.addError("Constraint Violation",
                              {{"resource_path", resourcePath},
                               {"description",
                                data.notStringFieldError(JSON_DNN_LIST_ID)}});
              } else {
                dnnList.dnnListId = var3[JSON_DNN_LIST_ID].GetString();
              }
            } else {
              data.addError("Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description", data.unfoundMandatoryFieldError(
                                                 JSON_DNN_LIST_ID)}});
            }
            if (var3.HasMember(JSON_DNN_DATA_LIST)) {
              if (not var3[JSON_DNN_DATA_LIST].IsArray()) {
                data.addError("Constraint Violation",
                              {{"resource_path", resourcePath},
                               {"description",
                                data.notArrayFieldError(JSON_DNN_DATA_LIST)}});
              } else {
                for (auto& b : var3[JSON_DNN_DATA_LIST].GetArray()) {
                  entities::DnnData dnnData;
                  if (b.IsObject()) {
                    auto var4 = b.GetObject();
                    ValidatorVendorSpecificParser::getVendorSpecific(
                        b, dnnData.vendorSpecific);
                    if (var4.HasMember(JSON_DNN)) {
                      if (not var4[JSON_DNN].IsString()) {
                        data.addError("Constraint Violation",
                                      {{"resource_path", resourcePath},
                                       {"description",
                                        data.notStringFieldError(JSON_DNN)}});
                      } else {
                        dnnData.dnn = var4[JSON_DNN].GetString();
                      }
                    }

                    if (var4.HasMember(JSON_DEFAULT_DNN_INDICATOR)) {
                      dnnData.fieldExistence.hasDefaultDnnIndicator = true;
                      if (not var4[JSON_DEFAULT_DNN_INDICATOR].IsBool()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description", data.notBoolFieldError(
                                                 JSON_DEFAULT_DNN_INDICATOR)}});
                      } else {
                        dnnData.defaultDnnIndicator =
                            var4[JSON_DEFAULT_DNN_INDICATOR].GetBool();
                      }
                    } else {
                      dnnData.fieldExistence.hasDefaultDnnIndicator = false;
                    }

                    if (var4.HasMember(JSON_LBO_ROAMING_ALLOWED)) {
                      dnnData.fieldExistence.hasLboRoamingAllowed = true;
                      if (not var4[JSON_LBO_ROAMING_ALLOWED].IsBool()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description", data.notBoolFieldError(
                                                 JSON_LBO_ROAMING_ALLOWED)}});
                      } else {
                        dnnData.lboRoamingAllowed =
                            var4[JSON_LBO_ROAMING_ALLOWED].GetBool();
                      }
                    } else {
                      dnnData.fieldExistence.hasLboRoamingAllowed = false;
                    }

                    if (var4.HasMember(JSON_IWK_EPS_IND)) {
                      dnnData.fieldExistence.hasIwkEpsInd = true;
                      if (not var4[JSON_IWK_EPS_IND].IsBool()) {
                        data.addError("Constraint Violation",
                                      {{"resource_path", resourcePath},
                                       {"description", data.notBoolFieldError(
                                                           JSON_IWK_EPS_IND)}});
                      } else {
                        dnnData.iwkEpsInd = var4[JSON_IWK_EPS_IND].GetBool();
                      }
                    } else {
                      dnnData.fieldExistence.hasIwkEpsInd = false;
                    }

                    if (var4.HasMember(JSON_LADN_INDICATOR)) {
                      dnnData.fieldExistence.hasLadnIndicator = true;
                      if (not var4[JSON_LADN_INDICATOR].IsBool()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description",
                              data.notBoolFieldError(JSON_LADN_INDICATOR)}});
                      } else {
                        dnnData.ladnIndicator =
                            var4[JSON_LADN_INDICATOR].GetBool();
                      }
                    } else {
                      dnnData.fieldExistence.hasLadnIndicator = false;
                    }

                    if (var4.HasMember(JSON_PDU_SESSION_TYPES)) {
                      dnnData.fieldExistence.hasPduSessionTypes = true;
                      if (not var4[JSON_PDU_SESSION_TYPES].IsObject()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description", data.notObjectFieldError(
                                                 JSON_PDU_SESSION_TYPES)}});
                      } else {
                        auto myobj = var4[JSON_PDU_SESSION_TYPES].GetObject();
                        ValidatorVendorSpecificParser::getVendorSpecific(
                            var4[JSON_PDU_SESSION_TYPES],
                            dnnData.pduSessionTypes.vendorSpecific);
                        if (myobj.HasMember(JSON_DEFAULT_SESSION_TYPE)) {
                          if (not myobj[JSON_DEFAULT_SESSION_TYPE].IsString()) {
                            data.addError("Constraint Violation",
                                          {{"resource_path", resourcePath},
                                           {"description",
                                            data.notStringFieldError(
                                                JSON_DEFAULT_SESSION_TYPE)}});
                          } else {
                            dnnData.pduSessionTypes.defaultSessionType =
                                myobj[JSON_DEFAULT_SESSION_TYPE].GetString();
                          }
                        }

                        if (myobj.HasMember(JSON_ALLOWED_SESSION_TYPES)) {
                          dnnData.fieldExistence.hasAllowedSessionTypes = true;
                          if (not myobj[JSON_ALLOWED_SESSION_TYPES].IsArray()) {
                            data.addError("Constraint Violation",
                                          {{"resource_path", resourcePath},
                                           {"description",
                                            data.notArrayFieldError(
                                                JSON_ALLOWED_SESSION_TYPES)}});
                          } else {
                            for (auto& c :
                                 myobj[JSON_ALLOWED_SESSION_TYPES].GetArray()) {
                              if (not c.IsString()) {
                                data.addError(
                                    "Constraint Violation",
                                    {{"resource_path", resourcePath},
                                     {"description",
                                      data.notStringFieldError(
                                          JSON_ALLOWED_SESSION_TYPES)}});
                              } else {
                                dnnData.pduSessionTypes.allowedSessionTypes
                                    .push_back(c.GetString());
                              }
                            }
                          }
                        } else {
                          dnnData.fieldExistence.hasAllowedSessionTypes = false;
                        }
                      }
                    } else {
                      dnnData.fieldExistence.hasPduSessionTypes = false;
                    }

                    if (var4.HasMember(JSON_SSC_MODES)) {
                      dnnData.fieldExistence.hasSscModes = true;
                      if (not var4[JSON_SSC_MODES].IsObject()) {
                        data.addError("Constraint Violation",
                                      {{"resource_path", resourcePath},
                                       {"description", data.notObjectFieldError(
                                                           JSON_SSC_MODES)}});
                      } else {
                        auto myobj = var4[JSON_SSC_MODES].GetObject();
                        ValidatorVendorSpecificParser::getVendorSpecific(
                            var4[JSON_SSC_MODES],
                            dnnData.sscModes.vendorSpecific);
                        if (myobj.HasMember(JSON_DEFAULT_SSC_MODE)) {
                          if (not myobj[JSON_DEFAULT_SSC_MODE].IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description", data.notStringFieldError(
                                                     JSON_DEFAULT_SSC_MODE)}});
                          } else {
                            dnnData.sscModes.defaultSscMode =
                                myobj[JSON_DEFAULT_SSC_MODE].GetString();
                          }
                        }

                        if (myobj.HasMember(JSON_ALLOWED_SSC_MODES)) {
                          if (not myobj[JSON_ALLOWED_SSC_MODES].IsArray()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description", data.notArrayFieldError(
                                                     JSON_ALLOWED_SSC_MODES)}});
                          } else {
                            for (auto& c :
                                 myobj[JSON_ALLOWED_SSC_MODES].GetArray()) {
                              if (not c.IsString()) {
                                data.addError("Constraint Violation",
                                              {{"resource_path", resourcePath},
                                               {"description",
                                                data.notStringFieldError(
                                                    JSON_ALLOWED_SSC_MODES)}});
                              } else {
                                dnnData.sscModes.allowedSscModes.push_back(
                                    c.GetString());
                              }
                            }
                          }
                        }
                      }
                    } else {
                      dnnData.fieldExistence.hasSscModes = false;
                    }

                    if (var4.HasMember(JSON_3GPP_CHARGING_CHARACTERISTICS)) {
                      if (not var4[JSON_3GPP_CHARGING_CHARACTERISTICS]
                                  .IsString()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description",
                              data.notStringFieldError(
                                  JSON_3GPP_CHARGING_CHARACTERISTICS)}});
                      } else {
                        dnnData.threegppChargingCharacteristics =
                            var4[JSON_3GPP_CHARGING_CHARACTERISTICS]
                                .GetString();
                      }
                    }

                    if (var4.HasMember(JSON_UP_SECURITY)) {
                      dnnData.fieldExistence.hasUpSecurity = true;
                      if (not var4[JSON_UP_SECURITY].IsObject()) {
                        data.addError("Constraint Violation",
                                      {{"resource_path", resourcePath},
                                       {"description", data.notObjectFieldError(
                                                           JSON_UP_SECURITY)}});
                      } else {
                        auto myobj = var4[JSON_UP_SECURITY].GetObject();
                        ValidatorVendorSpecificParser::getVendorSpecific(
                            var4[JSON_UP_SECURITY],
                            dnnData.upSecurity.vendorSpecific);
                        if (myobj.HasMember(JSON_UP_INTEGR)) {
                          if (not myobj[JSON_UP_INTEGR].IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notStringFieldError(JSON_UP_INTEGR)}});
                          } else {
                            dnnData.upSecurity.upIntegr =
                                myobj[JSON_UP_INTEGR].GetString();
                          }
                        }
                        if (myobj.HasMember(JSON_UP_CONFID)) {
                          if (not myobj[JSON_UP_CONFID].IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notStringFieldError(JSON_UP_CONFID)}});
                          } else {
                            dnnData.upSecurity.upConfid =
                                myobj[JSON_UP_CONFID].GetString();
                          }
                        }
                      }
                    } else {
                      dnnData.fieldExistence.hasUpSecurity = false;
                    }

                    if (var4.HasMember(JSON_SESSION_AMBR)) {
                      dnnData.fieldExistence.hasSessionAmbr = true;
                      if (not var4[JSON_SESSION_AMBR].IsObject()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description",
                              data.notObjectFieldError(JSON_SESSION_AMBR)}});
                      } else {
                        auto myobj = var4[JSON_SESSION_AMBR].GetObject();
                        ValidatorVendorSpecificParser::getVendorSpecific(
                            var4[JSON_SESSION_AMBR],
                            dnnData.sessionAmbr.vendorSpecific);
                        if (myobj.HasMember(JSON_UPLINK)) {
                          if (not myobj[JSON_UPLINK].IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notStringFieldError(JSON_UPLINK)}});
                          } else {
                            dnnData.sessionAmbr.uplink =
                                myobj[JSON_UPLINK].GetString();
                          }
                        } else {
                          data.addError(
                              "Constraint Violation",
                              {{"resource_path", resourcePath},
                               {"description",
                                data.unfoundMandatoryFieldError(
                                    JSON_UPLINK, JSON_SESSION_AMBR)}});
                        }
                        if (myobj.HasMember(JSON_DOWNLINK)) {
                          if (not myobj[JSON_DOWNLINK].IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notStringFieldError(JSON_DOWNLINK)}});
                          } else {
                            dnnData.sessionAmbr.downlink =
                                myobj[JSON_DOWNLINK].GetString();
                          }
                        } else {
                          data.addError(
                              "Constraint Violation",
                              {{"resource_path", resourcePath},
                               {"description",
                                data.unfoundMandatoryFieldError(
                                    JSON_DOWNLINK, JSON_SESSION_AMBR)}});
                        }
                      }
                    } else {
                      dnnData.fieldExistence.hasSessionAmbr = false;
                    }
                    if (var4.HasMember(JSON_SMF_LIST)) {
                      dnnData.fieldExistence.hasSmfList = true;
                      if (not var4[JSON_SMF_LIST].IsArray()) {
                        data.addError("Constraint Violation",
                                      {{"resource_path", resourcePath},
                                       {"description", data.notArrayFieldError(
                                                           JSON_SMF_LIST)}});
                      } else {
                        for (auto& v : var4[JSON_SMF_LIST].GetArray()) {
                          if (not v.IsString()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notStringFieldError(JSON_SMF_LIST)}});
                          } else {
                            dnnData.smfList.push_back(v.GetString());
                          }
                        }
                      }
                    } else {
                      dnnData.fieldExistence.hasSmfList = false;
                    }
                    if (var4.HasMember(JSON_SAME_SMF_IND)) {
                      dnnData.fieldExistence.hasSameSmfInd = true;
                      if (not var4[JSON_SAME_SMF_IND].IsBool()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description",
                              data.notBoolFieldError(JSON_SAME_SMF_IND)}});
                      } else {
                        dnnData.sameSmfInd = var4[JSON_SAME_SMF_IND].GetBool();
                      }
                    } else {
                      dnnData.fieldExistence.hasSameSmfInd = false;
                    }
                    if (var4.HasMember(JSON_5G_QOS_PROFILE)) {
                      dnnData.fieldExistence.hasFivegQosProfile = true;
                      if (not var4[JSON_5G_QOS_PROFILE].IsObject()) {
                        data.addError(
                            "Constraint Violation",
                            {{"resource_path", resourcePath},
                             {"description",
                              data.notObjectFieldError(JSON_5G_QOS_PROFILE)}});
                      } else {
                        auto myobj = var4[JSON_5G_QOS_PROFILE].GetObject();
                        ValidatorVendorSpecificParser::getVendorSpecific(
                            var4[JSON_5G_QOS_PROFILE],
                            dnnData.fivegQosProfile.vendorSpecific);
                        if (myobj.HasMember(JSON_5QI)) {
                          if (not myobj[JSON_5QI].IsInt()) {
                            data.addError("Constraint Violation",
                                          {{"resource_path", resourcePath},
                                           {"description",
                                            data.notIntFieldError(JSON_5QI)}});
                          } else {
                            dnnData.fivegQosProfile.fiveqi =
                                myobj[JSON_5QI].GetInt();
                          }
                        } else {
                          data.addError("Constraint Violation",
                                        {{"resource_path", resourcePath},
                                         {"description",
                                          data.unfoundMandatoryFieldError(
                                              JSON_5QI, JSON_5G_QOS_PROFILE)}});
                        }

                        if (myobj.HasMember(JSON_ARP)) {
                          if (not myobj[JSON_ARP].IsObject()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notObjectFieldError(JSON_ARP)}});
                          } else {
                            auto theobj = myobj[JSON_ARP].GetObject();
                            ValidatorVendorSpecificParser::getVendorSpecific(
                                myobj[JSON_ARP],
                                dnnData.fivegQosProfile.arp.vendorSpecific);
                            if (theobj.HasMember(JSON_PRIORITY_LEVEL)) {
                              if (not theobj[JSON_PRIORITY_LEVEL].IsInt()) {
                                data.addError("Constraint Violation",
                                              {{"resource_path", resourcePath},
                                               {"description",
                                                data.notIntFieldError(
                                                    JSON_PRIORITY_LEVEL)}});
                              } else {
                                dnnData.fivegQosProfile.arp.priorityLevel =
                                    theobj[JSON_PRIORITY_LEVEL].GetInt();
                              }
                            } else {
                              data.addError(
                                  "Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description",
                                    data.unfoundMandatoryFieldError(
                                        JSON_PRIORITY_LEVEL, JSON_ARP)}});
                            }
                            if (theobj.HasMember(JSON_PREEMPTCAP)) {
                              if (not theobj[JSON_PREEMPTCAP].IsString()) {
                                data.addError(
                                    "Constraint Violation",
                                    {{"resource_path", resourcePath},
                                     {"description", data.notStringFieldError(
                                                         JSON_PREEMPTCAP)}});
                              } else {
                                dnnData.fivegQosProfile.arp.preemptCap =
                                    theobj[JSON_PREEMPTCAP].GetString();
                              }
                            } else {
                              data.addError("Constraint Violation",
                                            {{"resource_path", resourcePath},
                                             {"description",
                                              data.unfoundMandatoryFieldError(
                                                  JSON_PREEMPTCAP, JSON_ARP)}});
                            }

                            if (theobj.HasMember(JSON_PREEMPTVULN)) {
                              if (not theobj[JSON_PREEMPTVULN].IsString()) {
                                data.addError(
                                    "Constraint Violation",
                                    {{"resource_path", resourcePath},
                                     {"description", data.notStringFieldError(
                                                         JSON_PREEMPTVULN)}});
                              } else {
                                dnnData.fivegQosProfile.arp.preemptVuln =
                                    theobj[JSON_PREEMPTVULN].GetString();
                              }
                            } else {
                              data.addError(
                                  "Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description",
                                    data.unfoundMandatoryFieldError(
                                        JSON_PREEMPTVULN, JSON_ARP)}});
                            }
                          }
                        } else {
                          data.addError("Constraint Violation",
                                        {{"resource_path", resourcePath},
                                         {"description",
                                          data.unfoundMandatoryFieldError(
                                              JSON_ARP, JSON_5G_QOS_PROFILE)}});
                        }

                        if (myobj.HasMember(JSON_PRIORITY_LEVEL)) {
                          dnnData.fivegQosProfile.fieldExistence
                              .hasPriorityLevel = true;
                          if (not myobj[JSON_PRIORITY_LEVEL].IsInt()) {
                            data.addError(
                                "Constraint Violation",
                                {{"resource_path", resourcePath},
                                 {"description",
                                  data.notIntFieldError(JSON_PRIORITY_LEVEL)}});
                          } else {
                            dnnData.fivegQosProfile.priorityLevel =
                                myobj[JSON_PRIORITY_LEVEL].GetInt();
                          }
                        } else {
                          dnnData.fivegQosProfile.fieldExistence
                              .hasPriorityLevel = false;
                        }
                      }
                    } else {
                      dnnData.fieldExistence.hasFivegQosProfile = false;
                    }
                    dnnList.dnnDataList.push_back(dnnData);
                  } else {
                    data.addError("Constraint Violation",
                                  {{"resource_path", resourcePath},
                                   {"description", data.notObjectFieldError(
                                                       JSON_DNN_DATA_LIST)}});
                  }
                }
              }
            }

            plmnIdSet.dnnLists.push_back(dnnList);
          } else {
            data.addError(
                "Constraint Violation",
                {{"resource_path", resourcePath},
                 {"description", data.notObjectFieldError(JSON_DNN_LISTS)}});
          }
        }
      }
    } else if (not isSliceData) {
      data.addError(
          "Constraint Violation",
          {{"resource_path", resourcePath},
           {"description", data.unfoundMandatoryFieldError(JSON_DNN_LISTS)}});
    }
    allPlmnIdSets.push_back(plmnIdSet);
  }
  return allPlmnIdSets;
}

void ValidatorRapidJsonParser::getUdmRoamingAreas(
    const rapidjson::Value& attrData,
    entities::udm_roaming_area_t& udmRoamingAreas,
    entities::ValidationData& data, const std::string& resourcePath) {
  ValidatorVendorSpecificParser::getVendorSpecific(
      attrData, udmRoamingAreas.vendorSpecific);

  if (attrData.HasMember(JSON_UDM_ROAMING_AREA_ID)) {
    if (attrData[JSON_UDM_ROAMING_AREA_ID].IsString()) {
      udmRoamingAreas.udmRoamingAreaId =
          attrData[JSON_UDM_ROAMING_AREA_ID].GetString();
    } else {
      data.addError("Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description",
                      data.notStringFieldError(JSON_UDM_ROAMING_AREA_ID)}});
    }
  }

  if (attrData.HasMember(JSON_AREA_LIST)) {
    if (attrData[JSON_AREA_LIST].IsArray()) {
      entities::area_list_array_t areaList;
      for (auto& v : attrData[JSON_AREA_LIST].GetArray()) {
        entities::area_list_t areaObj;
        if (v.IsObject()) {
          auto obj = v.GetObject();
          ValidatorVendorSpecificParser::getVendorSpecific(
              v, areaObj.vendorSpecific);
          rapidjson::Value::ConstMemberIterator iterator =
              obj.FindMember(JSON_AREA_CODE);
          if (iterator != obj.MemberEnd()) {
            if (iterator->value.IsString()) {
              areaObj.area = iterator->value.GetString();
            } else {
              data.addError(
                  "Constraint Violation",
                  {{"resource_path", resourcePath},
                   {"description", data.notStringFieldError(JSON_AREA_CODE)}});
            }
          } else {
            rapidjson::Value::ConstMemberIterator itr2 =
                obj.FindMember(JSON_TACS);
            if (itr2 != obj.MemberEnd()) {
              if (itr2->value.IsArray()) {
                entities::tacs_t tacs;
                for (auto& elem : itr2->value.GetArray()) {
                  if (elem.IsString()) {
                    tacs.push_back(elem.GetString());
                  } else {
                    data.addError(
                        "Constraint Violation",
                        {{"resource_path", resourcePath},
                         {"description", data.notStringFieldError(JSON_TACS)}});
                  }
                }
                areaObj.area = tacs;
              } else {
                data.addError(
                    "Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description", data.notArrayFieldError(JSON_TACS)}});
              }
            } else {
              data.addError(
                  "Constraint Violation",
                  {{"resource_path", resourcePath},
                   {"description",
                    "Mandatory fields:[" + std::string(JSON_AREA_CODE) +
                        "] or [" + std::string(JSON_TACS) + "] not found"}});
            }
          }
          areaList.push_back(areaObj);
        } else {
          data.addError(
              "Constraint Violation",
              {{"resource_path", resourcePath},
               {"description", data.notObjectFieldError(JSON_AREA_LIST)}});
        }
      }
      udmRoamingAreas.areaList = areaList;
    } else {
      data.addError("Constraint Violation",
                    {{"resource_path", resourcePath},
                     {"description", data.notArrayFieldError(JSON_AREA_LIST)}});
    }
  }
}

std::string ValidatorRapidJsonParser::getString(
    const rapidjson::Value& attrData, const char* key) {
  if (attrData.HasMember(key)) {
    if (attrData[key].IsString()) {
      return attrData[key].GetString();
    }
  }

  return {};
}

}  // namespace json
}  // namespace secondary
}  // namespace port
