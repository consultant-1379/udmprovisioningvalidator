#include "StrDocumentConverter.hpp"
#include "boost/variant.hpp"
#include "entities/types.hpp"
#include "gtest/gtest.h"
#include "ports/json/ValidatorRapidJsonParser.hpp"
#include "ports/ports.hpp"

TEST(ValidatorRapidJsonParserTest, ParseOneChange) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"path_to_"
      "resource_to_change\"}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "path_to_resource_to_change");
}

TEST(ValidatorRapidJsonParserTest, ParseSeveralChanges) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"resource1\"}"
      ",{"
      "\"operation\":\"UPDATE\", "
      "\"resource_path\":\"resource2\"},{\"operation\":\"DELETE\", "
      "\"resource_path\":\"resource3\"}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 3);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "resource1");
  EXPECT_EQ(record.changes[1].operation, "UPDATE");
  EXPECT_EQ(record.changes[1].resourcePath, "resource2");
  EXPECT_EQ(record.changes[2].operation, "DELETE");
  EXPECT_EQ(record.changes[2].resourcePath, "resource3");
}

TEST(ValidatorRapidJsonParserTest, EmptyOperation) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"\",\"resource_path\":\"path_to_"
      "resource_to_change\"}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), false);

  EXPECT_EQ(parser.error(), true);
  EXPECT_EQ(parser.errorString(), "Field:[operation] is empty");
}

TEST(ValidatorRapidJsonParserTest, EmptyResourcePath) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"\"}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), false);

  EXPECT_EQ(parser.error(), true);
  EXPECT_EQ(parser.errorString(), "Field:[resource_path] is empty");
}

TEST(ValidatorRapidJsonParserTest, ChangesIsNotArray) {
  std::string jsonString(
      "{\"changes\":{\"operation\":\"CREATE\",\"resource_path\":\"path_to_"
      "resource_to_change\"}}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), false);

  EXPECT_EQ(parser.error(), true);
  EXPECT_EQ(parser.errorString(), "changes is not a list");
}

TEST(ValidatorRapidJsonParserTest, WrongJsonFormatDueToOneChangeIsNotAnObject) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"resource1\"}"
      ",{"
      "\"operation\":\"UPDATE\", "
      "\"resource_path\":\"resource2\"},\"operation\":\"DELETE\", "
      "\"resource_path\":\"resource3\"]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), true);
  EXPECT_EQ(parser.errorString(), "wrong json format");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticData) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "udmStaticData\",\"data\":{\"subscribedUeAmbr\":{\"uplink\":\"1 "
      "Gbps\",\"downlink\":\"2 "
      "Gbps\"},\"udmSliceProfileId\":{\"id\":\"2\"},\"ratRestrictions\":["
      "\"NR\","
      "\"WLAN\"]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.subscribedUeAmbr.uplink, "1 Gbps");
  EXPECT_EQ(record.changes[0].udmStaticData.subscribedUeAmbr.downlink,
            "2 Gbps");
  EXPECT_EQ(record.changes[0].udmStaticData.ratRestrictions.size(), 2);
  EXPECT_EQ(record.changes[0].udmStaticData.ratRestrictions[0], "NR");
  EXPECT_EQ(record.changes[0].udmStaticData.ratRestrictions[1], "WLAN");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataForbiddenAreas) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"forbiddenAreas\":[{\"id\":\"FB1\"}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.forbiddenAreas.size(), 1);
  EXPECT_EQ(record.changes[0].udmStaticData.forbiddenAreas[0].id, "FB1");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataServiceAreaRestriction) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"serviceAreaRestriction\":{\"restrictionType\":\"ALLOWED_AREAS\","
      "\"areas\":[{\"id\":\"AREA1\"},{\"id\":\"AREA2\"}],\"maxNumOfTAs\":2}}}"
      "]"
      "}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(
      record.changes[0].udmStaticData.serviceAreaRestriction.restrictionType,
      "ALLOWED_AREAS");
  EXPECT_EQ(record.changes[0].udmStaticData.serviceAreaRestriction.areas.size(),
            2);
  EXPECT_EQ(record.changes[0].udmStaticData.serviceAreaRestriction.areas[0].id,
            "AREA1");
  EXPECT_EQ(record.changes[0].udmStaticData.serviceAreaRestriction.areas[1].id,
            "AREA2");
  EXPECT_EQ(record.changes[0].udmStaticData.serviceAreaRestriction.maxNumOfTas,
            2);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataCoreNetworkType) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"coreNetworkTypeRestrictions\":[\"5GC\",\"EPC\"]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.coreNetworkTypeRestrictions.size(),
            2);
  EXPECT_EQ(record.changes[0].udmStaticData.coreNetworkTypeRestrictions[0],
            "5GC");
  EXPECT_EQ(record.changes[0].udmStaticData.coreNetworkTypeRestrictions[1],
            "EPC");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataMpsPriority) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"mpsPriority\": true}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.mpsPriority, true);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataMcsPriority) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"mcsPriority\": true}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.mcsPriority, true);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataDlPacketCount) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"dlPacketCount\": 3}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.dlPacketCount, 3);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataMicoAllowed) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"micoAllowed\": true}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.micoAllowed, true);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataStaticIpAddress) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"staticIpAddress\":{\"nssaiDnnIpLists\":[{\"nssai\":{\"sst\":22,"
      "\"sd\":"
      "\"11\"},\"dnnIpList\":[{\"dnn\":\"1\",\"ipAddress\":[{\"ipv4Addr\":"
      "\"11\"},{\"ipv6Addr\":\"22\"},"
      "{\"ipv6Prefix\":\"33\"}]}]},{\"nssai\":{\"sst\":33,\"sd\":\"33\"},"
      "\"dnnIpList\":[{\"dnn\":"
      "\"4\",\"ipAddress\":[{\"ipv4Addr\":\"1211\"},{\"ipv6Addr\":\"1222\"},{"
      "\"ipv6Prefix\":\"1133\"}]}]}]}}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(
      record.changes[0].udmStaticData.staticIpAddress.nssaiDnnIpLists.size(),
      2);
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.sst,
            22);
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.sd,
            "11");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .dnn,
            "1");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .ipAddress.size(),
            3);
  std::string str = boost::get<entities::ipv4_t>(
      record.changes[0]
          .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
          .dnnIpList[0]
          .ipAddress[0]
          .ip);
  EXPECT_EQ(str, "11");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .ipAddress[0]
                .ipType,
            entities::IpAddrType::IPv4);
  str = boost::get<entities::ipv6_t>(
      record.changes[0]
          .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
          .dnnIpList[0]
          .ipAddress[1]
          .ip);
  EXPECT_EQ(str, "22");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .ipAddress[1]
                .ipType,
            entities::IpAddrType::IPv6);
  str = boost::get<entities::ipv6_prefix_t>(
      record.changes[0]
          .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
          .dnnIpList[0]
          .ipAddress[2]
          .ip);
  EXPECT_EQ(str, "33");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .ipAddress[2]
                .ipType,
            entities::IpAddrType::IPv6Prefix);
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[1]
                .snssai.sst,
            33);
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[1]
                .snssai.sd,
            "33");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[1]
                .dnnIpList[0]
                .dnn,
            "4");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[1]
                .dnnIpList[0]
                .ipAddress.size(),
            3);
  str = boost::get<entities::ipv4_t>(
      record.changes[0]
          .udmStaticData.staticIpAddress.nssaiDnnIpLists[1]
          .dnnIpList[0]
          .ipAddress[0]
          .ip);
  EXPECT_EQ(str, "1211");
  str = boost::get<entities::ipv6_t>(
      record.changes[0]
          .udmStaticData.staticIpAddress.nssaiDnnIpLists[1]
          .dnnIpList[0]
          .ipAddress[1]
          .ip);
  EXPECT_EQ(str, "1222");
  str = boost::get<entities::ipv6_prefix_t>(
      record.changes[0]
          .udmStaticData.staticIpAddress.nssaiDnnIpLists[1]
          .dnnIpList[0]
          .ipAddress[2]
          .ip);
  EXPECT_EQ(str, "1133");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataRoamingOdb) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"roamingOdb\":\"OUTSIDE_HOME_PLMN\"}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.roamingOdb, "OUTSIDE_HOME_PLMN");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataOdbPacketServices) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"}"
      "}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataUeUsageType) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"ueUsageType\":5}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.ueUsageType, 5);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataRfspIndex) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"rfspIndex\":5}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.rfspIndex, 5);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataSubsRegTimer) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"subsRegTimer\":5}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.subsRegTimer, 5);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataSubsNoUdmSliceProfileId) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"subsRegTimer\":5}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[udmSliceProfileId] not found");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataUdmSliceData) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"udmSliceData\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"12345\"},{"
      "\"plmnIdSetIdentifier\":\"12346\"}]}}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.udmSliceData.plmnIdSets[0]
                .plmnIdSetIdentifier,
            "12345");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.udmSliceData.plmnIdSets[1]
                .plmnIdSetIdentifier,
            "12346");
}

TEST(ValidatorRapidJsonParserTest, ParseRelatedResources) {
  std::string jsonString(
      "{\"relatedResources\":{\"/subscribers/2208a/udmSubscription/"
      "udmStaticAndRegistrationData/"
      "udmStaticData\":{\"udmSliceProfileId\":{\"id\":\"1\"},"
      "\"subscribedUeAmbr\":{"
      "\"uplink\":\"1 "
      "Gbps\",\"downlink\":\"2 "
      "Gbps\"},\"ratRestrictions\":[\"NR\",\"WLAN\"]},\"/subscribers/3319b/"
      "udmSubscription/"
      "udmStaticAndRegistrationData/"
      "udmStaticData\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"subscribedUeAmbr\":{"
      "\"uplink\":\"3 "
      "Gbps\",\"downlink\":\"4 "
      "Gbps\"},\"ratRestrictions\":[\"EUTRA\",\"VIRTUAL\"]},\"/subscribers/"
      "3319b/journal/provJournal\":{\"notifRef\":\"my_notif\"}}}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 0);
  EXPECT_EQ(record.relatedResources.size(), 3);

  entities::UdmStaticData& udmStaticData =
      boost::get<entities::UdmStaticData>(record.relatedResources.at(
          "/subscribers/2208a/udmSubscription/udmStaticAndRegistrationData/"
          "udmStaticData"));
  EXPECT_EQ(udmStaticData.udmSliceProfileId.id, "1");
  EXPECT_EQ(udmStaticData.subscribedUeAmbr.uplink, "1 Gbps");
  EXPECT_EQ(udmStaticData.subscribedUeAmbr.downlink, "2 Gbps");
  EXPECT_EQ(udmStaticData.ratRestrictions.size(), 2);
  EXPECT_EQ(udmStaticData.ratRestrictions[0], "NR");
  EXPECT_EQ(udmStaticData.ratRestrictions[1], "WLAN");

  entities::UdmStaticData& udmStaticData2 =
      boost::get<entities::UdmStaticData>(record.relatedResources.at(
          "/subscribers/3319b/udmSubscription/udmStaticAndRegistrationData/"
          "udmStaticData"));
  EXPECT_EQ(udmStaticData2.udmSliceProfileId.id, "2");
  EXPECT_EQ(udmStaticData2.subscribedUeAmbr.uplink, "3 Gbps");
  EXPECT_EQ(udmStaticData2.subscribedUeAmbr.downlink, "4 Gbps");
  EXPECT_EQ(udmStaticData2.ratRestrictions.size(), 2);
  EXPECT_EQ(udmStaticData2.ratRestrictions[0], "EUTRA");
  EXPECT_EQ(udmStaticData2.ratRestrictions[1], "VIRTUAL");

  entities::ProvJournal& provJournal = boost::get<entities::ProvJournal>(
      record.relatedResources.at("/subscribers/3319b/journal/provJournal"));
  EXPECT_EQ(provJournal.notifRef, "my_notif");
}

TEST(ValidatorRapidJsonParserTest, ParseSeveralErrors) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"subsRegTimer\":5,\"subscribedUeAmbr\":{"
      "\"uplink\":\"1 Gbps\"}}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 2);
  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "Mandatory field:[udmSliceProfileId] not found");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "udmStaticData");

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "Mandatory field:[downlink] not found as child of:[subscribedUeAmbr]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmStaticDataSubsEmptyUdmSliceProfileId) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":\"\"}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[udmSliceProfileId] is not an object");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest, ParseProvJournalRelatedResources) {
  std::string jsonString(
      "{\"relatedResources\":{\"/subscribers/2208a/journal/"
      "provJournal\":{\"notifRef\":\"notifRef1\",\"imsi\":\"imsi1\","
      "\"imsiMask\":\"imsiMask1\",\"imsiExtMask\":\"imsiExtMask1\",\"msisdn\":"
      "\"msisdn1\",\"msisdnMask\":\"msisdnMask1\",\"msisdnExtMask\":"
      "\"msisdnExtMask1\",\"imsiAux\":\"imsiAux1\",\"imsiAuxMask\":"
      "\"imsiAuxMask1\",\"imsiAuxExtMask\":\"imsiAuxExtMask1\",\"impi\":"
      "\"impi1\",\"impiMask\":\"impiMask1\",\"impiExtMask\":\"impiExtMask1\","
      "\"secImpi\":\"secImpi1\",\"impiAux\":\"impiAux1\",\"username\":"
      "\"username1\",\"usernameMask\":\"usernameMask1\",\"usernameExtMask\":"
      "\"usernameExtMask1\"},\"/subscribers/3319b/journal/"
      "provJournal\":{\"imsiChoStatus\":2,\"imsiExpiryDate\":\"expiryDate2\","
      "\"imsiChoExec\":\"exec2\",\"impuChoIds\":[\"impuchoid1\","
      "\"impuchoid2\"],\"mscIdAux\":\"mscIdAux2\",\"notifInfo\":\"notifInfo2\","
      "\"ueFunctionMask\":\"ueFunctionMask2\",\"extIdList\":[\"extId1\","
      "\"extId2\"],\"nai\":\"nai2\",\"naiMask\":\"naiMask2\",\"naiExtMask\":"
      "\"naiExtMask2\",\"subsIdList\":[{\"id\":\"i3\",\"prefix\":\"p3\"},{\"id"
      "\":\"i4\"}]}}}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 0);
  EXPECT_EQ(record.relatedResources.size(), 2);

  entities::ProvJournal& journal1 = boost::get<entities::ProvJournal>(
      record.relatedResources.at("/subscribers/2208a/journal/provJournal"));
  EXPECT_EQ(journal1.notifRef, "notifRef1");
  EXPECT_EQ(journal1.imsi, "imsi1");
  EXPECT_EQ(journal1.imsiMask, "imsiMask1");
  EXPECT_EQ(journal1.imsiExtMask, "imsiExtMask1");
  EXPECT_EQ(journal1.msisdn, "msisdn1");
  EXPECT_EQ(journal1.msisdnMask, "msisdnMask1");
  EXPECT_EQ(journal1.msisdnExtMask, "msisdnExtMask1");
  EXPECT_EQ(journal1.imsiAux, "imsiAux1");
  EXPECT_EQ(journal1.imsiAuxMask, "imsiAuxMask1");
  EXPECT_EQ(journal1.imsiAuxExtMask, "imsiAuxExtMask1");
  EXPECT_EQ(journal1.impi, "impi1");
  EXPECT_EQ(journal1.impiMask, "impiMask1");
  EXPECT_EQ(journal1.impiExtMask, "impiExtMask1");
  EXPECT_EQ(journal1.secImpi, "secImpi1");
  EXPECT_EQ(journal1.impiAux, "impiAux1");
  EXPECT_EQ(journal1.username, "username1");
  EXPECT_EQ(journal1.usernameMask, "usernameMask1");
  EXPECT_EQ(journal1.usernameExtMask, "usernameExtMask1");

  entities::ProvJournal& journal2 = boost::get<entities::ProvJournal>(
      record.relatedResources.at("/subscribers/3319b/journal/provJournal"));
  EXPECT_EQ(journal2.notifRef, "");
  EXPECT_EQ(journal2.imsiChoStatus, 2);
  EXPECT_EQ(journal2.imsiExpiryDate, "expiryDate2");
  EXPECT_EQ(journal2.imsiChoExec, "exec2");
  EXPECT_EQ(journal2.impuChoIds.size(), 2);
  EXPECT_EQ(journal2.impuChoIds[0], "impuchoid1");
  EXPECT_EQ(journal2.impuChoIds[1], "impuchoid2");
  EXPECT_EQ(journal2.mscIdAux, "mscIdAux2");
  EXPECT_EQ(journal2.notifInfo, "notifInfo2");
  EXPECT_EQ(journal2.ueFunctionMask, "ueFunctionMask2");
  EXPECT_EQ(journal2.extIdList.size(), 2);
  EXPECT_EQ(journal2.extIdList[0], "extId1");
  EXPECT_EQ(journal2.extIdList[1], "extId2");
  EXPECT_EQ(journal2.nai, "nai2");
  EXPECT_EQ(journal2.naiMask, "naiMask2");
  EXPECT_EQ(journal2.naiExtMask, "naiExtMask2");
  EXPECT_EQ(journal2.subsIdList.size(), 2);
  EXPECT_EQ(journal2.subsIdList[0].id, "i3");
  EXPECT_EQ(journal2.subsIdList[0].prefix, "p3");
  EXPECT_EQ(journal2.subsIdList[1].id, "i4");
  EXPECT_EQ(journal2.subsIdList[1].prefix, "");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmStaticDataSubscribedUeAmbrIsNotObject) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"subscribedUeAmbr\":\"\","
      "\"udmSliceProfileId\":{\"id\":\"2\"}}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[subscribedUeAmbr] is not an object");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmStaticDataRatRestrictionsIsNotArray) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"ratRestrictions\":"
      "\"NR\"}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[ratRestrictions] is not an array");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataUeUsageTypeIsNotInt) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"ueUsageType\":\"5\"}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[ueUsageType] is not integer");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataMcsPriorityIsNotBoolean) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"mcsPriority\": 4}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[mcsPriority] is not boolean");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmStaticDataServiceAreaRestrictionAreasIsNotArray) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"serviceAreaRestriction\":{\"restrictionType\":\"ALLOWED_AREAS\","
      "\"areas\":\"AREA1\",\"maxNumOfTAs\":2}}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[areas] is not an array");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmStaticDataStaticIpAddressIsNotArray) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"staticIpAddress\":{\"nssaiDnnIpLists\":[{\"nssai\":{\"sst\":22,"
      "\"sd\":"
      "\"11\"},\"dnnIpList\":[{\"dnn\":\"1\",\"ipAddress\":[{\"ipv4Addr\":"
      "\"11\"},{\"ipv6Addr\":\"22\"},"
      "{\"ipv6Prefix\":\"33\"}]}]},{\"nssai\":{\"sst\":33,\"sd\":\"33\"},"
      "\"dnnIpList\":[{\"dnn\":"
      "\"4\",\"ipAddress\":\"1211\"}]}]}}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[ipAddress] is not an array");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "udmStaticData");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfile) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"/"
      "profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      ",\"data\":{\"udmSliceProfileId\":\"profile1\",\"plmnIdSets\":[{"
      "\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{"
      "\"dnnListId\":"
      "\"33\", "
      "\"3gppChargingCharacteristics\": \"3gppcharging\", "
      "\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", "
      "\"preemptVuln\":\"NOT_PREEMPTABLE\"},\"priorityLevel\":101},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", "
      "\"upConfid\":\"PREFERRED\"}, "
      "\"smfList\":[\"smfList1\",\"smfList2\"],"
      "\"sameSmfInd\":true"
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(parser.error(), false);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath,
            "/profiles/udmProfiles/udmSliceProfiles/2222");
  EXPECT_EQ(record.changes[0].udmSliceProfile.udmSliceProfileId, "profile1");
  EXPECT_EQ(record.changes[0].udmSliceProfile.plmnIdSets[0].plmnIdSetIdentifier,
            "234");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "22");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .threegppChargingCharacteristics,
            "charging");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            3);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "2");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .dnnListId,
            "33");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .threegppChargingCharacteristics,
            "3gppcharging");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sst,
            5);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sd,
            "33");
  EXPECT_EQ(
      record.changes[0].udmSliceProfile.plmnIdSets[0].dnnLists[0].dnnListId,
      "222");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .dnn,
            "1");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .ladnIndicator,
            false);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasLadnIndicator,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .defaultDnnIndicator,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasDefaultDnnIndicator,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .lboRoamingAllowed,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasLboRoamingAllowed,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .iwkEpsInd,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasIwkEpsInd,
            true);

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasPduSessionTypes,
            true);

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV4");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[1],
            "IPV6");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasSscModes,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_1");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasFivegQosProfile,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            2);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            3);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            101);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fieldExistence.hasPriorityLevel,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasSessionAmbr,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "10 Gbps");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "1 Gbps");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasUpSecurity,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .threegppChargingCharacteristics,
            "charging");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasSmfList,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fieldExistence.hasSameSmfInd,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .smfList[0],
            "smfList1");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sameSmfInd,
            true);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfiles) {
  std::string jsonDefaultSingleNssaisElem1(
      "{\"nssai\":{\"sst\":111,\"sd\":\"AAAA\"},"
      "\"dnnListId\":\"ThisIsDnnListId1\"}");

  std::string jsonDefaultSingleNssaisElem2(
      "{\"nssai\":{\"sst\":222,\"sd\":\"BBBB\"},"
      "\"dnnListId\":\"ThisIsDnnListId2\"}");

  std::string jsonDefaultSingleNssaisArray("[" + jsonDefaultSingleNssaisElem1 +
                                           "," + jsonDefaultSingleNssaisElem2 +
                                           "]");

  std::string jsonSingleNssaisElem1(
      "{\"nssai\":{\"sst\":333,\"sd\":\"CCCC\"},"
      "\"dnnListId\":\"ThisIsDnnListId3\"}");

  std::string jsonSingleNssaisElem2(
      "{\"nssai\":{\"sst\":444,\"sd\":\"DDDD\"},"
      "\"dnnListId\":\"ThisIsDnnListId4\"}");

  std::string jsonSingleNssaisArray("[" + jsonSingleNssaisElem1 + "," +
                                    jsonSingleNssaisElem2 + "]");

  std::string jsonNssaiObject(
      "{\"defaultSingleNssais\":" + jsonDefaultSingleNssaisArray + "," +
      "\"singleNssais\":" + jsonSingleNssaisArray + "}");

  std::string jsonDnnDataElem1(
      "{\"dnn\":\"ThisIsDnn1\","
      "\"defaultDnnIndicator\":true,"
      "\"lboRoamingAllowed\":true,"
      "\"iwkEpsInd\":true,"
      "\"ladnIndicator\":true,"
      "\"pduSessionTypes\":{\"defaultSessionType\":\"IPV4\","
      "\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\":\"SSC_MODE_1\",\"allowedSscModes\":["
      "\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\":0,\"arp\":{\"priorityLevel\":1,\"preemptCap\":"
      "\"NOT_PREEMPT\",\"preemptVuln\":\"NOT_PREEMPTABLE\"},"
      "\"priorityLevel\":1},\"sessionAmbr\":{\"uplink\":\"100 "
      "Mbps\",\"downlink\":\"150 "
      "Mbps\"},\"3gppChargingCharacteristics\":"
      "\"ThisIs3gppChargingCharacteristics\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\",\"upConfid\":\"REQUIRED\"}}");

  std::string jsonDnnDataElem2(
      "{\"dnn\":\"ThisIsDnn2\","
      "\"defaultDnnIndicator\":false,"
      "\"lboRoamingAllowed\":false,"
      "\"iwkEpsInd\":false,"
      "\"ladnIndicator\":false,"
      "\"pduSessionTypes\":{\"defaultSessionType\":\"IPV4\","
      "\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\":\"SSC_MODE_2\",\"allowedSscModes\":["
      "\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\":0,\"arp\":{\"priorityLevel\":2,\"preemptCap\":"
      "\"NOT_PREEMPT\",\"preemptVuln\":\"NOT_PREEMPTABLE\"},"
      "\"priorityLevel\":2},\"sessionAmbr\":{\"uplink\":\"200 "
      "Mbps\",\"downlink\":\"250 "
      "Mbps\"},\"3gppChargingCharacteristics\":"
      "\"ThisIs3gppChargingCharacteristics\","
      "\"upSecurity\":{\"upIntegr\":\"PREFERRED\",\"upConfid\":\"PREFERRED\"}"
      "}");

  std::string jsonDataListArray("[" + jsonDnnDataElem1 + "," +
                                jsonDnnDataElem2 + "]");

  std::string jsonDnnDataObject(
      "{\"dnnListId:\":\"ThisIsDnnListId\",\"dnnDataList\":" +
      jsonDataListArray + "}");

  std::string jsonUdmSliceProfiles(
      "[{\"plmnIdSetIdentifier\":\"1234\",\"nssai\":" + jsonNssaiObject + "," +
      "\"dnnLists\":[" + jsonDnnDataObject + "]}]");

  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//1111\""
      ",\"data\":{\"plmnIdSets\":" +
      jsonUdmSliceProfiles + "}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(parser.error(), false);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");

  EXPECT_EQ(record.changes[0].udmSliceProfile.plmnIdSets[0].plmnIdSetIdentifier,
            "1234");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "ThisIsDnnListId1");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            111);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "AAAA");

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[1]
                .nssai.sst,
            222);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[1]
                .nssai.sd,
            "BBBB");

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .dnnListId,
            "ThisIsDnnListId3");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sst,
            333);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sd,
            "CCCC");

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[1]
                .dnnListId,
            "ThisIsDnnListId4");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[1]
                .nssai.sst,
            444);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[1]
                .nssai.sd,
            "DDDD");

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .ladnIndicator,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .defaultDnnIndicator,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .lboRoamingAllowed,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .iwkEpsInd,
            true);

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV4");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[1],
            "IPV6");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_1");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            0);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            1);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            1);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "100 Mbps");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "150 Mbps");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "REQUIRED");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .threegppChargingCharacteristics,
            "ThisIs3gppChargingCharacteristics");

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .ladnIndicator,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .defaultDnnIndicator,
            true);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .lboRoamingAllowed,
            true);

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .iwkEpsInd,
            false);

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV4");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .pduSessionTypes.allowedSessionTypes[1],
            "IPV6");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .sscModes.defaultSscMode,
            "SSC_MODE_2");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_1");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .fivegQosProfile.fiveqi,
            0);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .fivegQosProfile.arp.priorityLevel,
            2);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .fivegQosProfile.priorityLevel,
            2);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .sessionAmbr.uplink,
            "200 Mbps");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .sessionAmbr.downlink,
            "250 Mbps");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .upSecurity.upIntegr,
            "PREFERRED");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .threegppChargingCharacteristics,
            "ThisIs3gppChargingCharacteristics");

  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .ladnIndicator,
            false);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .defaultDnnIndicator,
            false);
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[1]
                .lboRoamingAllowed,
            false);
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoPlmnIdSetIdentifier) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[plmnIdSetIdentifier] not found");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoDnnLists) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]}}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[dnnLists] not found");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfileNeitherPlmnIdSetIdentifierNorDnnLists) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]}}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[plmnIdSetIdentifier] not found");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "Mandatory field:[dnnLists] not found");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNo5qi) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[5qi] not found as child of:[5gQosProfile]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoArp) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[arp] not found as child of:[5gQosProfile]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNeither5qiNorArp) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[5qi] not found as child of:[5gQosProfile]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "Mandatory field:[arp] not found as child of:[5gQosProfile]");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfileNoPriorityLevelNoPreemptCapNopreemptVuln) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 3);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[priorityLevel] not found as child of:[arp]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "Mandatory field:[preemptCap] not found as child of:[arp]");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");

  EXPECT_EQ(record.response.errors[2].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[2].errorDetails.at("description"),
            "Mandatory field:[preemptVuln] not found as child of:[arp]");
  EXPECT_EQ(record.response.errors[2].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoSessionAmbr) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\": \"NOT_PREEMPT\" ,\"preemptVuln\": \"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[uplink] not found as child of:[sessionAmbr]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "Mandatory field:[downlink] not found as child of:[sessionAmbr]");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoNssaiSst) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\": \"NOT_PREEMPT\" ,\"preemptVuln\": \"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[sst] not found as child of:[nssai]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoDefaultNssai) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\": \"NOT_PREEMPT\" ,\"preemptVuln\": \"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "When \"nssai\" is included, it must have at least one element in "
            "\"defaultSingleNssais\"");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}
TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoNssaiInDefault) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\""
      "}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\": \"NOT_PREEMPT\" ,\"preemptVuln\": \"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "Mandatory field:[nssai] not found as child of:[defaultSingleNssais]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}
TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfileUpIntegrityWithInvalidValue) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":12345, \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[upIntegr] is not string");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}
TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfilePlmnSetIdentifierNotString) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":234,\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[plmnIdSetIdentifier] is not string");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}
TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfileNssaiDetailsNotObjectDefaultSingle) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}]],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[defaultSingleNssais] is not an object");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "When \"nssai\" is included, it must have at least one element in "
            "\"defaultSingleNssais\"");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}
TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfileNssaiDetailNotObjectSingleNssais) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[singleNssais] is not an object");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNssaiNotObject) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":[{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]}],"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[nssai] is not an object");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "When \"nssai\" is included, it must have at least one element in "
            "\"defaultSingleNssais\"");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfileNoNssaiSstInSingleNssai) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\": \"NOT_PREEMPT\" ,\"preemptVuln\": \"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[sst] not found as child of:[nssai]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoNssaiSingleNssai) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\"}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\": \"NOT_PREEMPT\" ,\"preemptVuln\": \"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[nssai] not found as child of:[singleNssais]");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfileUpConfidWithInvalidValue) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":12345} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[upConfid] is not string");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileDnnDataIsNotObject) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"REQUIRED\"} "
      "}]]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[dnnDataList] is not an object");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileDnnListNotObject) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"//profiles//"
      "udmProfiles//udmSliceProfiles//2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"preemptVuln\":\"NOT_PREEMPTABLE\"}},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"REQUIRED\"} "
      "}]}]]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[dnnLists] is not an object");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "//profiles//udmProfiles//udmSliceProfiles//2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileNoDnnListId) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"/profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      ",\"data\":{\"udmSliceProfileId\":\"profile1\",\"plmnIdSets\":[{"
      "\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", "
      "\"preemptVuln\":\"NOT_PREEMPTABLE\"},\"priorityLevel\":101},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory field:[dnnListId] not found");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/2222");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreas) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"udmRoamingAreaId\":\"roaming_area_1\",\"areaList\":["
      "{\"tacs\":[\"1357\",\"246802\"]},{"
      "\"areaCodes\":\"area1\"}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath,
            "/profiles/udmProfiles/udmRoamingAreas/1234e");
  EXPECT_EQ(record.changes[0].udmRoamingArea.udmRoamingAreaId,
            "roaming_area_1");
  EXPECT_EQ(record.changes[0].udmRoamingArea.areaList.size(), 2);
  entities::tacs_t tacs = boost::get<entities::tacs_t>(
      record.changes[0].udmRoamingArea.areaList[0].area);
  EXPECT_EQ(tacs.size(), 2);
  EXPECT_EQ(tacs[0], "1357");
  EXPECT_EQ(tacs[1], "246802");
  entities::area_code_t areaCode = boost::get<entities::area_code_t>(
      record.changes[0].udmRoamingArea.areaList[1].area);
  EXPECT_EQ(areaCode, "area1");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreasNotAnArray) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"areaList\":{\"tacs\":[\"1357\",\"246802\"]}}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[areaList] is not an array");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/1234e");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreaNotAnObject) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"areaList\":[\"1357\"]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[areaList] is not an object");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/1234e");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreasTacsNotString) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"areaList\":[{\"tacs\":[1357]},{\"areaCodes\":"
      "\"area1\"}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[tacs] is not string");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/1234e");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreasAreaCodeNotString) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"areaList\":[{\"tacs\":[\"1357\"]},{\"areaCodes\":1}]"
      "}"
      "}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[areaCodes] is not string");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/1234e");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreasMissingTacsAndAreaCode) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"areaList\":[{}"
      "]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Mandatory fields:[areaCodes] or [tacs] not found");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/1234e");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreaIdNotString) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"udmRoamingAreaId\":1,\"areaList\":[{\"tacs\":["
      "\"1357\"]},{\"areaCodes\":\"1\"}]}"
      "}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Field:[udmRoamingAreaId] is not string");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/1234e");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmStaticDataVendorSpecific) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "udmStaticData\",\"data\":{\"subscribedUeAmbr\":{\"uplink\":\"1 "
      "Gbps\",\"downlink\":\"2 "
      "Gbps\"},\"udmSliceProfileId\":{\"id\":\"2\"},\"ratRestrictions\":["
      "\"NR\","
      "\"WLAN\"],\"vendorSpecific-00193\":{\"key1\":[\"hello\",\"world\"],"
      "\"key2\":true,\"key3\":100},\"vendorSpecific-123456\":20,\"hello\":21}}]"
      "}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.subscribedUeAmbr.uplink, "1 Gbps");
  EXPECT_EQ(record.changes[0].udmStaticData.subscribedUeAmbr.downlink,
            "2 Gbps");
  EXPECT_EQ(record.changes[0].udmStaticData.ratRestrictions.size(), 2);
  EXPECT_EQ(record.changes[0].udmStaticData.ratRestrictions[0], "NR");
  EXPECT_EQ(record.changes[0].udmStaticData.ratRestrictions[1], "WLAN");
  EXPECT_EQ(record.changes[0].udmStaticData.vendorSpecific.size(), 2);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0].udmStaticData.vendorSpecific.at(
                    "vendorSpecific-00193")),
            "{\"key1\":[\"hello\",\"world\"],\"key2\":true,\"key3\":100}");
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0].udmStaticData.vendorSpecific.at(
                    "vendorSpecific-123456")),
            "20");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmStaticDataForbiddenAreasVendorSpecific) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"forbiddenAreas\":[{\"id\":\"FB1\",\"vendorSpecific-001\":[1,2]},{"
      "\"id\":\"FB2\",\"vendorSpecific-002\":{\"key1\":\"val1\",\"key2\":"
      "\"val2\"}}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(record.changes[0].udmStaticData.forbiddenAreas.size(), 2);
  EXPECT_EQ(record.changes[0].udmStaticData.forbiddenAreas[0].id, "FB1");
  EXPECT_EQ(record.changes[0].udmStaticData.forbiddenAreas[1].id, "FB2");
  EXPECT_EQ(
      record.changes[0].udmStaticData.forbiddenAreas[0].vendorSpecific.size(),
      1);
  EXPECT_EQ(
      StrDocumentConverter::documentToStr(
          record.changes[0].udmStaticData.forbiddenAreas[0].vendorSpecific.at(
              "vendorSpecific-001")),
      "[1,2]");
  EXPECT_EQ(
      record.changes[0].udmStaticData.forbiddenAreas[1].vendorSpecific.size(),
      1);
  EXPECT_EQ(
      StrDocumentConverter::documentToStr(
          record.changes[0].udmStaticData.forbiddenAreas[1].vendorSpecific.at(
              "vendorSpecific-002")),
      "{\"key1\":\"val1\",\"key2\":\"val2\"}");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmStaticDataServiceAreaRestrictionVendorSpecific) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"serviceAreaRestriction\":{\"restrictionType\":\"ALLOWED_AREAS\","
      "\"areas\":[{\"id\":\"AREA1\",\"vendorSpecific-1\":100},{\"id\":"
      "\"AREA2\",\"vendorSpecific-2\":{\"the\":\"Vendor\"}}],\"maxNumOfTAs\":2,"
      "\"vendorSpecific-00194\":2,\"vendorSpecific-00195\":3}}}"
      "]"
      "}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(
      record.changes[0].udmStaticData.serviceAreaRestriction.restrictionType,
      "ALLOWED_AREAS");
  EXPECT_EQ(record.changes[0].udmStaticData.serviceAreaRestriction.areas.size(),
            2);
  EXPECT_EQ(record.changes[0]
                .udmStaticData.serviceAreaRestriction.areas[0]
                .vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmStaticData.serviceAreaRestriction.areas[0]
                    .vendorSpecific.at("vendorSpecific-1")),
            "100");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.serviceAreaRestriction.areas[1]
                .vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmStaticData.serviceAreaRestriction.areas[1]
                    .vendorSpecific.at("vendorSpecific-2")),
            "{\"the\":\"Vendor\"}");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.serviceAreaRestriction.vendorSpecific.size(),
            2);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmStaticData.serviceAreaRestriction.vendorSpecific.at(
                        "vendorSpecific-00194")),
            "2");
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmStaticData.serviceAreaRestriction.vendorSpecific.at(
                        "vendorSpecific-00195")),
            "3");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmStaticDataStaticIpAddressVendorSpecific) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":"
      "\"udmStaticData\""
      ",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"staticIpAddress\":{\"vendorSpecific-1\":[1,2],\"vendorSpecific-2\":{"
      "\"key1\":\"A\"},\"nssaiDnnIpLists\":[{\"vendorSpecific-3\":100,"
      "\"nssai\":{\"sst\":22,"
      "\"sd\":"
      "\"11\",\"vendorSpecific-4\":4},\"dnnIpList\":[{\"vendorSpecific-5\":5,"
      "\"dnn\":\"1\",\"ipAddress\":[{\"vendorSpecific-6\":{\"key6\":6},"
      "\"ipv4Addr\":"
      "\"11\"},{\"ipv6Addr\":\"22\"},"
      "{\"ipv6Prefix\":\"33\"}]}]},{\"nssai\":{\"sst\":33,\"sd\":\"33\"},"
      "\"dnnIpList\":[{\"dnn\":"
      "\"4\",\"ipAddress\":[{\"ipv4Addr\":\"1211\"},{\"ipv6Addr\":\"1222\"},{"
      "\"ipv6Prefix\":\"1133\"}]}]}]}}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].operation, "CREATE");
  EXPECT_EQ(record.changes[0].resourcePath, "udmStaticData");
  EXPECT_EQ(
      record.changes[0].udmStaticData.staticIpAddress.nssaiDnnIpLists.size(),
      2);
  EXPECT_EQ(
      record.changes[0].udmStaticData.staticIpAddress.vendorSpecific.size(), 2);
  EXPECT_EQ(
      StrDocumentConverter::documentToStr(
          record.changes[0].udmStaticData.staticIpAddress.vendorSpecific.at(
              "vendorSpecific-1")),
      "[1,2]");
  EXPECT_EQ(
      StrDocumentConverter::documentToStr(
          record.changes[0].udmStaticData.staticIpAddress.vendorSpecific.at(
              "vendorSpecific-2")),
      "{\"key1\":\"A\"}");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                    .vendorSpecific.at("vendorSpecific-3")),
            "100");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                    .snssai.vendorSpecific.at("vendorSpecific-4")),
            "4");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                    .dnnIpList[0]
                    .vendorSpecific.at("vendorSpecific-5")),
            "5");
  EXPECT_EQ(record.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .ipAddress[0]
                .vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                    .dnnIpList[0]
                    .ipAddress[0]
                    .vendorSpecific.at("vendorSpecific-6")),
            "{\"key6\":6}");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmSliceProfileVendorSpecific) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"/profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      ",\"data\":{\"vendorSpecific-00193\":{\"key1\":[\"hello\",\"world\"],"
      "\"key2\":true,\"key3\":100},\"vendorSpecific-123456\":20,\"hello\":21,"
      "\"udmSliceProfileId\":\"profile1\",\"plmnIdSets\":[{"
      "\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, \"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\": 2, \"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", "
      "\"preemptVuln\":\"NOT_PREEMPTABLE\"},\"priorityLevel\":101},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", \"upConfid\":\"PREFERRED\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(parser.error(), false);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].udmSliceProfile.vendorSpecific.size(), 2);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0].udmSliceProfile.vendorSpecific.at(
                    "vendorSpecific-00193")),
            "{\"key1\":[\"hello\",\"world\"],\"key2\":true,\"key3\":100}");
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0].udmSliceProfile.vendorSpecific.at(
                    "vendorSpecific-123456")),
            "20");
}

TEST(ValidatorRapidJsonParserTest,
     ParseUdmSliceProfilePlmnIdSetsVendorSpecific) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"/profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      ",\"data\":{\"udmSliceProfileId\":\"profile1\",\"plmnIdSets\":[{"
      "\"plmnIdSetIdentifier\":\"234\",\"vendorSpecific-00193\":{\"key1\":["
      "\"hello\",\"world\"],\"key2\":true,\"key3\":100},\"vendorSpecific-"
      "123456\":20,\"nssai\":{"
      "\"defaultSingleNssais\":[{\"vendorSpecific-01\":true,\"dnnListId\":"
      "\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\",\"vendorSpecific-02\":{\"keyA\":\"A\","
      "\"keyB\":\"B\"}}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"vendorSpecific-03\":[\"1\",\"2\"],\"nssai\":{\"sst\":5,\"sd\":"
      "\"33\",\"vendorSpecific-04\":false}}]},"
      "\"dnnLists\":[{\"vendorSpecific-05\":{\"key051\":[10,11],\"key052\":"
      "\"hi\"},\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\": true, "
      "\"vendorSpecific-06\":\"06\",\"vendorSpecific-07\":{\"key071\":\"071\","
      "\"key072\":\"072\"},\"lboRoamingAllowed\": true, "
      "\"iwkEpsInd\": true"
      ",\"ladnIndicator\": false"
      ",\"pduSessionTypes\":{\"vendorSpecific-08\":\"ABC\","
      "\"defaultSessionType\": "
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"vendorSpecific-09\":\"09\",\"vendorSpecific-10\":\"10\","
      "\"defaultSscMode\": "
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"vendorSpecific-11\":[11,12],\"5qi\": 2, "
      "\"arp\":{\"priorityLevel\":3, "
      "\"preemptCap\":\"NOT_PREEMPT\", \"vendorSpecific-12\":\"ACDC\","
      "\"preemptVuln\":\"NOT_PREEMPTABLE\"},\"priorityLevel\":101},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\", \"downlink\":\"1 "
      "Gbps\",\"vendorSpecific-13\":\"Pink Floyd\"},"
      "\"3gppChargingCharacteristics\": \"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\", "
      "\"upConfid\":\"PREFERRED\",\"vendorSpecific-14\":\"The Rolling "
      "Stones\"} "
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);
  EXPECT_EQ(parser.error(), false);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].udmSliceProfile.vendorSpecific.size(), 0);
  EXPECT_EQ(
      record.changes[0].udmSliceProfile.plmnIdSets[0].vendorSpecific.size(), 2);
  EXPECT_EQ(
      StrDocumentConverter::documentToStr(
          record.changes[0].udmSliceProfile.plmnIdSets[0].vendorSpecific.at(
              "vendorSpecific-00193")),
      "{\"key1\":[\"hello\",\"world\"],\"key2\":true,\"key3\":100}");
  EXPECT_EQ(
      StrDocumentConverter::documentToStr(
          record.changes[0].udmSliceProfile.plmnIdSets[0].vendorSpecific.at(
              "vendorSpecific-123456")),
      "20");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .nssai.defaultSingleNssais[0]
                    .vendorSpecific.at("vendorSpecific-01")),
            "true");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .nssai.defaultSingleNssais[0]
                    .nssai.vendorSpecific.at("vendorSpecific-02")),
            "{\"keyA\":\"A\",\"keyB\":\"B\"}");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .nssai.singleNssais[0]
                    .vendorSpecific.at("vendorSpecific-03")),
            "[\"1\",\"2\"]");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .nssai.singleNssais[0]
                    .nssai.vendorSpecific.at("vendorSpecific-04")),
            "false");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .vendorSpecific.at("vendorSpecific-05")),
            "{\"key051\":[10,11],\"key052\":\"hi\"}");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .vendorSpecific.size(),
            2);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .dnnDataList[0]
                    .vendorSpecific.at("vendorSpecific-06")),
            "\"06\"");
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .dnnDataList[0]
                    .vendorSpecific.at("vendorSpecific-07")),
            "{\"key071\":\"071\",\"key072\":\"072\"}");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .dnnDataList[0]
                    .pduSessionTypes.vendorSpecific.at("vendorSpecific-08")),
            "\"ABC\"");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.vendorSpecific.size(),
            2);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .dnnDataList[0]
                    .sscModes.vendorSpecific.at("vendorSpecific-09")),
            "\"09\"");
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .dnnDataList[0]
                    .sscModes.vendorSpecific.at("vendorSpecific-10")),
            "\"10\"");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .dnnDataList[0]
                    .fivegQosProfile.vendorSpecific.at("vendorSpecific-11")),
            "[11,12]");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.vendorSpecific.size(),
            1);
  EXPECT_EQ(
      StrDocumentConverter::documentToStr(
          record.changes[0]
              .udmSliceProfile.plmnIdSets[0]
              .dnnLists[0]
              .dnnDataList[0]
              .fivegQosProfile.arp.vendorSpecific.at("vendorSpecific-12")),
      "\"ACDC\"");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .dnnDataList[0]
                    .sessionAmbr.vendorSpecific.at("vendorSpecific-13")),
            "\"Pink Floyd\"");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.vendorSpecific.size(),
            1);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0]
                    .udmSliceProfile.plmnIdSets[0]
                    .dnnLists[0]
                    .dnnDataList[0]
                    .upSecurity.vendorSpecific.at("vendorSpecific-14")),
            "\"The Rolling Stones\"");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreasVendorSpecific) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"vendorSpecific-00193\":{\"key1\":[\"hello\","
      "\"world\"],\"key2\":true,\"key3\":100},\"vendorSpecific-123456\":20,"
      "\"hello\":21,\"udmRoamingAreaId\":\"roaming_area_1\",\"areaList\":["
      "{\"tacs\":[\"1357\",\"246802\"]},{"
      "\"areaCodes\":\"area1\"}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].udmRoamingArea.vendorSpecific.size(), 2);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0].udmRoamingArea.vendorSpecific.at(
                    "vendorSpecific-00193")),
            "{\"key1\":[\"hello\",\"world\"],\"key2\":true,\"key3\":100}");
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0].udmRoamingArea.vendorSpecific.at(
                    "vendorSpecific-123456")),
            "20");
}

TEST(ValidatorRapidJsonParserTest, ParseUdmRoamingAreasAreaListVendorSpecific) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\""
      "/profiles/udmProfiles/udmRoamingAreas/"
      "1234e\",\"data\":{\"udmRoamingAreaId\":\"roaming_area_1\",\"areaList\":["
      "{\"vendorSpecific-01\":[\"John\",\"Lennon\"],\"vendorSpecific-02\":\"is "
      "a Beatle\",\"tacs\":[\"1357\",\"246802\"]},{"
      "\"areaCodes\":\"area1\"}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);
  EXPECT_EQ(parser.error(), false);
  entities::ValidationData record;
  EXPECT_EQ(parser.getValidationData(record), true);

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.changes[0].udmRoamingArea.vendorSpecific.size(), 0);
  EXPECT_EQ(record.changes[0].udmRoamingArea.areaList[0].vendorSpecific.size(),
            2);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0].udmRoamingArea.areaList[0].vendorSpecific.at(
                    "vendorSpecific-01")),
            "[\"John\",\"Lennon\"]");
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                record.changes[0].udmRoamingArea.areaList[0].vendorSpecific.at(
                    "vendorSpecific-02")),
            "\"is a Beatle\"");
}
