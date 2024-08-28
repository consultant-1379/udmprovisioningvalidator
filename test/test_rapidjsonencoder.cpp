#include "StrDocumentConverter.hpp"
#include "gtest/gtest.h"
#include "ports/json/ValidatorRapidJsonEncoder.hpp"
#include "ports/ports.hpp"

TEST(ValidatorRapidJsonEncoderTest, EncodeSeveralErrors) {
  std::string jsonString{
      "{\"errors\":[{\"errorMessage\":\"Error message "
      "1\",\"errorDetails\":{\"description\":\"Error description "
      "1\",\"path\":\"resource1\"}},{\"errorMessage\":\"Error message "
      "2\",\"errorDetails\":{\"description\":\"Error description "
      "2\",\"info\":\"Additional info\",\"path\":\"resource2\"}}]}"};
  entities::ValidationData data;
  entities::Error err1, err2;
  err1.errorMessage = "Error message 1";
  err1.errorDetails.insert(
      {{"path", "resource1"}, {"description", "Error description 1"}});
  err2.errorMessage = "Error message 2";
  err2.errorDetails.insert({{"path", "resource2"},
                            {"description", "Error description 2"},
                            {"info", "Additional info"}});
  data.response.errors.push_back(err1);
  data.response.errors.push_back(err2);
  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;
  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest,
     GivenEmptyDataThenJsonIsGeneratedCorrectly) {
  std::string jsonString{"{}"};
  entities::ValidationData data;
  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;
  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest, EncodeUdmStaticDataChanges) {
  std::string jsonString{
      "{\"changes\":[{\"operation\":\"CREATE\","
      "\"resource_path\":\"/subscribers/111aaa/udmSubscription/"
      "udmStaticAndRegistrationData/"
      "udmStaticData\",\"data\":{\"udmSliceProfileId\":{\"id\":\"1\"},"
      "\"subscribedUeAmbr\":{\"uplink\":\"1 Gbps\",\"downlink\":\"2 "
      "Gbps\"},\"ratRestrictions\":[\"NR\",\"WLAN\"],\"forbiddenAreas\":[{"
      "\"id\":\"roaming1\"},{\"id\":\"roaming2\"}],"
      "\"serviceAreaRestriction\":{"
      "\"restrictionType\":\"ALLOWED_AREAS\",\"areas\":[{\"id\":"
      "\"roaming1\"},{"
      "\"id\":\"roaming2\"}],\"maxNumOfTAs\":7},"
      "\"coreNetworkTypeRestrictions\":[\"5GC\",\"EPC\"],"
      "\"staticIpAddress\":{"
      "\"nssaiDnnIpLists\":[{\"nssai\":{\"sst\":1,\"sd\":\"sd1\"},"
      "\"dnnIpList\":[{\"dnn\":\"10\",\"ipAddress\":[{\"ipv4Addr\":\"192."
      "168."
      "10.1\"},{\"ipv6Addr\":\"22:33:44\"}]}]},{\"nssai\":{\"sst\":2,"
      "\"sd\":"
      "\"sd2\"},\"dnnIpList\":[{\"dnn\":\"11\",\"ipAddress\":[{"
      "\"ipv4Addr\":"
      "\"192.168.10.2\"},{\"ipv6Prefix\":\"22:33:55\"}]}]}]}}},"
      "{\"operation\":\"UPDATE\","
      "\"resource_path\":\"/subscribers/222bbb/udmSubscription/"
      "udmStaticAndRegistrationData/"
      "udmStaticData\",\"data\":{\"udmSliceProfileId\":{\"id\":\"2\"},"
      "\"subscribedUeAmbr\":{\"uplink\":\"3 Gbps\",\"downlink\":\"4 "
      "Gbps\"},\"mpsPriority\":true,\"mcsPriority\":false,"
      "\"dlPacketCount\":"
      "100,\"micoAllowed\":true,\"roamingOdb\":\"roaming_odb_2\","
      "\"ueUsageType\":200,"
      "\"rfspIndex\":201,\"subsRegTimer\":202,\"udmSliceData\":{"
      "\"plmnIdSets\":"
      "[{"
      "\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"3gppChargingCharacteristics\":\"charging\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{"
      "\"dnnListId\":"
      "\"33\","
      "\"3gppChargingCharacteristics\":\"3gppcharging\","
      "\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\":true,\"lboRoamingAllowed\":true,"
      "\"iwkEpsInd\":true"
      ",\"ladnIndicator\":false"
      ",\"pduSessionTypes\":{\"defaultSessionType\":"
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\":"
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\":2,\"arp\":{\"priorityLevel\":3,"
      "\"preemptCap\":\"NOT_PREEMPT\",\"preemptVuln\":\"NOT_PREEMPTABLE\"},"
      "\"priorityLevel\":10},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\",\"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\":\"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\",\"upConfid\":\"PREFERRED\"}"
      "}]}]}]}}}]}"};
  entities::ValidationData data;
  entities::Change ch1, ch2;
  entities::UdmRoamingAreaReference area1, area2;
  entities::ServiceAreaRestriction serv1;
  entities::Snssai sa1, sa2;
  entities::DnnIps dnn1, dnn2;
  entities::IpAddr ip1, ip2, ip3, ip4;
  entities::NssaiDnnIps nssai1, nssai2;

  entities::PlmnIdSets set1;
  entities::NssaiDetails nssaiDet1, nssaiDet2;
  entities::SliceInfoNssai slice1;
  entities::Snssai snssai1, snssai2;
  entities::DnnList dnnList1;
  entities::DnnData dnnData1;
  entities::PduSessionTypes pduSess1;
  entities::SscModes sscModes1;
  entities::Arp arp1;
  entities::Ambr ambr1;
  entities::UpSecurity upSec1;

  set1.plmnIdSetIdentifier = "234";
  nssaiDet1.dnnListId = "22";
  nssaiDet1.threegppChargingCharacteristics = "charging";
  snssai1.sst = 3;
  snssai1.sd = "2";
  nssaiDet1.nssai = snssai1;
  slice1.defaultSingleNssais.push_back(nssaiDet1);
  nssaiDet2.dnnListId = "33";
  nssaiDet2.threegppChargingCharacteristics = "3gppcharging";
  snssai2.sst = 5;
  snssai2.sd = "33";
  nssaiDet2.nssai = snssai2;

  slice1.singleNssais.push_back(nssaiDet2);
  set1.nssai = slice1;
  dnnList1.dnnListId = "222";
  dnnData1.dnn = "1";
  dnnData1.defaultDnnIndicator = true;
  dnnData1.lboRoamingAllowed = true;
  dnnData1.iwkEpsInd = true;
  dnnData1.ladnIndicator = false;
  pduSess1.defaultSessionType = "IPV4";
  pduSess1.allowedSessionTypes.push_back("IPV4");
  pduSess1.allowedSessionTypes.push_back("IPV6");
  dnnData1.pduSessionTypes = pduSess1;
  sscModes1.defaultSscMode = "SSC_MODE_1";
  sscModes1.allowedSscModes.push_back("SSC_MODE_1");
  sscModes1.allowedSscModes.push_back("SSC_MODE_2");
  dnnData1.sscModes = sscModes1;
  dnnData1.fivegQosProfile.fiveqi = 2;
  arp1.priorityLevel = 3;
  arp1.preemptCap = "NOT_PREEMPT";
  arp1.preemptVuln = "NOT_PREEMPTABLE";
  dnnData1.fivegQosProfile.arp = arp1;
  dnnData1.fivegQosProfile.priorityLevel = 10;
  dnnData1.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  ambr1.uplink = "10 Gbps";
  ambr1.downlink = "1 Gbps";
  dnnData1.sessionAmbr = ambr1;
  dnnData1.threegppChargingCharacteristics = "charging";
  upSec1.upIntegr = "REQUIRED";
  upSec1.upConfid = "PREFERRED";
  dnnData1.upSecurity = upSec1;
  dnnData1.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData1.fieldExistence.hasLboRoamingAllowed = true;
  dnnData1.fieldExistence.hasIwkEpsInd = true;
  dnnData1.fieldExistence.hasLadnIndicator = true;
  dnnData1.fieldExistence.hasPduSessionTypes = true;
  dnnData1.fieldExistence.hasSscModes = true;
  dnnData1.fieldExistence.hasSessionAmbr = true;
  dnnData1.fieldExistence.hasFivegQosProfile = true;
  dnnData1.fieldExistence.hasUpSecurity = true;
  dnnList1.dnnDataList.push_back(dnnData1);
  set1.dnnLists.push_back(dnnList1);

  set1.fieldExistenceUdmSliceProfile.hasNssai = true;
  set1.fieldExistenceUdmSliceProfile.hasAnyNssai = true;

  area1.id = "roaming1";
  area2.id = "roaming2";
  serv1.restrictionType = "ALLOWED_AREAS";
  serv1.areas.push_back(area1);
  serv1.areas.push_back(area2);
  serv1.maxNumOfTas = 7;
  sa1.sst = 1;
  sa1.sd = "sd1";
  sa2.sst = 2;
  sa2.sd = "sd2";
  dnn1.dnn = "10";
  ip1.ipType = entities::IpAddrType::IPv4;
  entities::ipv4_t ipAddr4 = "192.168.10.1";
  ip1.ip = ipAddr4;
  ip2.ipType = entities::IpAddrType::IPv6;
  entities::ipv6_t ipAddr5 = "22:33:44";
  ip2.ip = ipAddr5;
  ip3.ipType = entities::IpAddrType::IPv4;
  entities::ipv4_t ipAddr6 = "192.168.10.2";
  ip3.ip = ipAddr6;
  ip4.ipType = entities::IpAddrType::IPv6Prefix;
  entities::ipv6_prefix_t ipAddr7 = "22:33:55";
  ip4.ip = ipAddr7;
  dnn1.ipAddress.push_back(ip1);
  dnn1.ipAddress.push_back(ip2);
  dnn2.dnn = "11";
  dnn2.ipAddress.push_back(ip3);
  dnn2.ipAddress.push_back(ip4);
  nssai1.snssai = sa1;
  nssai1.dnnIpList.push_back(dnn1);
  nssai2.snssai = sa2;
  nssai2.dnnIpList.push_back(dnn2);

  ch1.operation = "CREATE";
  ch1.resourcePath =
      "/subscribers/111aaa/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  ch1.udmStaticData.udmSliceProfileId.id = "1";
  ch1.udmStaticData.subscribedUeAmbr.uplink = "1 Gbps";
  ch1.udmStaticData.subscribedUeAmbr.downlink = "2 Gbps";
  ch1.udmStaticData.ratRestrictions.push_back("NR");
  ch1.udmStaticData.ratRestrictions.push_back("WLAN");
  ch1.udmStaticData.forbiddenAreas.push_back(area1);
  ch1.udmStaticData.forbiddenAreas.push_back(area2);
  ch1.udmStaticData.serviceAreaRestriction = serv1;
  ch1.udmStaticData.coreNetworkTypeRestrictions.push_back("5GC");
  ch1.udmStaticData.coreNetworkTypeRestrictions.push_back("EPC");
  ch1.udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(nssai1);
  ch1.udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(nssai2);
  ch1.udmStaticData.fieldExistence.hasSubscribedUeAmbr = true;
  ch1.udmStaticData.fieldExistence.hasServiceAreaRestriction = true;
  ch1.udmStaticData.fieldExistence.hasMaxNumOfTas = true;
  ch1.udmStaticData.fieldExistence.hasStaticIpAddress = true;

  ch2.operation = "UPDATE";
  ch2.resourcePath =
      "/subscribers/222bbb/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  ch2.udmStaticData.udmSliceProfileId.id = "2";
  ch2.udmStaticData.subscribedUeAmbr.uplink = "3 Gbps";
  ch2.udmStaticData.subscribedUeAmbr.downlink = "4 Gbps";
  ch2.udmStaticData.mpsPriority = true;
  ch2.udmStaticData.mcsPriority = false;
  ch2.udmStaticData.dlPacketCount = 100;
  ch2.udmStaticData.micoAllowed = true;
  ch2.udmStaticData.roamingOdb = "roaming_odb_2";
  ch2.udmStaticData.ueUsageType = 200;
  ch2.udmStaticData.rfspIndex = 201;
  ch2.udmStaticData.subsRegTimer = 202;
  ch2.udmStaticData.udmSliceData.plmnIdSets.push_back(set1);
  ch2.udmStaticData.fieldExistence.hasSubscribedUeAmbr = true;
  ch2.udmStaticData.fieldExistence.hasMpsPriority = true;
  ch2.udmStaticData.fieldExistence.hasMcsPriority = true;
  ch2.udmStaticData.fieldExistence.hasDlPacketCount = true;
  ch2.udmStaticData.fieldExistence.hasMicoAllowed = true;
  ch2.udmStaticData.fieldExistence.hasUeUsageType = true;
  ch2.udmStaticData.fieldExistence.hasRfspIndex = true;
  ch2.udmStaticData.fieldExistence.hasSubsRegTimer = true;
  ch2.udmStaticData.fieldExistence.hasUdmSliceData = true;
  data.response.changes.push_back(ch1);
  data.response.changes.push_back(ch2);
  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest, EncodeSmsStaticDataChanges) {
  std::string jsonString{
      "{\"changes\":[{\"operation\":\"CREATE\","
      "\"resource_path\":\"/subscribers/111aaa/smsSubscription/"
      "smsStaticAndRegistrationData/"
      "smsStaticData\",\"data\":{\"smsSubscriptionData\":{\"smsSubscribed\":"
      "true},"
      "\"smsManagementSubscriptionData\":{\"mtSmsSubscribed\":true,"
      "\"mtSmsBarringAll\":true,\"mtSmsBarringRoaming\":true,"
      "\"moSmsSubscribed\":true,"
      "\"moSmsBarringAll\":true,\"moSmsBarringRoaming\":true}}}]}"};

  entities::ValidationData data;
  entities::Change ch1;

  ch1.operation = "CREATE";
  ch1.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  ch1.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  ch1.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  ch1.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  ch1.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  ch1.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = true;
  ch1.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = true;
  ch1.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.response.changes.push_back(ch1);

  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest,
     EncodeSmsStaticDataChangesWithMissingAtributes) {
  std::string jsonString{
      "{\"changes\":[{\"operation\":\"CREATE\","
      "\"resource_path\":\"/subscribers/111aaa/smsSubscription/"
      "smsStaticAndRegistrationData/"
      "smsStaticData\",\"data\":{\"smsSubscriptionData\":{\"smsSubscribed\":"
      "true},"
      "\"smsManagementSubscriptionData\":{\"mtSmsSubscribed\":true,"
      "\"mtSmsBarringAll\":true,\"mtSmsBarringRoaming\":true}}}]}"};

  entities::ValidationData data;
  entities::Change ch1;

  ch1.operation = "CREATE";
  ch1.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  ch1.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  ch1.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  ch1.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  ch1.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  ch1.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = true;
  ch1.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = true;
  ch1.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = false;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = false;
  ch1.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = false;

  data.response.changes.push_back(ch1);

  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest, EncodeOneError) {
  std::string jsonString{
      "{\"errorMessage\":\"Error message "
      "1\",\"errorDetails\":{\"description\":\"Error description 1\"}}"};
  entities::Error err;
  err.errorMessage = "Error message 1";
  err.errorDetails.insert({{"description", "Error description 1"}});
  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;
  EXPECT_EQ(encoder.errorResponseToJson(err).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest, EncodeUdmSliceProfile) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"/profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      ",\"data\":{\"udmSliceProfileId\":\"profile1\",\"plmnIdSets\":[{"
      "\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"defaultDnnIndicator\":true,\"lboRoamingAllowed\":true,"
      "\"iwkEpsInd\":true"
      ",\"ladnIndicator\":false"
      ",\"pduSessionTypes\":{\"defaultSessionType\":"
      "\"IPV4\",\"allowedSessionTypes\":[\"IPV4\",\"IPV6\"]},"
      "\"sscModes\":{\"defaultSscMode\":"
      "\"SSC_MODE_1\",\"allowedSscModes\":[\"SSC_MODE_1\",\"SSC_MODE_2\"]},"
      "\"5gQosProfile\":{\"5qi\":2,\"arp\":{\"priorityLevel\":3,"
      "\"preemptCap\":\"NOT_PREEMPT\",\"preemptVuln\":\"NOT_PREEMPTABLE\"},"
      "\"priorityLevel\":10},"
      "\"sessionAmbr\":{\"uplink\":\"10 Gbps\",\"downlink\":\"1 Gbps\"},"
      "\"3gppChargingCharacteristics\":\"charging\","
      "\"upSecurity\":{\"upIntegr\":\"REQUIRED\",\"upConfid\":\"PREFERRED\"},"
      "\"smfList\":[\"smfList1\",\"smfList2\"],"
      "\"sameSmfInd\":true"
      "}]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  entities::ValidationData data;
  entities::Change ch1;
  entities::PlmnIdSets set1;
  entities::NssaiDetails nssai1, nssai2;
  entities::SliceInfoNssai slice1;
  entities::Snssai snssai1, snssai2;
  entities::DnnList dnn1;
  entities::DnnData dnnData1;
  entities::PduSessionTypes pduSess1;
  entities::SscModes sscModes1;
  entities::Arp arp1;
  entities::Ambr ambr1;
  entities::UpSecurity upSec1;

  ch1.operation = "CREATE";
  ch1.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/2222";
  set1.plmnIdSetIdentifier = "234";
  nssai1.dnnListId = "22";
  snssai1.sst = 3;
  snssai1.sd = "2";
  nssai1.nssai = snssai1;
  slice1.defaultSingleNssais.push_back(nssai1);
  nssai2.dnnListId = "33";
  snssai2.sst = 5;
  snssai2.sd = "33";
  nssai2.nssai = snssai2;

  slice1.singleNssais.push_back(nssai2);
  set1.nssai = slice1;
  dnn1.dnnListId = "222";
  dnnData1.dnn = "1";
  dnnData1.defaultDnnIndicator = true;
  dnnData1.lboRoamingAllowed = true;
  dnnData1.iwkEpsInd = true;
  dnnData1.ladnIndicator = false;
  pduSess1.defaultSessionType = "IPV4";
  pduSess1.allowedSessionTypes.push_back("IPV4");
  pduSess1.allowedSessionTypes.push_back("IPV6");
  dnnData1.pduSessionTypes = pduSess1;
  sscModes1.defaultSscMode = "SSC_MODE_1";
  sscModes1.allowedSscModes.push_back("SSC_MODE_1");
  sscModes1.allowedSscModes.push_back("SSC_MODE_2");
  dnnData1.sscModes = sscModes1;
  dnnData1.fivegQosProfile.fiveqi = 2;
  arp1.priorityLevel = 3;
  arp1.preemptCap = "NOT_PREEMPT";
  arp1.preemptVuln = "NOT_PREEMPTABLE";
  dnnData1.fivegQosProfile.arp = arp1;
  dnnData1.fivegQosProfile.priorityLevel = 10;
  dnnData1.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  ambr1.uplink = "10 Gbps";
  ambr1.downlink = "1 Gbps";
  dnnData1.sessionAmbr = ambr1;
  dnnData1.threegppChargingCharacteristics = "charging";
  upSec1.upIntegr = "REQUIRED";
  upSec1.upConfid = "PREFERRED";
  dnnData1.smfList.push_back("smfList1");
  dnnData1.smfList.push_back("smfList2");
  dnnData1.sameSmfInd = true;
  dnnData1.upSecurity = upSec1;
  dnnData1.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData1.fieldExistence.hasLboRoamingAllowed = true;
  dnnData1.fieldExistence.hasIwkEpsInd = true;
  dnnData1.fieldExistence.hasLadnIndicator = true;
  dnnData1.fieldExistence.hasPduSessionTypes = true;
  dnnData1.fieldExistence.hasSscModes = true;
  dnnData1.fieldExistence.hasSessionAmbr = true;
  dnnData1.fieldExistence.hasFivegQosProfile = true;
  dnnData1.fieldExistence.hasUpSecurity = true;
  dnnData1.fieldExistence.hasSmfList = true;
  dnnData1.fieldExistence.hasSameSmfInd = true;
  dnn1.dnnDataList.push_back(dnnData1);
  set1.dnnLists.push_back(dnn1);

  set1.fieldExistenceUdmSliceProfile.hasNssai = true;
  set1.fieldExistenceUdmSliceProfile.hasAnyNssai = true;

  ch1.udmSliceProfile.udmSliceProfileId = "profile1";
  ch1.udmSliceProfile.plmnIdSets.push_back(set1);
  data.response.changes.push_back(ch1);
  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest, EncodeUdmSliceProfileButNoData) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"/profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      ",\"data\":{}"
      "}]}");

  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  entities::ValidationData data;
  entities::Change ch1;

  ch1.operation = "CREATE";
  ch1.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/2222";
  data.response.changes.push_back(ch1);
  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest,
     EncodeUdmSliceProfileIdentifierAndDefaultSingleNssais) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"/profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}]}"
      "}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  entities::ValidationData data;
  entities::Change ch1;
  entities::PlmnIdSets set1;
  entities::NssaiDetails nssai1;
  entities::SliceInfoNssai slice1;
  entities::Snssai snssai1;

  ch1.operation = "CREATE";
  ch1.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/2222";
  set1.plmnIdSetIdentifier = "234";
  nssai1.dnnListId = "22";
  snssai1.sst = 3;
  snssai1.sd = "2";
  nssai1.nssai = snssai1;
  slice1.defaultSingleNssais.push_back(nssai1);
  set1.nssai = slice1;
  set1.fieldExistenceUdmSliceProfile.hasAnyNssai = true;
  ch1.udmSliceProfile.plmnIdSets.push_back(set1);
  data.response.changes.push_back(ch1);
  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest, EncodeUdmSliceProfileCheckFieldExistence) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"CREATE\",\"resource_path\":\"/profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      ",\"data\":{\"plmnIdSets\":[{\"plmnIdSetIdentifier\":\"234\",\"nssai\":{"
      "\"defaultSingleNssais\":[{\"dnnListId\":\"22\","
      "\"nssai\":{\"sst\":3,\"sd\":\"2\"}}],\"singleNssais\":[{\"dnnListId\":"
      "\"33\",\"nssai\":{\"sst\":5,\"sd\":\"33\"}}]},"
      "\"dnnLists\":[{\"dnnListId\":\"222\",\"dnnDataList\":[{\"dnn\":\"1\","
      "\"3gppChargingCharacteristics\":\"charging\"}"
      "]}]}]}}]}");

  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  entities::ValidationData data;
  entities::Change ch1;
  entities::PlmnIdSets set1;
  entities::NssaiDetails nssai1, nssai2;
  entities::SliceInfoNssai slice1;
  entities::Snssai snssai1, snssai2;
  entities::DnnList dnn1;
  entities::DnnData dnnData1;
  entities::SscModes sscModes1;
  entities::Arp arp1;
  entities::Ambr ambr1;

  ch1.operation = "CREATE";
  ch1.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/2222";
  set1.plmnIdSetIdentifier = "234";
  nssai1.dnnListId = "22";
  snssai1.sst = 3;
  snssai1.sd = "2";
  nssai1.nssai = snssai1;

  slice1.defaultSingleNssais.push_back(nssai1);
  nssai2.dnnListId = "33";
  snssai2.sst = 5;
  snssai2.sd = "33";
  nssai2.nssai = snssai2;

  slice1.singleNssais.push_back(nssai2);
  set1.nssai = slice1;
  dnn1.dnnListId = "222";
  dnnData1.dnn = "1";
  sscModes1.defaultSscMode = "SSC_MODE_1";
  sscModes1.allowedSscModes.push_back("SSC_MODE_1");
  sscModes1.allowedSscModes.push_back("SSC_MODE_2");
  dnnData1.sscModes = sscModes1;
  dnnData1.fivegQosProfile.fiveqi = 2;
  arp1.priorityLevel = 3;
  arp1.preemptCap = "NOT_PREEMPT";
  arp1.preemptVuln = "NOT_PREEMPTABLE";
  dnnData1.fivegQosProfile.arp = arp1;
  dnnData1.fivegQosProfile.fieldExistence.hasPriorityLevel = false;
  ambr1.uplink = "10 Gbps";
  ambr1.downlink = "1 Gbps";
  dnnData1.sessionAmbr = ambr1;
  dnnData1.threegppChargingCharacteristics = "charging";
  dnnData1.fieldExistence.hasDefaultDnnIndicator = false;
  dnnData1.fieldExistence.hasLboRoamingAllowed = false;
  dnnData1.fieldExistence.hasIwkEpsInd = false;
  dnnData1.fieldExistence.hasLadnIndicator = false;
  dnnData1.fieldExistence.hasPduSessionTypes = false;
  dnnData1.fieldExistence.hasUpSecurity = false;
  dnnData1.fieldExistence.hasSscModes = false;
  dnnData1.fieldExistence.hasSessionAmbr = false;
  dnnData1.fieldExistence.hasFivegQosProfile = false;
  dnn1.dnnDataList.push_back(dnnData1);
  set1.dnnLists.push_back(dnn1);

  set1.fieldExistenceUdmSliceProfile.hasNssai = true;
  set1.fieldExistenceUdmSliceProfile.hasAnyNssai = true;

  ch1.udmSliceProfile.plmnIdSets.push_back(set1);
  data.response.changes.push_back(ch1);
  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest, EncodeUdmRoamingAreaChanges) {
  std::string jsonString{
      "{\"changes\":[{\"operation\":\"CREATE\","
      "\"resource_path\":\"/profiles/udmProfiles/udmRoamingAreas/1111\","
      "\"data\":{\"udmRoamingAreaId\":\"roaming_area_1\",\"areaList\":[{"
      "\"tacs\":[\"1357\",\"246802\"]},{"
      "\"areaCodes\":\"area1\"}]}}]}"};
  entities::ValidationData data;
  entities::Change ch1;
  entities::area_list_t area1, area2;

  area1.area = std::vector<std::string>{"1357", "246802"};
  area2.area = std::string{"area1"};

  ch1.operation = "CREATE";
  ch1.resourcePath = "/profiles/udmProfiles/udmRoamingAreas/1111";
  ch1.udmRoamingArea.udmRoamingAreaId = "roaming_area_1";
  ch1.udmRoamingArea.areaList.push_back(area1);
  ch1.udmRoamingArea.areaList.push_back(area2);
  data.response.changes.push_back(ch1);
  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest, EncodeDeleteOperation) {
  std::string jsonString(
      "{\"changes\":[{\"operation\":\"DELETE\",\"resource_path\":\"/profiles/"
      "udmProfiles/udmSliceProfiles/2222\""
      "}]}");

  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  entities::ValidationData data;
  entities::Change ch1;

  ch1.operation = "DELETE";
  ch1.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/2222";
  data.response.changes.push_back(ch1);
  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}

TEST(ValidatorRapidJsonEncoderTest,
     EncodeUdmRoamingAreaChangesWithVendorSpecific) {
  std::string jsonString{
      "{\"changes\":[{\"operation\":\"CREATE\","
      "\"resource_path\":\"/profiles/udmProfiles/udmRoamingAreas/1111\","
      "\"data\":{\"vendorSpecific-00193\":1,"
      "\"udmRoamingAreaId\":\"roaming_area_1\",\"areaList\":[{"
      "\"tacs\":[\"1357\",\"246802\"]},{"
      "\"areaCodes\":\"area1\"}]}}]}"};
  entities::ValidationData data;
  entities::Change ch1;
  entities::area_list_t area1, area2;

  area1.area = std::vector<std::string>{"1357", "246802"};
  area2.area = std::string{"area1"};

  ch1.operation = "CREATE";
  ch1.resourcePath = "/profiles/udmProfiles/udmRoamingAreas/1111";
  ch1.udmRoamingArea.udmRoamingAreaId = "roaming_area_1";
  ch1.udmRoamingArea.areaList.push_back(area1);
  ch1.udmRoamingArea.areaList.push_back(area2);
  ch1.udmRoamingArea.vendorSpecific.emplace(
      "vendorSpecific-00193", StrDocumentConverter::strToDocument("1"));
  data.response.changes.push_back(ch1);
  ::port::secondary::json::ValidatorRapidJsonEncoder encoder;

  EXPECT_EQ(encoder.validatorResponseToJson(data).str(), jsonString);
}
