#include "entities/ValidationData.hpp"
#include "gtest/gtest.h"
#include "ports/HTTPcodes.hpp"

TEST(ValidationDataTest, ValidateUdmCreateSubscriptionOk) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1;
  entities::ServiceAreaRestriction serviceArea1;
  entities::NssaiDnnIps ips;
  entities::DnnIps dnnIps;
  entities::ProvJournal provJournal;
  entities::IpAddr ipAddress;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";
  provJournal.imsi = "imsi123";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.subscribedUeAmbr.uplink = "1 Gbps";
  change.udmStaticData.subscribedUeAmbr.downlink = "2 Gbps";
  change.udmStaticData.ratRestrictions.push_back("NR");
  change.udmStaticData.ratRestrictions.push_back("EUTRA");
  roamingArea1.id = "roamingArea1";
  change.udmStaticData.forbiddenAreas.push_back(roamingArea1);
  serviceArea1.restrictionType = "ALLOWED_AREAS";
  serviceArea1.areas.push_back(roamingArea1);
  serviceArea1.maxNumOfTas = 1;
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("5GC");
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("EPC");
  change.udmStaticData.mpsPriority = true;
  change.udmStaticData.mcsPriority = false;
  change.udmStaticData.dlPacketCount = 100;
  change.udmStaticData.micoAllowed = true;
  ips.snssai.sst = 90;
  ips.snssai.sd = "ABCDEF";
  dnnIps.dnn = "DNN1";
  ipAddress.ipType = entities::IpAddrType::IPv4;
  entities::ipv4_t ipAddr4 = "192.168.10.1";
  ipAddress.ip = ipAddr4;
  dnnIps.ipAddress.push_back(ipAddress);
  ips.dnnIpList.push_back(dnnIps);
  change.udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(ips);
  change.udmStaticData.ueUsageType = 12;
  change.udmStaticData.rfspIndex = 15;
  change.udmStaticData.subsRegTimer = 40;
  change.udmStaticData.fieldExistence = entities::fieldExistence{
      true, true, true, true, true, true, true, true, true, true, true, false};
  record.changes.push_back(change);
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});
  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.changes[0].udmStaticData.udmSliceProfileId.id,
            "profile1");
  EXPECT_EQ(record.response.changes[0].udmStaticData.subscribedUeAmbr.uplink,
            "1 Gbps");
  EXPECT_EQ(record.response.changes[0].udmStaticData.subscribedUeAmbr.downlink,
            "2 Gbps");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions.size(), 2);
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions[0], "NR");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions[1],
            "EUTRA");
  EXPECT_EQ(record.response.changes[0].udmStaticData.forbiddenAreas.size(), 1);
  EXPECT_EQ(record.response.changes[0].udmStaticData.forbiddenAreas[0].id,
            "roamingArea1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.restrictionType,
            "ALLOWED_AREAS");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.areas.size(),
            1);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.areas[0]
                .id,
            "roamingArea1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.maxNumOfTas,
            1);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.coreNetworkTypeRestrictions.size(),
            2);
  EXPECT_EQ(
      record.response.changes[0].udmStaticData.coreNetworkTypeRestrictions[0],
      "5GC");
  EXPECT_EQ(
      record.response.changes[0].udmStaticData.coreNetworkTypeRestrictions[1],
      "EPC");
  EXPECT_EQ(record.response.changes[0].udmStaticData.mpsPriority, true);
  EXPECT_EQ(record.response.changes[0].udmStaticData.mcsPriority, false);
  EXPECT_EQ(record.response.changes[0].udmStaticData.dlPacketCount, 100);
  EXPECT_EQ(record.response.changes[0].udmStaticData.micoAllowed, true);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .dnn,
            "DNN1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .ipAddress[0]
                .ipType,
            entities::IpAddrType::IPv4);
  EXPECT_EQ(boost::get<entities::ipv4_t>(
                record.response.changes[0]
                    .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                    .dnnIpList[0]
                    .ipAddress[0]
                    .ip),
            "192.168.10.1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.sst,
            90);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.sd,
            "ABCDEF");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ueUsageType, 12);
  EXPECT_EQ(record.response.changes[0].udmStaticData.rfspIndex, 15);
  EXPECT_EQ(record.response.changes[0].udmStaticData.subsRegTimer, 40);
}

TEST(ValidationDataTest, ValidateUdmCreateSubscriptionOkBitRateWithDot) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1;
  entities::ServiceAreaRestriction serviceArea1;
  entities::NssaiDnnIps ips;
  entities::DnnIps dnnIps;
  entities::ProvJournal provJournal;
  entities::IpAddr ipAddress;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  provJournal.imsi = "imsi123";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.subscribedUeAmbr.uplink = "1.0 Tbps";
  change.udmStaticData.subscribedUeAmbr.downlink = "2.0 Gbps";
  change.udmStaticData.ratRestrictions.push_back("NR");
  change.udmStaticData.ratRestrictions.push_back("EUTRA");
  roamingArea1.id = "roamingArea1";
  change.udmStaticData.forbiddenAreas.push_back(roamingArea1);
  serviceArea1.restrictionType = "ALLOWED_AREAS";
  serviceArea1.areas.push_back(roamingArea1);
  serviceArea1.maxNumOfTas = 1;
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("5GC");
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("EPC");
  change.udmStaticData.mpsPriority = true;
  change.udmStaticData.mcsPriority = false;
  change.udmStaticData.dlPacketCount = 100;
  change.udmStaticData.micoAllowed = true;
  ips.snssai.sst = 90;
  ips.snssai.sd = "ABCDEF";
  dnnIps.dnn = "DNN1";
  ipAddress.ipType = entities::IpAddrType::IPv4;
  entities::ipv4_t ipAddr4 = "192.168.10.1";
  ipAddress.ip = ipAddr4;
  dnnIps.ipAddress.push_back(ipAddress);
  ips.dnnIpList.push_back(dnnIps);
  change.udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(ips);
  change.udmStaticData.ueUsageType = 12;
  change.udmStaticData.rfspIndex = 15;
  change.udmStaticData.subsRegTimer = 40;
  change.udmStaticData.fieldExistence = entities::fieldExistence{
      true, true, true, true, true, true, true, true, true, true, true, false};
  record.changes.push_back(change);
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});
  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.changes[0].udmStaticData.udmSliceProfileId.id,
            "profile1");
  EXPECT_EQ(record.response.changes[0].udmStaticData.subscribedUeAmbr.uplink,
            "1.0 Tbps");
  EXPECT_EQ(record.response.changes[0].udmStaticData.subscribedUeAmbr.downlink,
            "2.0 Gbps");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions.size(), 2);
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions[0], "NR");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions[1],
            "EUTRA");
  EXPECT_EQ(record.response.changes[0].udmStaticData.forbiddenAreas.size(), 1);
  EXPECT_EQ(record.response.changes[0].udmStaticData.forbiddenAreas[0].id,
            "roamingArea1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.restrictionType,
            "ALLOWED_AREAS");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.areas.size(),
            1);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.areas[0]
                .id,
            "roamingArea1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.maxNumOfTas,
            1);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.coreNetworkTypeRestrictions.size(),
            2);
  EXPECT_EQ(
      record.response.changes[0].udmStaticData.coreNetworkTypeRestrictions[0],
      "5GC");
  EXPECT_EQ(
      record.response.changes[0].udmStaticData.coreNetworkTypeRestrictions[1],
      "EPC");
  EXPECT_EQ(record.response.changes[0].udmStaticData.mpsPriority, true);
  EXPECT_EQ(record.response.changes[0].udmStaticData.mcsPriority, false);
  EXPECT_EQ(record.response.changes[0].udmStaticData.dlPacketCount, 100);
  EXPECT_EQ(record.response.changes[0].udmStaticData.micoAllowed, true);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .dnn,
            "DNN1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .ipAddress[0]
                .ipType,
            entities::IpAddrType::IPv4);
  EXPECT_EQ(boost::get<entities::ipv4_t>(
                record.response.changes[0]
                    .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                    .dnnIpList[0]
                    .ipAddress[0]
                    .ip),
            "192.168.10.1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.sst,
            90);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.sd,
            "ABCDEF");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ueUsageType, 12);
  EXPECT_EQ(record.response.changes[0].udmStaticData.rfspIndex, 15);
  EXPECT_EQ(record.response.changes[0].udmStaticData.subsRegTimer, 40);
}

TEST(ValidationDataTest, ValidateUdmUpdateSubscriptionOk) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1;
  entities::ServiceAreaRestriction serviceArea1;
  entities::NssaiDnnIps ips;
  entities::DnnIps dnnIps;
  entities::UdmStaticData relResource;
  entities::IpAddr ipAddress;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profileNew";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  relResource.udmSliceProfileId.id = "profileOld";
  record.relatedResources.insert(
      {"/subscribers/456abc/udmSubscription/udmStaticAndRegistrationData/"
       "udmStaticData",
       relResource});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/456abc/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profileNew";
  change.udmStaticData.subscribedUeAmbr.uplink = "3 Gbps";
  change.udmStaticData.subscribedUeAmbr.downlink = "4 Gbps";
  change.udmStaticData.ratRestrictions.push_back("WLAN");
  change.udmStaticData.ratRestrictions.push_back("VIRTUAL");
  roamingArea1.id = "roamingArea3";
  change.udmStaticData.forbiddenAreas.push_back(roamingArea1);
  serviceArea1.restrictionType = "ALLOWED_AREAS";
  serviceArea1.areas.push_back(roamingArea1);
  serviceArea1.maxNumOfTas = 4;
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("EPC");
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("5GC");
  change.udmStaticData.mpsPriority = false;
  change.udmStaticData.mcsPriority = true;
  change.udmStaticData.dlPacketCount = 101;
  change.udmStaticData.micoAllowed = false;
  ips.snssai.sst = 91;
  ips.snssai.sd = "FACDEE";
  dnnIps.dnn = "DNN2";
  ipAddress.ipType = entities::IpAddrType::IPv4;
  entities::ipv4_t ipAddr4 = "192.168.10.1";
  ipAddress.ip = ipAddr4;
  dnnIps.ipAddress.push_back(ipAddress);
  ips.dnnIpList.push_back(dnnIps);
  change.udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(ips);
  change.udmStaticData.ueUsageType = 13;
  change.udmStaticData.rfspIndex = 16;
  change.udmStaticData.subsRegTimer = 41;
  change.udmStaticData.fieldExistence = entities::fieldExistence{
      true, true, true, true, true, true, true, true, true, true, true, false};
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "UPDATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/subscribers/456abc/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.changes[0].udmStaticData.udmSliceProfileId.id,
            "profileNew");
  EXPECT_EQ(record.response.changes[0].udmStaticData.subscribedUeAmbr.uplink,
            "3 Gbps");
  EXPECT_EQ(record.response.changes[0].udmStaticData.subscribedUeAmbr.downlink,
            "4 Gbps");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions.size(), 2);
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions[0],
            "WLAN");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ratRestrictions[1],
            "VIRTUAL");
  EXPECT_EQ(record.response.changes[0].udmStaticData.forbiddenAreas.size(), 1);
  EXPECT_EQ(record.response.changes[0].udmStaticData.forbiddenAreas[0].id,
            "roamingArea3");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.restrictionType,
            "ALLOWED_AREAS");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.areas.size(),
            1);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.areas[0]
                .id,
            "roamingArea3");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.serviceAreaRestriction.maxNumOfTas,
            4);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.coreNetworkTypeRestrictions.size(),
            2);
  EXPECT_EQ(
      record.response.changes[0].udmStaticData.coreNetworkTypeRestrictions[0],
      "EPC");
  EXPECT_EQ(
      record.response.changes[0].udmStaticData.coreNetworkTypeRestrictions[1],
      "5GC");
  EXPECT_EQ(record.response.changes[0].udmStaticData.mpsPriority, false);
  EXPECT_EQ(record.response.changes[0].udmStaticData.mcsPriority, true);
  EXPECT_EQ(record.response.changes[0].udmStaticData.dlPacketCount, 101);
  EXPECT_EQ(record.response.changes[0].udmStaticData.micoAllowed, false);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .dnn,
            "DNN2");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .dnnIpList[0]
                .ipAddress[0]
                .ipType,
            entities::IpAddrType::IPv4);
  EXPECT_EQ(boost::get<entities::ipv4_t>(
                record.response.changes[0]
                    .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                    .dnnIpList[0]
                    .ipAddress[0]
                    .ip),
            "192.168.10.1");
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.sst,
            91);
  EXPECT_EQ(record.response.changes[0]
                .udmStaticData.staticIpAddress.nssaiDnnIpLists[0]
                .snssai.sd,
            "FACDEE");
  EXPECT_EQ(record.response.changes[0].udmStaticData.ueUsageType, 13);
  EXPECT_EQ(record.response.changes[0].udmStaticData.rfspIndex, 16);
  EXPECT_EQ(record.response.changes[0].udmStaticData.subsRegTimer, 41);
}

TEST(ValidationDataTest, ValidateNoChange) {
  entities::ValidationData record;
  entities::UdmStaticData relResource;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  relResource.udmSliceProfileId.id = "profileOld";
  relResource.subscribedUeAmbr.uplink = "1 Gbps";
  relResource.subscribedUeAmbr.downlink = "2 Gbps";
  record.relatedResources.insert(
      {"/subscribers/abc123/udmSubscription/udmStaticAndRegistrationData/"
       "udmStaticData",
       relResource});

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(ValidationDataTest,
     ValidateUdmCreateSubscriptionServiceAreaRestrictionSizeLimitation) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1, roamingArea2;
  entities::ServiceAreaRestriction serviceArea1;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai678";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/678fgh/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  roamingArea1.id = "roamingArea4";
  roamingArea2.id = "roamingArea5";
  change.udmStaticData.forbiddenAreas.push_back(roamingArea1);
  serviceArea1.restrictionType = "ALLOWED_AREAS";
  serviceArea1.areas.push_back(roamingArea1);
  serviceArea1.areas.push_back(roamingArea2);
  serviceArea1.maxNumOfTas = 1;
  change.udmStaticData.fieldExistence.hasServiceAreaRestriction = true;
  change.udmStaticData.fieldExistence.hasMaxNumOfTas = true;
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  record.relatedResources.insert(
      {"/subscribers/678fgh/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/678fgh/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Only one reference to an udmRoamingArea allowed in "
            "\"serviceAreaRestriction\"");
}

TEST(ValidationDataTest,
     ValidateUdmCreateSubscriptionForbiddenAreasSizeLimitation) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1, roamingArea2;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile10";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "imsi000";
  journal.nai = "nai000";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/000aaa/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile10";
  change.udmStaticData.subscribedUeAmbr.uplink = "100 Gbps";
  change.udmStaticData.subscribedUeAmbr.downlink = "200 Gbps";
  roamingArea1.id = "roamingArea6";
  roamingArea2.id = "roamingArea7";
  change.udmStaticData.forbiddenAreas.push_back(roamingArea1);
  change.udmStaticData.forbiddenAreas.push_back(roamingArea2);
  record.relatedResources.insert(
      {"/subscribers/000aaa/journal/provJournal", journal});
  change.udmStaticData.fieldExistence.hasSubscribedUeAmbr = true;
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/000aaa/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "Only one reference to an udmRoamingArea allowed in \"forbiddenAreas\"");
}

TEST(ValidationDataTest, ValidateSeveralErrors) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1, roamingArea2;
  entities::ServiceAreaRestriction serviceArea1;
  entities::UdmStaticData relResource;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profileNew";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  relResource.udmSliceProfileId.id = "profileOld";
  record.relatedResources.insert(
      {"/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
       "udmStaticData",
       relResource});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profileNew";
  roamingArea1.id = "roamingArea1";
  roamingArea2.id = "roamingArea2";
  change.udmStaticData.forbiddenAreas.push_back(roamingArea1);
  change.udmStaticData.forbiddenAreas.push_back(roamingArea2);
  serviceArea1.restrictionType = "ALLOWED_AREAS";
  serviceArea1.areas.push_back(roamingArea1);
  serviceArea1.areas.push_back(roamingArea2);
  serviceArea1.maxNumOfTas = 1;
  change.udmStaticData.fieldExistence.hasServiceAreaRestriction = true;
  change.udmStaticData.fieldExistence.hasMaxNumOfTas = true;
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Only one reference to an udmRoamingArea allowed in "
            "\"serviceAreaRestriction\"");
  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(
      record.response.errors[1].errorDetails.at("description"),
      "Only one reference to an udmRoamingArea allowed in \"forbiddenAreas\"");
}

TEST(ValidationDataTest, ValidateUdmCreateSubscriptionNeitherImsiDefined) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal provJournal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile10";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  provJournal.username = "user456";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/456bef/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile10";
  record.changes.push_back(change);
  record.relatedResources.insert(
      {"/subscribers/456bef/journal/provJournal", provJournal});

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/456bef/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "There must be an IMSI defined for the subscriber mscId=456bef");
}

TEST(ValidationDataTest,
     ValidateUdmCreateSubscriptionProvJournalNotAssociated) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal provJournal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile10";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  provJournal.imsi = "imsi456";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/456bab/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile10";
  record.changes.push_back(change);
  record.relatedResources.insert(
      {"/subscribers/123cde/journal/provJournal", provJournal});

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_UNPROCESSABLE_ENTITY);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Unprocessable entity");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/456bab/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "provJournal for subscriber mscId=456bab not included. Needed to "
      "check if IMSI is defined: /subscribers/456bab/journal/provJournal");
}

TEST(ValidationDataTest,
     ValidateUdmCreateSubscriptionSliceProfileNotAssociated) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal provJournal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile20";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  provJournal.imsi = "imsi456";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123cde/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile10";
  record.changes.push_back(change);
  record.relatedResources.insert(
      {"/subscribers/123cde/journal/provJournal", provJournal});

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_UNPROCESSABLE_ENTITY);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Unprocessable entity");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/123cde/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "UdmSliceProfile with id=profile10 for subscriber mscId=123cde not "
            "included as "
            "relatedResource.");
}

TEST(ValidationDataTest, GetMscIdOk) {
  std::string resourcePath{
      "/subscribers/000aaa/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData"};
  entities::ValidationData record;
  std::string mscId = record.getMscId(resourcePath);

  EXPECT_EQ(mscId, "000aaa");
}

TEST(ValidationDataTest, BuildProvJournalResourcePathOk) {
  std::string mscId{"123abc"};
  entities::ValidationData record;
  std::string resourcePath =
      record.buildProvJournalResourcePathFromMscId(mscId);

  EXPECT_EQ(resourcePath, "/subscribers/123abc/journal/provJournal");
}

TEST(ValidationDataTest,
     ValidateUdmCreateSubscriptionRestrictionTypeLimitation) {
  entities::ValidationData record;
  entities::Change change;
  entities::ServiceAreaRestriction serviceArea1;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai678";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/678fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  serviceArea1.restrictionType = "ALLOWED_AREAS";
  serviceArea1.maxNumOfTas = 1;
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  change.udmStaticData.fieldExistence.hasServiceAreaRestriction = true;
  change.udmStaticData.fieldExistence.hasMaxNumOfTas = true;
  record.relatedResources.insert(
      {"/subscribers/678fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/678fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"restrictionType\" in \"serviceAreaRestriction\" can be only "
            "present if \"areas\" is present");
}

TEST(ValidationDataTest, ValidateUdmCreateSubscriptionMaxNumOfTAsLimitation) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1;
  entities::ServiceAreaRestriction serviceArea1;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai888";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  roamingArea1.id = "roamingArea6";
  serviceArea1.restrictionType = "NOT_ALLOWED_AREAS";
  serviceArea1.areas.push_back(roamingArea1);
  serviceArea1.maxNumOfTas = 1;
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  change.udmStaticData.fieldExistence.hasServiceAreaRestriction = true;
  change.udmStaticData.fieldExistence.hasMaxNumOfTas = true;
  record.relatedResources.insert(
      {"/subscribers/888fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"maxNumOfTAs\" in \"serviceAreaRestriction\" can be only present "
            "if \"restrictionType\" is not \"NOT_ALLOWED_AREAS\"");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmCreateSubscribedUeAmbrWrongFormat) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1;
  entities::ServiceAreaRestriction serviceArea1;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai888";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.subscribedUeAmbr.uplink = "100 Rbps";
  change.udmStaticData.subscribedUeAmbr.downlink = "aaa Gbps";
  roamingArea1.id = "roamingArea6";
  serviceArea1.restrictionType = "NOT_ALLOWED_AREAS";
  serviceArea1.areas.push_back(roamingArea1);
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  change.udmStaticData.fieldExistence.hasServiceAreaRestriction = true;
  change.udmStaticData.fieldExistence.hasSubscribedUeAmbr = true;
  record.relatedResources.insert(
      {"/subscribers/888fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"uplink\" in \"subscribedUeAmbr\" "
            "has not the valid format");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "\"downlink\" in \"subscribedUeAmbr\" "
            "has not the valid format");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmCreateRatRestrictionsInvalidValue) {
  entities::ValidationData record;
  entities::Change change;

  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai888";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.ratRestrictions.push_back("ELEM1");
  change.udmStaticData.ratRestrictions.push_back("ELEM2");

  record.relatedResources.insert(
      {"/subscribers/888fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"ratRestrictions\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"ratRestrictions\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmCreateRatRestrictionTypeInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::ServiceAreaRestriction serviceArea1;
  entities::UdmRoamingAreaReference roamingArea1;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai888";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  serviceArea1.restrictionType = "KUNG_FU";
  roamingArea1.id = "roamingArea1";
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  change.udmStaticData.serviceAreaRestriction.areas.push_back(roamingArea1);

  record.relatedResources.insert(
      {"/subscribers/888fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "\"restrictionType\" in \"serviceAreaRestriction\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest,
     ValidateUdmCreateCoreNetworkTypeRestrictionsInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai888";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("4GC");

  record.relatedResources.insert(
      {"/subscribers/888fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"coreNetworkTypeRestrictions\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmCreateDlPacketCountInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai888";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.dlPacketCount = -3;
  change.udmStaticData.fieldExistence.hasDlPacketCount = true;

  record.relatedResources.insert(
      {"/subscribers/888fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"dlPacketCount\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmStaticIpAddressInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal journal;
  entities::Snssai sa1, sa2;
  entities::DnnIps dnn1, dnn2;
  entities::IpAddr ip1, ip2, ip3, ip4;
  entities::NssaiDnnIps nssai1, nssai2;
  sa1.sst = 256;
  sa1.sd = "AFEDCC";
  sa2.sst = 2;
  sa2.sd = "sd2";
  dnn1.dnn = "10";
  ip1.ipType = entities::IpAddrType::IPv4;
  entities::ipv4_t ipAddr4 = "192.";
  ip1.ip = ipAddr4;
  ip2.ipType = entities::IpAddrType::IPv6;
  entities::ipv6_t ipAddr5 = "21.1.1";
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

  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai888";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(nssai1);
  change.udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(nssai2);
  change.udmStaticData.fieldExistence.hasStaticIpAddress = true;

  record.relatedResources.insert(
      {"/subscribers/888fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 5);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"sst\" in \"staticIpAddress\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "\"ipv4Addr\" in \"staticIpAddress\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[2].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[2].errorDetails.at("description"),
            "\"ipv6Addr\" in \"staticIpAddress\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[2].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[3].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[3].errorDetails.at("description"),
            "\"sd\" in \"staticIpAddress\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[3].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[4].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[4].errorDetails.at("description"),
            "\"ipv6Prefix\" in \"staticIpAddress\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[4].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmStaticIpAddressInvalidSize) {
  entities::ValidationData record;
  entities::Change change;
  entities::UdmRoamingAreaReference roamingArea1;
  entities::ServiceAreaRestriction serviceArea1;
  entities::NssaiDnnIps ips;
  entities::DnnIps dnnIps;
  entities::ProvJournal provJournal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  provJournal.imsi = "imsi123";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/111aaa/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.subscribedUeAmbr.uplink = "1 Gbps";
  change.udmStaticData.subscribedUeAmbr.downlink = "2 Gbps";
  change.udmStaticData.ratRestrictions.push_back("NR");
  change.udmStaticData.ratRestrictions.push_back("EUTRA");
  roamingArea1.id = "roamingArea1";
  change.udmStaticData.forbiddenAreas.push_back(roamingArea1);
  serviceArea1.restrictionType = "ALLOWED_AREAS";
  serviceArea1.areas.push_back(roamingArea1);
  serviceArea1.maxNumOfTas = 1;
  change.udmStaticData.serviceAreaRestriction = serviceArea1;
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("5GC");
  change.udmStaticData.coreNetworkTypeRestrictions.push_back("EPC");
  change.udmStaticData.mpsPriority = true;
  change.udmStaticData.mcsPriority = false;
  change.udmStaticData.dlPacketCount = 100;
  change.udmStaticData.micoAllowed = true;
  ips.snssai.sst = 90;
  ips.snssai.sd = "ABCDEF";
  dnnIps.dnn = "DNN1";
  ips.dnnIpList.push_back(dnnIps);
  change.udmStaticData.staticIpAddress.nssaiDnnIpLists.push_back(ips);
  change.udmStaticData.ueUsageType = 12;
  change.udmStaticData.rfspIndex = 15;
  change.udmStaticData.subsRegTimer = 40;
  change.udmStaticData.fieldExistence = entities::fieldExistence{
      true, true, true, true, true, true, true, true, true, true, true, false};
  record.changes.push_back(change);
  record.relatedResources.insert(
      {"/subscribers/111aaa/journal/provJournal", provJournal});

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"ipAddress\" in \"dnnIpList\" has an Invalid Size. It should "
            "have at least 1 and at most 2 elements");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/111aaa/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmCreateRoamingOdbInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai888";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.roamingOdb = "HOME_OUTSIDE_COUNTRY";
  change.udmStaticData.fieldExistence.hasDlPacketCount = false;

  record.relatedResources.insert(
      {"/subscribers/888fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"roamingOdb\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/888fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest,
     ValidateUdmSliceProfileplmnIdSetIdentifierInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "111";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 254;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"plmnIdSetIdentifier\" has not the valid format");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[1].errorDetails.at("description"),
      "At least a \"plmnIdSetIdentifier\" with \"Default\" value must exist");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfilednnListsnvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::DnnList dnnList;
  entities::DnnData dnnData;
  entities::NssaiDetails defaultSingleNnsaiDetail;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.threegppChargingCharacteristics = "ABCX";
  defaultSingleNnsaiDetail.nssai.sst = 254;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "PEPEP";
  dnnData.sscModes.allowedSscModes.push_back("MEEEE");
  dnnData.pduSessionTypes.defaultSessionType = "AAAA";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("AAAAA");
  dnnData.fivegQosProfile.fiveqi = -1;
  dnnData.fivegQosProfile.priorityLevel = 128;
  dnnData.fivegQosProfile.arp.priorityLevel = 16;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMP";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PRMPTABLE";
  dnnData.sessionAmbr.downlink = "1 mgb";
  dnnData.sessionAmbr.uplink = "2 mgb";
  dnnData.upSecurity.upIntegr = "rquired";
  dnnData.upSecurity.upConfid = "Prferred";
  dnnData.dnn = "Default";

  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData.threegppChargingCharacteristics = "ABCX";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 15);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"3gppChargingCharacteristics\" in \"defaultSingleNssais\" has "
            "not the valid format");

  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "\"defaultSscMode\" in \"sscModes\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[2].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[2].errorDetails.at("description"),
            "\"allowedSscModes\" in \"sscModes\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[2].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[3].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[3].errorDetails.at("description"),
      "\"defaultSessionType\" in \"pduSessionTypes\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[3].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[4].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[4].errorDetails.at("description"),
      "\"allowedSessionTypes\" in \"pduSessionTypes\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[4].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[5].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[5].errorDetails.at("description"),
            "\"5qi\" in \"5gQosProfile\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[5].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[6].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[6].errorDetails.at("description"),
            "\"priorityLevel\" in \"5gQosProfile\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[6].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[7].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[7].errorDetails.at("description"),
            "\"priorityLevel\" in \"arp\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[7].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[8].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[8].errorDetails.at("description"),
            "\"preemptCap\" in \"arp\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[8].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[9].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[9].errorDetails.at("description"),
            "\"preemptVuln\" in \"arp\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[9].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[10].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[10].errorDetails.at("description"),
            "\"uplink\" in \"sessionAmbr\" has not the valid format");
  EXPECT_EQ(record.response.errors[10].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[11].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[11].errorDetails.at("description"),
            "\"downlink\" in \"sessionAmbr\" has not the valid format");
  EXPECT_EQ(record.response.errors[11].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[12].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[12].errorDetails.at("description"),
            "\"upIntegr\" in \"upSecurity\" has not the valid format");
  EXPECT_EQ(record.response.errors[12].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[13].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[13].errorDetails.at("description"),
            "\"upConfid\" in \"upSecurity\" has not the valid format");
  EXPECT_EQ(record.response.errors[13].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[14].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[14].errorDetails.at("description"),
            "\"3gppChargingCharacteristics\" in \"dnnDataList\" has not the "
            "valid format");

  EXPECT_EQ(record.response.errors[14].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfilednnIndicatorNotOk) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 254;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "*";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"defaultDnnIndicator\" in \"dnnDataList\" cannot be set to true "
            "when \"dnn\" is \"*\"");

  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfilednnIndicatorOk) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 254;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "*";

  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .plmnIdSetIdentifier,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            254);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .dnnListId,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sst,
            253);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_3");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV6");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            10);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            124);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            13);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "2 Gbps");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "1 Gbps");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnListId,
            "Default");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileDefaultDnnValueNotOk) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::DnnList dnnList;
  entities::DnnData dnnData;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = false;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "At least a \"defaultDnnIndicator\" with \"true\" value must exist");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileOk) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::DnnList dnnList;
  entities::DnnData dnnData;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;
  dnnData.smfList.push_back("daf98e6c-70ff-11eb-9439-0242ac130002");
  dnnData.sameSmfInd = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;
  dnnData.fieldExistence.hasSmfList = true;
  dnnData.fieldExistence.hasSameSmfInd = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .plmnIdSetIdentifier,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            2);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .dnnListId,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sst,
            3);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_3");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV6");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            10);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            124);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            13);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "2 Gbps");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "1 Gbps");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnListId,
            "Default");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .smfList[0],
            "daf98e6c-70ff-11eb-9439-0242ac130002");
  EXPECT_EQ(record.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sameSmfInd,
            true);
}

TEST(ValidationDataTest, ValidateUdmSliceProfileDnnListIdPointToDnnLists) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 252;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "21";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "\"dnnListId\" in \"nssaiDetails\" must point to existing \"dnnLists\"");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRepeatedDnns) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;
  entities::DnnData dnnData3;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 252;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "22";

  dnnData3.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData3.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData3.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData3.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData3.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData3.fivegQosProfile.fiveqi = 10;
  dnnData3.fivegQosProfile.priorityLevel = 124;
  dnnData3.fivegQosProfile.arp.priorityLevel = 13;
  dnnData3.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData3.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData3.sessionAmbr.downlink = "1 Gbps";
  dnnData3.sessionAmbr.uplink = "2 Gbps";
  dnnData3.upSecurity.upIntegr = "REQUIRED";
  dnnData3.upSecurity.upConfid = "PREFERRED";
  dnnData3.dnn = "22";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"dnn\" is duplicated in \"dnnLists\"");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRepeatedDnnsCaseInsensitive) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;
  entities::DnnData dnnData3;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 252;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "aa";

  dnnData3.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData3.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData3.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData3.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData3.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData3.fivegQosProfile.fiveqi = 10;
  dnnData3.fivegQosProfile.priorityLevel = 124;
  dnnData3.fivegQosProfile.arp.priorityLevel = 13;
  dnnData3.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData3.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData3.sessionAmbr.downlink = "1 Gbps";
  dnnData3.sessionAmbr.uplink = "2 Gbps";
  dnnData3.upSecurity.upIntegr = "REQUIRED";
  dnnData3.upSecurity.upConfid = "PREFERRED";
  dnnData3.dnn = "AA";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"dnn\" is duplicated in \"dnnLists\"");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileDefaultNssaiInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 256;
  defaultSingleNnsaiDetail.nssai.sd = "wwwwww";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"sst\" in \"defaultSingleNssais\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "\"sd\" in \"defaultSingleNssais\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileSingleNssaiInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 256;
  singleNnsaiDetail.nssai.sd = "wwwwww";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";

  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";

  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"sst\" in \"singleNssais\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "\"sd\" in \"singleNssais\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileNoDefaultSscMode) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "AAABBB";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_1");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";

  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"defaultSscMode\" in \"sscModes\" is a required Field");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileAllowedSscModeInvalidSize) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "AAABBB";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_1");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"allowedSscModes\" in \"sscModes\" has an Invalid Size");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileNoDefaultSessionType) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "AAABBB";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV4");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "\"defaultSessionType\" in \"pduSessionTypes\" is a required Field");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileAllowedSessionTypeInvalidSize) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "AAABBB";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  // dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV4");
  // dnnData.pduSessionTypes.allowedSessionTypes.push_back("ETHERNET");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";

  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(
      record.response.errors[0].errorDetails.at("description"),
      "\"allowedSessionTypes\" in \"pduSessionTypes\" has an Invalid Size");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileAllowedSessionTypeMissingOk) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "AAABBB";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";

  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = false;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .plmnIdSetIdentifier,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "22");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            253);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .dnnListId,
            "22");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sst,
            253);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sd,
            "AAABBB");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_3");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            10);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            124);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            13);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "2 Gbps");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "1 Gbps");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnListId,
            "Default");
}

TEST(ValidationDataTest, ValidateCreateUdmRoamingAreaOk) {
  entities::ValidationData record;
  entities::Change change;
  entities::area_list_t area1, area2;
  entities::udm_roaming_area_t roam1;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmRoamingAreas/1234";
  area1.area = std::string{"area_code_1"};
  roam1.areaList.push_back(area1);
  area2.area = std::vector<std::string>{"123f", "abcdef"};
  roam1.areaList.push_back(area2);
  change.udmRoamingArea = roam1;
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/profiles/udmProfiles/udmRoamingAreas/1234");
  EXPECT_EQ(boost::get<entities::area_code_t>(
                record.response.changes[0].udmRoamingArea.areaList[0].area),
            "area_code_1");
  entities::tacs_t tacs = boost::get<entities::tacs_t>(
      record.response.changes[0].udmRoamingArea.areaList[1].area);
  EXPECT_EQ(tacs[0], "123f");
  EXPECT_EQ(tacs[1], "abcdef");
}

TEST(ValidationDataTest, ValidateCreateUdmRoamingAreaTacsWrongFormat) {
  entities::ValidationData record;
  entities::Change change;
  entities::area_list_t area1, area2;
  entities::udm_roaming_area_t roam1;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmRoamingAreas/2222";
  area1.area = std::string{"area_code"};
  roam1.areaList.push_back(area1);
  area2.area = std::vector<std::string>{"123f", "abcdef111", "12345"};
  roam1.areaList.push_back(area2);
  change.udmRoamingArea = roam1;
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"tacs\" in \"areaList\" has an invalid format");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/2222");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "\"tacs\" in \"areaList\" has an invalid format");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/2222");
}

TEST(ValidationDataTest, ValidateCreateUdmRoamingAreaWrongSizeAreaList) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_roaming_area_t roam1;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmRoamingAreas/3333";
  change.udmRoamingArea = roam1;
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "At least one \"areaList\" must exist");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/3333");
}

TEST(ValidationDataTest, ValidateCreateUdmRoamingAreaWrongSizeTacs) {
  entities::ValidationData record;
  entities::Change change;
  entities::area_list_t area1, area2;
  entities::udm_roaming_area_t roam1;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmRoamingAreas/aaaa";
  area1.area = std::string{"area_1"};
  roam1.areaList.push_back(area1);
  area2.area = std::vector<std::string>{};
  roam1.areaList.push_back(area2);
  change.udmRoamingArea = roam1;
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "At least one element in \"tacs\" must exist");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmRoamingAreas/aaaa");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRepeatedNssais) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::NssaiDetails defaultSingleNnsaiDetail2;
  entities::NssaiDetails singleNnsaiDetail2;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  defaultSingleNnsaiDetail2.dnnListId = "22";
  defaultSingleNnsaiDetail2.nssai.sst = 253;
  defaultSingleNnsaiDetail2.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 251;
  singleNnsaiDetail.nssai.sd = "AAABBB";

  singleNnsaiDetail2.dnnListId = "22";
  singleNnsaiDetail2.nssai.sst = 251;
  singleNnsaiDetail2.nssai.sd = "AAABBB";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail2);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail2);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");

  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "There are \"nssai\" repeated in the \"nssai\" attribute");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "There are \"nssai\" repeated in the \"nssai\" attribute");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRepeatedNssaisCaseInsensitive) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::NssaiDetails defaultSingleNnsaiDetail2;
  entities::NssaiDetails singleNnsaiDetail2;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  defaultSingleNnsaiDetail2.dnnListId = "22";
  defaultSingleNnsaiDetail2.nssai.sst = 253;
  defaultSingleNnsaiDetail2.nssai.sd = "AbCdEf";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 251;
  singleNnsaiDetail.nssai.sd = "AAAbbb";

  singleNnsaiDetail2.dnnListId = "22";
  singleNnsaiDetail2.nssai.sst = 251;
  singleNnsaiDetail2.nssai.sd = "aaaBBB";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail2);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail2);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");

  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "There are \"nssai\" repeated in the \"nssai\" attribute");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "There are \"nssai\" repeated in the \"nssai\" attribute");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRepeatedAllowedSessionType) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "AAABBB";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV4");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";

  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "There are duplicated PDU session types between in "
            "\"defaultSessionType\" and "
            "\"allowedSessionTypes\"");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRepeatedAllowedSscModes) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 253;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "AAABBB";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";

  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "There are duplicated SSC modes between in \"defaultSscMode\" and "
            "\"allowedSscModes\"");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRepeatedDnnInDifferentDnnList) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 254;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .plmnIdSetIdentifier,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "22");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            254);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .dnnListId,
            "22");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sst,
            253);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_3");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV6");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            10);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            124);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            13);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "2 Gbps");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "1 Gbps");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnListId,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[1]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[1]
                .dnnListId,
            "22");
}

TEST(ValidationDataTest,
     ValidateUdmSliceProfileMutationNodnnListIdInDefaultSingleNssai) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 254;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "22";
  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .plmnIdSetIdentifier,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            254);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .dnnListId,
            "22");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sst,
            253);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_3");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV6");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            10);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            124);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            13);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "2 Gbps");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "1 Gbps");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnListId,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[1]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[1]
                .dnnListId,
            "22");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileNoSingleNssai) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 254;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = false;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .plmnIdSetIdentifier,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "22");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            254);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_3");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV6");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            10);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            124);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            13);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "2 Gbps");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "1 Gbps");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnListId,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[1]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[1]
                .dnnListId,
            "22");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileNodnnListIdSingleNssai) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;
  entities::DnnList dnnList;
  entities::DnnList dnnList2;
  entities::DnnData dnnData;
  entities::DnnData dnnData2;

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";
  plmnIdSets.plmnIdSetIdentifier = "Default";
  defaultSingleNnsaiDetail.dnnListId = "22";
  defaultSingleNnsaiDetail.nssai.sst = 254;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.nssai.sst = 253;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";

  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasLboRoamingAllowed = true;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData2.sscModes.allowedSscModes.push_back("SSC_MODE_2");
  dnnData2.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData2.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData2.fivegQosProfile.fiveqi = 10;
  dnnData2.fivegQosProfile.priorityLevel = 124;
  dnnData2.fivegQosProfile.arp.priorityLevel = 13;
  dnnData2.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData2.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData2.sessionAmbr.downlink = "1 Gbps";
  dnnData2.sessionAmbr.uplink = "2 Gbps";
  dnnData2.upSecurity.upIntegr = "REQUIRED";
  dnnData2.upSecurity.upConfid = "PREFERRED";
  dnnData2.dnn = "Default";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasLboRoamingAllowed = false;
  dnnData2.fieldExistence.hasIwkEpsInd = false;
  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData2.fieldExistence.hasPduSessionTypes = true;
  dnnData2.fieldExistence.hasAllowedSessionTypes = true;
  dnnData2.fieldExistence.hasSscModes = true;
  dnnData2.fieldExistence.hasSessionAmbr = true;
  dnnData2.fieldExistence.hasFivegQosProfile = true;
  dnnData2.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData2.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList2.dnnListId = "22";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList);
  plmnIdSets.dnnLists.push_back(dnnList2);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
  EXPECT_EQ(record.response.changes[0].operation, "CREATE");
  EXPECT_EQ(record.response.changes[0].resourcePath,
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .plmnIdSetIdentifier,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .dnnListId,
            "22");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sst,
            254);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.defaultSingleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .dnnListId,
            "Default");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sst,
            253);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .nssai.singleNssais[0]
                .nssai.sd,
            "ABCDEF");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.defaultSscMode,
            "SSC_MODE_1");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[0],
            "SSC_MODE_3");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sscModes.allowedSscModes[1],
            "SSC_MODE_2");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.defaultSessionType,
            "IPV4");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .pduSessionTypes.allowedSessionTypes[0],
            "IPV6");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.fiveqi,
            10);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.priorityLevel,
            124);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.priorityLevel,
            13);
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptCap,
            "NOT_PREEMPT");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .fivegQosProfile.arp.preemptVuln,
            "NOT_PREEMPTABLE");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.uplink,
            "2 Gbps");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .sessionAmbr.downlink,
            "1 Gbps");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upIntegr,
            "REQUIRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .upSecurity.upConfid,
            "PREFERRED");
  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[0]
                .dnnListId,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[1]
                .dnnDataList[0]
                .dnn,
            "Default");

  EXPECT_EQ(record.response.changes[0]
                .udmSliceProfile.plmnIdSets[0]
                .dnnLists[1]
                .dnnListId,
            "22");
}

TEST(ValidationDataTest,
     ValidateUdmSubscriptionFirstChangeIsWrongSecondChangeIsOk) {
  entities::ValidationData record;
  entities::Change change1, change2;
  entities::ProvJournal provJournal;
  entities::udm_slice_prof_t sliceProf, sliceProf2;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  sliceProf2.udmSliceProfileId = "profile2";
  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/2", sliceProf2});

  provJournal.imsi = "imsi123";

  change1.operation = "CREATE";
  change1.resourcePath =
      "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change1.udmStaticData.udmSliceProfileId.id = "profile1";
  record.changes.push_back(change1);

  record.relatedResources.insert(
      {"/subscribers/333aaa/journal/provJournal", provJournal});

  change2.operation = "CREATE";
  change2.resourcePath =
      "/subscribers/333aaa/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change2.udmStaticData.udmSliceProfileId.id = "profile2";
  record.changes.push_back(change2);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_UNPROCESSABLE_ENTITY);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Unprocessable entity");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "provJournal for subscriber mscId=123abc not included. Needed to "
            "check if IMSI is defined: "
            "/subscribers/123abc/journal/provJournal");
}

TEST(ValidationDataTest,
     ValidateUdmSubscriptionFirstChangeIsWrongSecondChangeIsWrong) {
  entities::ValidationData record;
  entities::Change change1, change2;
  entities::ProvJournal provJournal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  provJournal.imsi = "imsi123";

  change1.operation = "CREATE";
  change1.resourcePath =
      "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change1.udmStaticData.udmSliceProfileId.id = "profile1";
  record.changes.push_back(change1);

  record.relatedResources.insert(
      {"/subscribers/333aaa/journal/provJournal", provJournal});

  change2.operation = "UPDATE";
  change2.resourcePath =
      "/subscribers/333aaa/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  record.changes.push_back(change2);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_UNPROCESSABLE_ENTITY);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);

  EXPECT_EQ(record.response.errors[0].errorMessage, "Unprocessable entity");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/123abc/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "provJournal for subscriber mscId=123abc not included. Needed to "
            "check if IMSI is defined: "
            "/subscribers/123abc/journal/provJournal");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Unprocessable entity");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/subscribers/333aaa/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "There is no associated relatedResource: "
            "/subscribers/333aaa/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmUeUsageTypeMinInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai999";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/999fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.ueUsageType = -1;
  change.udmStaticData.fieldExistence.hasUeUsageType = true;

  record.relatedResources.insert(
      {"/subscribers/999fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"ueUsageType\" has an Invalid Value");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/999fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
}

TEST(ValidationDataTest, ValidateUdmUeUsageTypeMaxInvalidValue) {
  entities::ValidationData record;
  entities::Change change;
  entities::ProvJournal journal;
  entities::udm_slice_prof_t sliceProf;

  sliceProf.udmSliceProfileId = "profile1";

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1", sliceProf});

  journal.imsi = "nai000";
  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/000fff/udmSubscription/udmStaticAndRegistrationData/"
      "udmStaticData";
  change.udmStaticData.udmSliceProfileId.id = "profile1";
  change.udmStaticData.ueUsageType = 256;
  change.udmStaticData.fieldExistence.hasUeUsageType = true;

  record.relatedResources.insert(
      {"/subscribers/000fff/journal/provJournal", journal});
  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);
  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/000fff/udmSubscription/udmStaticAndRegistrationData/"
            "udmStaticData");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"ueUsageType\" has an Invalid Value");
}

TEST(ValidationDataTest,
     ValidateUdmSliceProfileUpdateDnnsDuplicatedButNotInRelatedResources) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.dnn = "ABC";

  dnnData3.dnn = "abc";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);
  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"dnn\" is duplicated in \"dnnLists\"");
}

TEST(
    ValidationDataTest,
    ValidateUdmSliceProfileUpdateDnnsDuplicatedButNotInRelatedResourcesWithSeveralPlmnIds) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSets2, plmnIdSetsRelResource,
      plmnIdSetsRelResource2;
  entities::DnnList dnnList, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.dnn = "ABC";

  dnnData3.dnn = "abc";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  plmnIdSets2.plmnIdSetIdentifier = "12345";
  plmnIdSets2.dnnLists.push_back(dnnList);
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets2);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";

  plmnIdSetsRelResource2.plmnIdSetIdentifier = "12345";
  plmnIdSetsRelResource2.dnnLists.push_back(dnnListRelResource);

  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource2);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"dnn\" is duplicated in \"dnnLists\"");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "\"dnn\" is duplicated in \"dnnLists\"");
}

TEST(ValidationDataTest,
     ValidateUdmSliceProfileUpdateDnnsNotDuplicatedButDoInRelatedResources) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.dnn = "ABC";

  dnnData3.dnn = "DEF";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);

  dnnData2.dnn = "ABC";
  dnnData3.dnn = "abc";
  dnnListRelResource.dnnDataList.push_back(dnnData2);
  dnnListRelResource.dnnDataList.push_back(dnnData3);

  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(ValidationDataTest,
     ValidateUdmSliceProfileUpdateDnnsDuplicatedAndInRelatedResources) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.dnn = "ABC";

  dnnData3.dnn = "abc";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);

  dnnData2.dnn = "ABC";
  dnnData3.dnn = "abc";
  dnnListRelResource.dnnDataList.push_back(dnnData2);
  dnnListRelResource.dnnDataList.push_back(dnnData3);

  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(ValidationDataTest, ValidateUdmSliceProfileAddDnnsDuplicated) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3, dnnData4;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.dnn = "ABC";

  dnnData3.dnn = "abc";

  /* Add one extra dnn duplicated. This should fail even if there are others
   * duplicated in related resource */
  dnnData4.dnn = "ABc";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  dnnList.dnnDataList.push_back(dnnData4);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);

  dnnData2.dnn = "ABC";
  dnnData3.dnn = "abc";
  dnnListRelResource.dnnDataList.push_back(dnnData2);
  dnnListRelResource.dnnDataList.push_back(dnnData3);

  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"dnn\" is duplicated in \"dnnLists\"");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRemoveDnnsDuplicated) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3, dnnData4;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.dnn = "ABC";

  dnnData3.dnn = "abc";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  dnnList.dnnDataList.push_back(dnnData4);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);

  dnnData2.dnn = "ABC";
  dnnData3.dnn = "abc";
  dnnData4.dnn = "ABc";
  dnnListRelResource.dnnDataList.push_back(dnnData2);
  dnnListRelResource.dnnDataList.push_back(dnnData3);
  /* Remove this dnn duplicated. This should be ok */
  dnnListRelResource.dnnDataList.push_back(dnnData4);

  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(ValidationDataTest, ValidateUdmSliceProfileUpdateDnnsDuplicated) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnData2.dnn = "ABC";

  /* Update this dnn */
  dnnData3.dnn = "DEF";

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  dnnList.dnnDataList.push_back(dnnData2);
  dnnList.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);

  dnnData2.dnn = "ABC";
  dnnData3.dnn = "abc";
  dnnListRelResource.dnnDataList.push_back(dnnData2);
  dnnListRelResource.dnnDataList.push_back(dnnData3);

  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(ValidationDataTest,
     ValidateUdmSliceProfileUpdateNssaisDuplicatedButNotInRelatedResources) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnList2, dnnList3, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail, defaultSingleNssaiDetail2;
  entities::NssaiDetails singleNnsaiDetail, singleNssaiDetail2;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  defaultSingleNssaiDetail2.dnnListId = "1";
  defaultSingleNssaiDetail2.nssai.sst = 2;
  defaultSingleNssaiDetail2.nssai.sd = "abcdef";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNssaiDetail2.dnnListId = "2";
  singleNssaiDetail2.nssai.sst = 3;
  singleNssaiDetail2.nssai.sd = "ABCdef";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNssaiDetail2);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNssaiDetail2);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  dnnData2.dnn = "1";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList2.dnnListId = "1";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList2);

  dnnData3.dnn = "2";
  dnnData3.defaultDnnIndicator = true;

  dnnData3.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList3.dnnListId = "2";
  dnnList3.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList3);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);
  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNnsaiDetail);
  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNnsaiDetail);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 2);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "There are \"nssai\" repeated in the \"nssai\" attribute");

  EXPECT_EQ(record.response.errors[1].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[1].errorDetails.at("description"),
            "There are \"nssai\" repeated in the \"nssai\" attribute");
}

TEST(ValidationDataTest,
     ValidateUdmSliceProfileUpdateNssaisDuplicatedAlsoInRelatedResources) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnList2, dnnList3, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail, defaultSingleNssaiDetail2;
  entities::NssaiDetails singleNnsaiDetail, singleNssaiDetail2;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  defaultSingleNssaiDetail2.dnnListId = "1";
  defaultSingleNssaiDetail2.nssai.sst = 2;
  defaultSingleNssaiDetail2.nssai.sd = "abcdef";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNssaiDetail2.dnnListId = "2";
  singleNssaiDetail2.nssai.sst = 3;
  singleNssaiDetail2.nssai.sd = "ABCdef";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNssaiDetail2);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNssaiDetail2);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  dnnData2.dnn = "1";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList2.dnnListId = "1";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList2);

  dnnData3.dnn = "2";
  dnnData3.defaultDnnIndicator = true;

  dnnData3.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList3.dnnListId = "2";
  dnnList3.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList3);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);
  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNnsaiDetail);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNssaiDetail2);
  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNssaiDetail2);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(ValidationDataTest, ValidateUdmSliceProfileAddNssaisDuplicated) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnList2, dnnList3, dnnList4, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3, dnnData4;

  entities::NssaiDetails defaultSingleNnsaiDetail, defaultSingleNssaiDetail2;
  entities::NssaiDetails singleNnsaiDetail, singleNssaiDetail2,
      singleNssaiDetail3;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  defaultSingleNssaiDetail2.dnnListId = "1";
  defaultSingleNssaiDetail2.nssai.sst = 2;
  defaultSingleNssaiDetail2.nssai.sd = "abcdef";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNssaiDetail2.dnnListId = "2";
  singleNssaiDetail2.nssai.sst = 3;
  singleNssaiDetail2.nssai.sd = "ABCdef";

  singleNssaiDetail3.dnnListId = "3";
  singleNssaiDetail3.nssai.sst = 3;
  singleNssaiDetail3.nssai.sd = "AbCdEf";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNssaiDetail2);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNssaiDetail2);
  plmnIdSets.nssai.singleNssais.push_back(singleNssaiDetail3);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  dnnData2.dnn = "1";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList2.dnnListId = "1";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList2);

  dnnData3.dnn = "2";
  dnnData3.defaultDnnIndicator = true;

  dnnData3.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList3.dnnListId = "2";
  dnnList3.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList3);

  dnnData4.dnn = "3";
  dnnData4.defaultDnnIndicator = true;

  dnnData4.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList4.dnnListId = "3";
  dnnList4.dnnDataList.push_back(dnnData4);
  plmnIdSets.dnnLists.push_back(dnnList4);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);
  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNnsaiDetail);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNssaiDetail2);
  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNssaiDetail2);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "There are \"nssai\" repeated in the \"nssai\" attribute");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileRemoveNssaisDuplicated) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnList2, dnnList3, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail, defaultSingleNssaiDetail2;
  entities::NssaiDetails singleNnsaiDetail, singleNssaiDetail2;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  defaultSingleNssaiDetail2.dnnListId = "1";
  defaultSingleNssaiDetail2.nssai.sst = 2;
  defaultSingleNssaiDetail2.nssai.sd = "abcdef";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNssaiDetail2);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  dnnData2.dnn = "1";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList2.dnnListId = "1";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList2);

  dnnData3.dnn = "2";
  dnnData3.defaultDnnIndicator = true;

  dnnData3.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList3.dnnListId = "2";
  dnnList3.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList3);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);
  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNnsaiDetail);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNssaiDetail2);
  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNnsaiDetail);

  singleNssaiDetail2.dnnListId = "2";
  singleNssaiDetail2.nssai.sst = 3;
  singleNssaiDetail2.nssai.sd = "ABCdef";

  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNssaiDetail2);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(ValidationDataTest, ValidateUdmSliceProfileUpdateNssaisDuplicated) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource;
  entities::DnnList dnnList, dnnList2, dnnList3, dnnListRelResource;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail, defaultSingleNssaiDetail2;
  entities::NssaiDetails singleNnsaiDetail, singleNssaiDetail2;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  defaultSingleNssaiDetail2.dnnListId = "1";
  defaultSingleNssaiDetail2.nssai.sst = 2;
  defaultSingleNssaiDetail2.nssai.sd = "abcdef";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNssaiDetail2.dnnListId = "2";
  singleNssaiDetail2.nssai.sst = 3;
  singleNssaiDetail2.nssai.sd = "ABCAAA";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNssaiDetail2);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNssaiDetail2);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  dnnData2.dnn = "1";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList2.dnnListId = "1";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList2);

  dnnData3.dnn = "2";
  dnnData3.defaultDnnIndicator = true;

  dnnData3.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList3.dnnListId = "2";
  dnnList3.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList3);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);
  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNnsaiDetail);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNssaiDetail2);
  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNnsaiDetail);

  singleNssaiDetail2.nssai.sd = "ABCdef";

  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNssaiDetail2);
  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);

  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(ValidationDataTest, ValidateUdmSliceProfileDnnListIdsCannotBeDeleted) {
  entities::ValidationData record;
  entities::Change change;
  entities::udm_slice_prof_t sliceProfile;
  entities::PlmnIdSets plmnIdSets, plmnIdSetsRelResource,
      plmnIdSetsRelResource2;
  entities::DnnList dnnList, dnnList2, dnnList3, dnnListRelResource,
      dnnListRelResource2;
  entities::DnnData dnnData, dnnData2, dnnData3;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail, singleNssaiDetail2;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "1";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNssaiDetail2.dnnListId = "2";
  singleNssaiDetail2.nssai.sst = 3;
  singleNssaiDetail2.nssai.sd = "ABCAAA";

  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNssaiDetail2);

  change.operation = "UPDATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  dnnData2.dnn = "1";
  dnnData2.defaultDnnIndicator = true;

  dnnData2.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList2.dnnListId = "1";
  dnnList2.dnnDataList.push_back(dnnData2);
  plmnIdSets.dnnLists.push_back(dnnList2);

  dnnData3.dnn = "2";
  dnnData3.defaultDnnIndicator = true;

  dnnData3.fieldExistence.hasDefaultDnnIndicator = true;

  dnnList3.dnnListId = "2";
  dnnList3.dnnDataList.push_back(dnnData3);
  plmnIdSets.dnnLists.push_back(dnnList3);

  change.udmSliceProfile.udmSliceProfileId = "Profile1111";
  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  /* Related resource*/
  sliceProfile.udmSliceProfileId = "Profile1111";
  dnnListRelResource.dnnListId = "Default";
  dnnListRelResource.dnnDataList.push_back(dnnData);

  dnnListRelResource2.dnnListId = "deleted";
  dnnListRelResource2.dnnDataList.push_back(dnnData2);

  plmnIdSetsRelResource.plmnIdSetIdentifier = "Default";
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource);
  plmnIdSetsRelResource.dnnLists.push_back(dnnListRelResource2);
  plmnIdSetsRelResource.nssai.defaultSingleNssais.push_back(
      defaultSingleNnsaiDetail);
  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNnsaiDetail);

  singleNssaiDetail2.nssai.sd = "ABCdef";

  plmnIdSetsRelResource.nssai.singleNssais.push_back(singleNssaiDetail2);

  sliceProfile.plmnIdSets.push_back(plmnIdSetsRelResource);

  record.relatedResources.insert(
      {"/profiles/udmProfiles/udmSliceProfiles/1111", sliceProfile});

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "\"dnnListId\"=\"deleted\" cannot be deleted for "
            "\"plmnIdSetIdentifier\"=\"Default\"");
}

TEST(ValidationDataTest, ValidateUdmSliceProfileWrongSmfListFormat) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::DnnList dnnList;
  entities::DnnData dnnData;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;
  dnnData.smfList.push_back("smfList1");
  dnnData.sameSmfInd = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;
  dnnData.fieldExistence.hasSmfList = true;
  dnnData.fieldExistence.hasSameSmfInd = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "Element in \"smfList\" has an invalid format");
}

TEST(ValidationDataTest, ValidateUdmSliceProfilesmfListHasNoElements) {
  entities::ValidationData record;
  entities::Change change;
  entities::PlmnIdSets plmnIdSets;
  entities::DnnList dnnList;
  entities::DnnData dnnData;

  entities::NssaiDetails defaultSingleNnsaiDetail;
  entities::NssaiDetails singleNnsaiDetail;

  defaultSingleNnsaiDetail.dnnListId = "Default";
  defaultSingleNnsaiDetail.nssai.sst = 2;
  defaultSingleNnsaiDetail.nssai.sd = "ABCDEF";

  singleNnsaiDetail.dnnListId = "Default";
  singleNnsaiDetail.nssai.sst = 3;
  singleNnsaiDetail.nssai.sd = "ABCDEF";
  plmnIdSets.fieldExistenceUdmSliceProfile.hasNssai = true;
  plmnIdSets.nssai.defaultSingleNssais.push_back(defaultSingleNnsaiDetail);
  plmnIdSets.nssai.singleNssais.push_back(singleNnsaiDetail);

  change.operation = "CREATE";
  change.resourcePath = "/profiles/udmProfiles/udmSliceProfiles/1111";

  plmnIdSets.plmnIdSetIdentifier = "Default";
  dnnData.sscModes.defaultSscMode = "SSC_MODE_1";
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_3");
  dnnData.sscModes.allowedSscModes.push_back("SSC_MODE_2");

  dnnData.pduSessionTypes.defaultSessionType = "IPV4";
  dnnData.pduSessionTypes.allowedSessionTypes.push_back("IPV6");
  dnnData.fivegQosProfile.fiveqi = 10;
  dnnData.fivegQosProfile.priorityLevel = 124;
  dnnData.fivegQosProfile.arp.priorityLevel = 13;
  dnnData.fivegQosProfile.arp.preemptCap = "NOT_PREEMPT";
  dnnData.fivegQosProfile.arp.preemptVuln = "NOT_PREEMPTABLE";
  dnnData.sessionAmbr.downlink = "1 Gbps";
  dnnData.sessionAmbr.uplink = "2 Gbps";
  dnnData.upSecurity.upIntegr = "REQUIRED";
  dnnData.upSecurity.upConfid = "PREFERRED";
  dnnData.dnn = "Default";
  dnnData.defaultDnnIndicator = true;
  dnnData.sameSmfInd = true;

  dnnData.fieldExistence.hasDefaultDnnIndicator = true;
  dnnData.fieldExistence.hasLboRoamingAllowed = false;
  dnnData.fieldExistence.hasIwkEpsInd = false;
  dnnData.fieldExistence.hasPduSessionTypes = true;
  dnnData.fieldExistence.hasAllowedSessionTypes = true;
  dnnData.fieldExistence.hasSscModes = true;
  dnnData.fieldExistence.hasSessionAmbr = true;
  dnnData.fieldExistence.hasFivegQosProfile = true;
  dnnData.fivegQosProfile.fieldExistence.hasPriorityLevel = true;
  dnnData.fieldExistence.hasUpSecurity = true;
  dnnData.fieldExistence.hasSmfList = true;
  dnnData.fieldExistence.hasSameSmfInd = true;

  dnnList.dnnListId = "Default";
  dnnList.dnnDataList.push_back(dnnData);
  plmnIdSets.dnnLists.push_back(dnnList);

  change.udmSliceProfile.plmnIdSets.push_back(plmnIdSets);

  record.changes.push_back(change);

  entities::validation_response_t resp = record.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_EQ(record.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("resource_path"),
            "/profiles/udmProfiles/udmSliceProfiles/1111");
  EXPECT_EQ(record.response.errors[0].errorDetails.at("description"),
            "At least one element in \"smfList\" must exist");
}
