#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <boost/regex.hpp>
#include <iostream>

#include "gtest/gtest.h"
#include "ports/HTTPcodes.hpp"
#include "ports/json/ValidatorRapidJsonParser.hpp"

TEST(SmsSubscriptionParser, ParseValidJsonSmsSubscriptionOk) {
  std::string jsonString(
      "{"
      "\"changes\":["

      "{\"operation\":\"CREATE\","
      "\"resource_path\":\"/subscribers/123abc/smsSubscription/"
      "smsStaticAndRegistrationData/smsStaticData\","

      "\"data\":{"

      "\"smsSubscriptionData\":{"
      "\"smsSubscribed\" : true"
      "},"

      "\"smsManagementSubscriptionData\":{"
      "\"mtSmsSubscribed\":true,"
      "\"mtSmsBarringAll\":true,"
      "\"mtSmsBarringRoaming\":true,"
      "\"moSmsSubscribed\":true,"
      "\"moSmsBarringAll\":true,"
      "\"moSmsBarringRoaming\":true"
      "}"

      "}"

      "}"

      "]"
      "}");
  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  EXPECT_FALSE(parser.error());
  entities::ValidationData record;
  EXPECT_TRUE(parser.getValidationData(record));

  EXPECT_EQ(record.changes.size(), 1);

  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData);
  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll);
  EXPECT_TRUE(record.changes[0]
                  .smsStaticData.fieldExistenceSmsStaticData
                  .hasSmsManagementSubscriptionData);
  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData);
  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming);
  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed);
  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll);
  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming);
  EXPECT_TRUE(record.changes[0]
                  .smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed);
  EXPECT_TRUE(record.changes[0]
                  .smsStaticData.smsManagementSubscriptionData.moSmsBarringAll);
  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming);
  EXPECT_TRUE(record.changes[0]
                  .smsStaticData.smsManagementSubscriptionData.moSmsSubscribed);
  EXPECT_TRUE(record.changes[0]
                  .smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll);
  EXPECT_TRUE(
      record.changes[0]
          .smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming);
}

// R4: Create: All attributes are according to format (boolean) and
// supported value ranges (true/false)
TEST(SmsSubscriptionParser, IfCreateAllAttributesOfSmsSubscriptionAreBooleans) {
  std::string jsonString(
      "{"
      "\"changes\":["

      "{\"operation\":\"CREATE\","
      "\"resource_path\":\"/subscribers/123abc/smsSubscription/"
      "smsStaticAndRegistrationData/smsStaticData\","

      "\"data\":{"

      "\"smsSubscriptionData\":{"
      "\"smsSubscribed\" : \"Absolutley not a boolean\""
      "},"

      "\"smsManagementSubscriptionData\":{"
      "\"mtSmsSubscribed\": { \"inside_object\" : true },"
      "\"mtSmsBarringAll\": 4,"
      "\"mtSmsBarringRoaming\": [\" This is an array.\" ],"
      "\"moSmsSubscribed\": \"I am a string\","
      "\"moSmsBarringAll\": \"true\","
      "\"moSmsBarringRoaming\": 0"
      "}"

      "}"

      "}"

      "]"
      "}");
  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  EXPECT_FALSE(parser.error());
  entities::ValidationData record;
  EXPECT_TRUE(parser.getValidationData(record));

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 7);

  EXPECT_STREQ(record.response.errors[0].errorDetails.at("description").c_str(),
               "Field:[smsSubscribed] is not boolean");

  EXPECT_STREQ(record.response.errors[1].errorDetails.at("description").c_str(),
               "Field:[mtSmsSubscribed] is not boolean");

  EXPECT_STREQ(record.response.errors[2].errorDetails.at("description").c_str(),
               "Field:[mtSmsBarringAll] is not boolean");

  EXPECT_STREQ(record.response.errors[3].errorDetails.at("description").c_str(),
               "Field:[mtSmsBarringRoaming] is not boolean");

  EXPECT_STREQ(record.response.errors[4].errorDetails.at("description").c_str(),
               "Field:[moSmsSubscribed] is not boolean");

  EXPECT_STREQ(record.response.errors[5].errorDetails.at("description").c_str(),
               "Field:[moSmsBarringAll] is not boolean");

  EXPECT_STREQ(record.response.errors[6].errorDetails.at("description").c_str(),
               "Field:[moSmsBarringRoaming] is not boolean");
}

// R4: Update: All attributes are according to format (boolean) and
// supported value ranges (true/false)
TEST(SmsSubscriptionParser, IfUpdateAllAttributesOfSmsSubscriptionAreBooleans) {
  std::string jsonString(
      "{"
      "\"changes\":["

      "{\"operation\":\"UPDATE\","
      "\"resource_path\":\"/subscribers/123abc/smsSubscription/"
      "smsStaticAndRegistrationData/smsStaticData\","

      "\"data\":{"

      "\"smsSubscriptionData\":{"
      "\"smsSubscribed\" : \"Absolutley not a boolean\""
      "},"

      "\"smsManagementSubscriptionData\":{"
      "\"mtSmsSubscribed\": { \"inside_object\" : true },"
      "\"mtSmsBarringAll\": 4,"
      "\"mtSmsBarringRoaming\": [\" This is an array.\" ],"
      "\"moSmsSubscribed\": \"I am a string\","
      "\"moSmsBarringAll\": \"true\","
      "\"moSmsBarringRoaming\": 0"
      "}"

      "}"

      "}"

      "]"
      "}");
  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  EXPECT_FALSE(parser.error());
  entities::ValidationData record;
  EXPECT_TRUE(parser.getValidationData(record));

  EXPECT_EQ(record.changes.size(), 1);
  EXPECT_EQ(record.response.errors.size(), 7);

  EXPECT_STREQ(record.response.errors[0].errorDetails.at("description").c_str(),
               "Field:[smsSubscribed] is not boolean");

  EXPECT_STREQ(record.response.errors[1].errorDetails.at("description").c_str(),
               "Field:[mtSmsSubscribed] is not boolean");

  EXPECT_STREQ(record.response.errors[2].errorDetails.at("description").c_str(),
               "Field:[mtSmsBarringAll] is not boolean");

  EXPECT_STREQ(record.response.errors[3].errorDetails.at("description").c_str(),
               "Field:[mtSmsBarringRoaming] is not boolean");

  EXPECT_STREQ(record.response.errors[4].errorDetails.at("description").c_str(),
               "Field:[moSmsSubscribed] is not boolean");

  EXPECT_STREQ(record.response.errors[5].errorDetails.at("description").c_str(),
               "Field:[moSmsBarringAll] is not boolean");

  EXPECT_STREQ(record.response.errors[6].errorDetails.at("description").c_str(),
               "Field:[moSmsBarringRoaming] is not boolean");
}

TEST(SmsSubscriptionParser, ParseValidJsonSmsSubscriptionRelatedResourcesOk) {
  std::string jsonString(
      "{\"relatedResources\":{\"/subscribers/123abc/smsSubscription/"
      "smsStaticAndRegistrationData/smsStaticData\":{"

      "\"smsSubscriptionData\":{"
      "\"smsSubscribed\" : true"
      "},"

      "\"smsManagementSubscriptionData\":{"
      "\"mtSmsSubscribed\":true,"
      "\"mtSmsBarringAll\":true,"
      "\"mtSmsBarringRoaming\":true,"
      "\"moSmsSubscribed\":true,"
      "\"moSmsBarringAll\":true,"
      "\"moSmsBarringRoaming\":true"
      "}"

      "}}}");
  ::port::secondary::json::ValidatorRapidJsonParser parser(jsonString);

  EXPECT_FALSE(parser.error());
  entities::ValidationData record;
  EXPECT_TRUE(parser.getValidationData(record));

  entities::SmsStaticData& smsStaticData = boost::get<entities::SmsStaticData>(
      record.relatedResources.at("/subscribers/123abc/smsSubscription/"
                                 "smsStaticAndRegistrationData/smsStaticData"));

  EXPECT_EQ(record.relatedResources.size(), 1);

  EXPECT_TRUE(smsStaticData.smsSubscriptionData.smsSubscribed);
  EXPECT_TRUE(smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed);
  EXPECT_TRUE(smsStaticData.fieldExistenceSmsStaticData
                  .hasSmsManagementSubscriptionData);
  EXPECT_TRUE(smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll);
  EXPECT_TRUE(smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringRoaming);
  EXPECT_TRUE(smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed);
  EXPECT_TRUE(smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll);
  EXPECT_TRUE(smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringRoaming);
  EXPECT_TRUE(smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscriptionData);
  EXPECT_TRUE(smsStaticData.smsManagementSubscriptionData.moSmsBarringAll);
  EXPECT_TRUE(smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming);
  EXPECT_TRUE(smsStaticData.smsManagementSubscriptionData.moSmsSubscribed);
  EXPECT_TRUE(smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll);
  EXPECT_TRUE(smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming);
}