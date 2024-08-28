#include "entities/ValidationData.hpp"
#include "gtest/gtest.h"
#include "ports/HTTPcodes.hpp"

using namespace entities;

TEST(SmsSubscriptionRules, NoRelatedResources) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  change.operation = "UPDATE";
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

  EXPECT_FALSE(std::get<VALIDATION>(resp));
  EXPECT_EQ(std::get<CODE>(resp), ::port::HTTP_UNPROCESSABLE_ENTITY);
  EXPECT_EQ(record.response.changes.size(), 0);
  EXPECT_EQ(record.response.errors.size(), 1);
}

TEST(SmsSubscriptionRules, RelatedResourcesIncluded) {
  ValidationData record;
  Change change;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  sms_subscription_data_t smsSubscriptionData;

  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = true;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  record.relatedResources.insert(
      {"/subscribers/123abc/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
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

// R8: Update: smsSubscribed cannot be removed if either
// mtSmsSubscribed or moSmsSubscribed is true
TEST(
    SmsSubscriptionRules,
    IfCreateOrUpdateSmsSubscribedCannotBeRemovedIfEitherMtSmsSubscribedOrMoSmsSubscribedIsTrueError) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;

  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = false;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      false;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(data.response.changes.size(), 0);
  EXPECT_EQ(data.response.errors.size(), 1);

  EXPECT_EQ(data.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(data.response.errors[0].errorDetails.at("description"),
            "smsSubscribed must be true if either mtSmsSubscribed or "
            "moSmsSubscribed is true");

  EXPECT_EQ(data.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
            "smsStaticData");
}

// R9: R9: Update: mtSmsSubscribed cannot be removed if either mtSmsBarringAll
// or mtSmsBarringRoaming is true
TEST(
    SmsSubscriptionRules,
    IfUpdateMtSmsSubscribedCannotBeRemovedIfEitherMtSmsBarringAllOrMtSmsBarringRoamingIsTrueError) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;

  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = false;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      false;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(data.response.changes.size(), 0);
  EXPECT_EQ(data.response.errors.size(), 1);

  EXPECT_EQ(data.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(data.response.errors[0].errorDetails.at("description"),
            "mtSmsSubscribed must be true if either mtSmsBarringAll or "
            "mtSmsBarringRoaming is true");

  EXPECT_EQ(data.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
            "smsStaticData");
}

// R10: Update: moSmsSubscribed cannot be removed if either  moSmsBarringAll
// or moSmsBarringRoaming is true
TEST(
    SmsSubscriptionRules,
    IfUpdateMoSmsSubscribedCannotBeRemovedIfEitherMoSmsBarringAllOrMoSmsBarringRoamingIsTrueError) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = true;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;

  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      false;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = false;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(data.response.changes.size(), 0);
  EXPECT_EQ(data.response.errors.size(), 1);

  EXPECT_EQ(data.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(data.response.errors[0].errorDetails.at("description"),
            "moSmsSubscribed must be true if either moSmsBarringAll or "
            "moSmsBarringRoaming is true");

  EXPECT_EQ(data.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
            "smsStaticData");
}

// R11: Update: When mtSmsBarringRoaming is added, it cannot take true value
// if mtSmsSubscribed is false
TEST(
    SmsSubscriptionRules,
    IfUpdateWhenMtSmsBarringRoamingIsAddedItCannotTakeTrueValueIfMtSmsSubscribedIsFalseError) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(data.response.changes.size(), 0);
  EXPECT_EQ(data.response.errors.size(), 1);

  EXPECT_EQ(data.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(data.response.errors[0].errorDetails.at("description"),
            "mtSmsSubscribed must be true if either mtSmsBarringAll or "
            "mtSmsBarringRoaming is true");

  EXPECT_EQ(data.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
            "smsStaticData");
}

TEST(
    SmsSubscriptionRules,
    IfUpdateWhenMtSmsBarringRoamingIsAddedItCannotTakeTrueValueIfMtSmsSubscribedIsFalse) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);
  EXPECT_EQ(data.response.changes.size(), 1);
  EXPECT_EQ(data.response.errors.size(), 0);
}

// R12: Update: When mtSmsBarringAll is added, it cannot take true value if
// mtSmsSubscribed is false
TEST(
    SmsSubscriptionRules,
    IfUpdateWhenMtSmsBarringAllIsAddedItCannotTakeTrueValueIfMtSmsSubscribedIsFalseError) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming =
      false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(data.response.changes.size(), 0);
  EXPECT_EQ(data.response.errors.size(), 1);

  EXPECT_EQ(data.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(data.response.errors[0].errorDetails.at("description"),
            "mtSmsSubscribed must be true if either mtSmsBarringAll or "
            "mtSmsBarringRoaming is true");

  EXPECT_EQ(data.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
            "smsStaticData");
}

TEST(
    SmsSubscriptionRules,
    IfUpdateWhenMtSmsBarringAllIsAddedItCannotTakeTrueValueIfMtSmsSubscribedIsFalse) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming =
      false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);
  EXPECT_EQ(data.response.changes.size(), 1);
  EXPECT_EQ(data.response.errors.size(), 0);
}

// R13: Update: When moSmsBarringRoaming is added, it cannot take true value
// if moSmsSubscribed is false
TEST(
    SmsSubscriptionRules,
    IfUpdateWhenMoSmsBarringRoamingIsAddedItCannotTakeTrueValueIfMoSmsSubscribedIsFalseError) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = false;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming =
      false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = true;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(data.response.changes.size(), 0);
  EXPECT_EQ(data.response.errors.size(), 1);

  EXPECT_EQ(data.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(data.response.errors[0].errorDetails.at("description"),
            "moSmsSubscribed must be true if either moSmsBarringAll or "
            "moSmsBarringRoaming is true");

  EXPECT_EQ(data.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
            "smsStaticData");
}

TEST(
    SmsSubscriptionRules,
    IfUpdateWhenMoSmsBarringRoamingIsAddedItCannotTakeTrueValueIfMoSmsSubscribedIsFalse) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = false;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming =
      false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = true;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);
  EXPECT_EQ(data.response.changes.size(), 1);
  EXPECT_EQ(data.response.errors.size(), 0);
}

// R14: Update: When moSmsBarringAll is added, it cannot take true value
// if moSmsSubscribed is false
TEST(
    SmsSubscriptionRules,
    IfUpdateWhenMoSmsBarringAllIsAddedItCannotTakeTrueValueIfMoSmsSubscribedIsFalseError) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming =
      false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), false);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_CONFLICT);

  EXPECT_EQ(data.response.changes.size(), 0);
  EXPECT_EQ(data.response.errors.size(), 1);

  EXPECT_EQ(data.response.errors[0].errorMessage, "Constraint Violation");
  EXPECT_EQ(data.response.errors[0].errorDetails.at("description"),
            "moSmsSubscribed must be true if either moSmsBarringAll or "
            "moSmsBarringRoaming is true");

  EXPECT_EQ(data.response.errors[0].errorDetails.at("resource_path"),
            "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
            "smsStaticData");
}

TEST(
    SmsSubscriptionRules,
    IfUpdateWhenMoSmsBarringAllIsAddedItCannotTakeTrueValueIfMoSmsSubscribedIsFalse) {
  entities::Change change;
  entities::ValidationData data;
  entities::sms_static_data_t smsStaticData;

  smsStaticData.smsSubscriptionData.smsSubscribed = true;
  smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming = false;
  smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = false;
  smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming = false;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData = true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsManagementSubscriptionData =
      true;
  smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = false;
  smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming = true;

  data.relatedResources.insert(
      {"/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
       "smsStaticData",
       smsStaticData});

  change.operation = "UPDATE";
  change.resourcePath =
      "/subscribers/111aaa/smsSubscription/smsStaticAndRegistrationData/"
      "smsStaticData";
  change.smsStaticData.smsSubscriptionData.smsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll = false;
  change.smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming =
      false;
  change.smsStaticData.smsManagementSubscriptionData.moSmsSubscribed = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringAll = true;
  change.smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming =
      false;

  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData
      .hasSmsManagementSubscriptionData = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming =
      true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll = true;
  change.smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming =
      true;

  data.changes.push_back(change);

  entities::validation_response_t resp = data.applyValidationRules();

  EXPECT_EQ(std::get<entities::VALIDATION>(resp), true);
  EXPECT_EQ(std::get<entities::CODE>(resp), ::port::HTTP_OK);
  EXPECT_EQ(data.response.changes.size(), 1);
  EXPECT_EQ(data.response.errors.size(), 0);
}
