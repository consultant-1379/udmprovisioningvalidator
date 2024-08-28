#include "entities/ValidationData.hpp"
#include "gtest/gtest.h"
#include "ports/HTTPcodes.hpp"

using namespace ::entities;

TEST(CreateAndCommonSmsSubscriptionRules, ValidCreateSmsSubscription) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";

  // R3: Create: IMSI is included in provJournal for the subscriber.
  ::entities::ProvJournal provJournal;
  provJournal.imsi = "imsi123abc";
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});

  fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData = true;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed
  // or moSmsSubscribed is true
  fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;
  smsSubscriptionData.smsSubscribed = true;

  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;

  smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;

  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.moSmsBarringAll = true;
  smsManagementSubscriptionData.moSmsBarringRoaming = true;

  change.smsStaticData.fieldExistenceSmsStaticData =
      fieldExistenceSmsStaticData;
  change.smsStaticData.smsManagementSubscriptionData =
      smsManagementSubscriptionData;
  change.smsStaticData.smsSubscriptionData = smsSubscriptionData;

  record.changes.push_back(change);

  validation_response_t resp = record.applyValidationRules();

  EXPECT_TRUE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_OK);
  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(CreateAndCommonSmsSubscriptionRules, ValidCreateSmsSubscription2) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";

  // R3: Create: IMSI is included in provJournal for the subscriber.
  ::entities::ProvJournal provJournal;
  provJournal.imsi = "imsi123abc";
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});

  fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData = true;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed
  // or moSmsSubscribed is true
  fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = false;
  smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsSubscriptionData.smsSubscribed = false;

  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;

  smsManagementSubscriptionData.mtSmsBarringRoaming = false;
  smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsManagementSubscriptionData.mtSmsSubscribed = false;

  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = false;
  smsManagementSubscriptionData.moSmsBarringAll = false;
  smsManagementSubscriptionData.moSmsBarringRoaming = false;

  change.smsStaticData.fieldExistenceSmsStaticData =
      fieldExistenceSmsStaticData;
  change.smsStaticData.smsManagementSubscriptionData =
      smsManagementSubscriptionData;
  change.smsStaticData.smsSubscriptionData = smsSubscriptionData;

  record.changes.push_back(change);

  validation_response_t resp = record.applyValidationRules();

  EXPECT_TRUE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_OK);
  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(CreateAndCommonSmsSubscriptionRules, ValidEmptyCreateSmsSubscription) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";

  // R3: Create: IMSI is included in provJournal for the subscriber.
  ::entities::ProvJournal provJournal;
  provJournal.imsi = "imsi123";
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});

  fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData = true;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed
  // or moSmsSubscribed is true
  fieldExistenceSmsStaticData.hasSmsSubscribed = false;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = false;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = false;

  smsManagementSubscriptionData.moSmsSubscribed = false;
  smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsSubscriptionData.smsSubscribed = false;

  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMtSmsBarringAll = false;
  fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = false;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = false;

  smsManagementSubscriptionData.mtSmsBarringRoaming = false;
  smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsManagementSubscriptionData.mtSmsSubscribed = false;

  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = false;
  fieldExistenceSmsStaticData.hasMoSmsBarringAll = false;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = false;

  smsManagementSubscriptionData.moSmsSubscribed = false;
  smsManagementSubscriptionData.moSmsBarringAll = false;
  smsManagementSubscriptionData.moSmsBarringRoaming = false;

  change.smsStaticData.fieldExistenceSmsStaticData =
      fieldExistenceSmsStaticData;
  change.smsStaticData.smsManagementSubscriptionData =
      smsManagementSubscriptionData;
  change.smsStaticData.smsSubscriptionData = smsSubscriptionData;

  record.changes.push_back(change);

  validation_response_t resp = record.applyValidationRules();

  EXPECT_TRUE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_OK);
  EXPECT_EQ(record.response.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 0);
}

TEST(CreateAndCommonSmsSubscriptionRules, CreateSmsSubscriptionNoProvJournal) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";

  // R3: Create: IMSI is included in provJournal for the subscriber.
  //::entities::ProvJournal provJournal;
  // provJournal.imsi = "";
  // record.relatedResources.insert(
  //    {"/subscribers/123abc/journal/provJournal", provJournal});

  fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData = true;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed
  // or moSmsSubscribed is true
  fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;
  smsSubscriptionData.smsSubscribed = true;

  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;

  smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;

  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.moSmsBarringAll = true;
  smsManagementSubscriptionData.moSmsBarringRoaming = true;

  change.smsStaticData.fieldExistenceSmsStaticData =
      fieldExistenceSmsStaticData;
  change.smsStaticData.smsManagementSubscriptionData =
      smsManagementSubscriptionData;
  change.smsStaticData.smsSubscriptionData = smsSubscriptionData;

  record.changes.push_back(change);

  validation_response_t resp = record.applyValidationRules();

  EXPECT_FALSE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_UNPROCESSABLE_ENTITY);
  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_STREQ(
      record.response.errors[0].errorDetails["description"].c_str(),
      "provJournal for subscriber mscId=123abc not included. Needed to check "
      "if IMSI is defined: /subscribers/123abc/journal/provJournal");
}

TEST(CreateAndCommonSmsSubscriptionRules,
     CreateSmsSubscriptionNoImsiInProvJournal) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";

  // R3: Create: IMSI is included in provJournal for the subscriber.
  ::entities::ProvJournal provJournal;
  provJournal.imsi = "";  // No imsi included
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});

  fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData = true;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed
  // or moSmsSubscribed is true
  fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;
  smsSubscriptionData.smsSubscribed = true;

  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;

  smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;

  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.moSmsBarringAll = true;
  smsManagementSubscriptionData.moSmsBarringRoaming = true;

  change.smsStaticData.fieldExistenceSmsStaticData =
      fieldExistenceSmsStaticData;
  change.smsStaticData.smsManagementSubscriptionData =
      smsManagementSubscriptionData;
  change.smsStaticData.smsSubscriptionData = smsSubscriptionData;

  record.changes.push_back(change);

  validation_response_t resp = record.applyValidationRules();

  EXPECT_FALSE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_CONFLICT);
  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_STREQ(record.response.errors[0].errorDetails["description"].c_str(),
               "There must be an IMSI defined for the subscriber mscId=123abc");
}

// R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed or
// moSmsSubscribed is true
TEST(
    CreateAndCommonSmsSubscriptionRules,
    IfCreateOrUpdateSmsSubscribedMustBeTrueIfEitherMtSmsSubscribedOrMoSmsSubscribedIsTrue) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";

  // R3: Create: IMSI is included in provJournal for the subscriber.
  ::entities::ProvJournal provJournal;
  provJournal.imsi = "imsi123";
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});

  fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData = true;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed
  // or moSmsSubscribed is true
  fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;
  smsSubscriptionData.smsSubscribed = false;

  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;

  smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;

  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.moSmsBarringAll = true;
  smsManagementSubscriptionData.moSmsBarringRoaming = true;

  change.smsStaticData.fieldExistenceSmsStaticData =
      fieldExistenceSmsStaticData;
  change.smsStaticData.smsManagementSubscriptionData =
      smsManagementSubscriptionData;
  change.smsStaticData.smsSubscriptionData = smsSubscriptionData;

  record.changes.push_back(change);

  validation_response_t resp = record.applyValidationRules();

  EXPECT_FALSE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_CONFLICT);
  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_STREQ(record.response.errors[0].errorDetails["description"].c_str(),
               "smsSubscribed must be true if either mtSmsSubscribed or "
               "moSmsSubscribed is true");
}

// R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
// or mtSmsBarringRoaming is true
TEST(
    CreateAndCommonSmsSubscriptionRules,
    IfCreateOrUpdateMtSmsSubscribedMustBeTrueIfEitherMtSmsBarringAllOrMtSmsBarringRoamingIsTrue) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";

  // R3: Create: IMSI is included in provJournal for the subscriber.
  ::entities::ProvJournal provJournal;
  provJournal.imsi = "imsi123";
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});

  fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData = true;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed
  // or moSmsSubscribed is true
  fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsSubscriptionData.smsSubscribed = true;

  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;

  smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsManagementSubscriptionData.mtSmsSubscribed = false;

  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.moSmsBarringAll = true;
  smsManagementSubscriptionData.moSmsBarringRoaming = true;

  change.smsStaticData.fieldExistenceSmsStaticData =
      fieldExistenceSmsStaticData;
  change.smsStaticData.smsManagementSubscriptionData =
      smsManagementSubscriptionData;
  change.smsStaticData.smsSubscriptionData = smsSubscriptionData;

  record.changes.push_back(change);

  validation_response_t resp = record.applyValidationRules();

  EXPECT_FALSE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_CONFLICT);
  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_STREQ(record.response.errors[0].errorDetails["description"].c_str(),
               "mtSmsSubscribed must be true if either mtSmsBarringAll or "
               "mtSmsBarringRoaming is true");
}

// R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
// or moSmsBarringRoaming is true
TEST(
    CreateAndCommonSmsSubscriptionRules,
    IfCreateOrUpdateMoSmsSubscribedMustBeTrueIfEitherMoSmsBarringAllOrMoSmsBarringRoamingIsTrue) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "CREATE";
  change.resourcePath =
      "/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";

  // R3: Create: IMSI is included in provJournal for the subscriber.
  ::entities::ProvJournal provJournal;
  provJournal.imsi = "imsi123";
  record.relatedResources.insert(
      {"/subscribers/123abc/journal/provJournal", provJournal});

  fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData = true;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed
  // or moSmsSubscribed is true
  fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = true;
  smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsSubscriptionData.smsSubscribed = true;

  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;

  smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsManagementSubscriptionData.mtSmsSubscribed = true;

  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;
  fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;

  smsManagementSubscriptionData.moSmsSubscribed = false;
  smsManagementSubscriptionData.moSmsBarringAll = true;
  smsManagementSubscriptionData.moSmsBarringRoaming = true;

  change.smsStaticData.fieldExistenceSmsStaticData =
      fieldExistenceSmsStaticData;
  change.smsStaticData.smsManagementSubscriptionData =
      smsManagementSubscriptionData;
  change.smsStaticData.smsSubscriptionData = smsSubscriptionData;

  record.changes.push_back(change);

  validation_response_t resp = record.applyValidationRules();

  EXPECT_FALSE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_CONFLICT);
  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
  EXPECT_STREQ(record.response.errors[0].errorDetails["description"].c_str(),
               "moSmsSubscribed must be true if either moSmsBarringAll or "
               "moSmsBarringRoaming is true");
}
