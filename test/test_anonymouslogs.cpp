#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "ports/logs/logwrapper.hpp"

TEST(ImsiAnonPositiveTest, StringAnonimizationTests) {
  /* IMSI - International Moobile Subscriber Identity */
  std::string imsi_min = "imsi-123456789012345";
  std::string imsi_may = "IMSI-123456789012345";

  std::string anon_imsi_min = ::anonlog::anonymizeString(imsi_min);
  std::string anon_imsi_may = ::anonlog::anonymizeString(imsi_may);

  ASSERT_STREQ("imsi-*2*4*6*8*0*2*4*", anon_imsi_min.c_str());
  ASSERT_STREQ("IMSI-*2*4*6*8*0*2*4*", anon_imsi_may.c_str());
}

TEST(MsisdnAnonPositiveTest, StringAnonimizationTests) {
  /* MSISDN - Global System for Mobile Communications */
  std::string msidn_min = "msisdn-918369110173";
  std::string msidn_may = "MSISDN-918369110173";

  std::string anon_msidn_min = ::anonlog::anonymizeString(msidn_min);
  std::string anon_msidn_may = ::anonlog::anonymizeString(msidn_may);

  ASSERT_STREQ("msisdn-*1*3*9*1*1*3", anon_msidn_min.c_str());
  ASSERT_STREQ("MSISDN-*1*3*9*1*1*3", anon_msidn_may.c_str());
}

TEST(PeiAnonPositiveTest, StringAnonimizationTests) {
  /* PEI - Permanent Equipment Identifier */
  std::string imei_min = "imei-123456789012345";
  std::string imei_may = "IMEI-123456789012345";

  std::string anon_imei_min = ::anonlog::anonymizeString(imei_min);
  std::string anon_imei_may = ::anonlog::anonymizeString(imei_may);

  ASSERT_STREQ("imei-*2*4*6*8*0*2*4*", anon_imei_min.c_str());
  ASSERT_STREQ("IMEI-*2*4*6*8*0*2*4*", anon_imei_may.c_str());

  std::string imeisv_min = "imeisv-1234567890123456";
  std::string imeisv_may = "IMEISV-1234567890123456";

  std::string anon_imeisv_min = ::anonlog::anonymizeString(imeisv_min);
  std::string anon_imeisv_may = ::anonlog::anonymizeString(imeisv_may);

  ASSERT_STREQ("imeisv-*2*4*6*8*0*2*4*6", anon_imeisv_min.c_str());
  ASSERT_STREQ("IMEISV-*2*4*6*8*0*2*4*6", anon_imeisv_may.c_str());
}

TEST(GpsiAnonPositiveTest, StringAnonimizationTests) {
  /* GPSI - Generic Public Subscription Identifier */
  std::string gpsi_min = "msisdn-12345678901234";
  std::string gpsi_may = "MSISDN-12345678901234";

  std::string anon_gpsi_min = ::anonlog::anonymizeString(gpsi_min);
  std::string anon_gpsi_may = ::anonlog::anonymizeString(gpsi_may);

  ASSERT_STREQ("msisdn-*2*4*6*8*0*2*4", anon_gpsi_min.c_str());
  ASSERT_STREQ("MSISDN-*2*4*6*8*0*2*4", anon_gpsi_may.c_str());
}

TEST(ImsiInJsonAnonPositiveTest, StringAnonimizationTests) {
  /* IMSI - International Moobile Subscriber Identity */
  std::string imsi_min = "\"imsi\" : \"123456789012345\"";
  std::string imsi_may = "\"IMSI\" : \"123456789012345\"";

  std::string anon_imsi_min = ::anonlog::anonymizeJson(imsi_min);
  std::string anon_imsi_may = ::anonlog::anonymizeJson(imsi_may);

  ASSERT_STREQ("\"imsi\":\"*2*4*6*8*0*2*4*\"", anon_imsi_min.c_str());
  ASSERT_STREQ("\"IMSI\":\"*2*4*6*8*0*2*4*\"", anon_imsi_may.c_str());
}

TEST(SuciInJsonAnonimization, JsonAnonimizationTests) {
  std::string suci = "\"SUCI\":\"0-123-12\"";

  std::string anon_suci = ::anonlog::anonymizeJson(suci);

  ASSERT_STREQ("\"SUCI\":\"*-*2*-*2\"", anon_suci.c_str());
}

TEST(MsisdnInJsonAnonimization, JsonAnonimizationTests) {
  std::string msisdn = "\"msisdn\":\"123456789012345\"";

  std::string anon_msisdn = ::anonlog::anonymizeJson(msisdn);

  ASSERT_STREQ("\"msisdn\":\"*2*4*6*8*0*2*4*\"", anon_msisdn.c_str());
}

TEST(ImeiInJsonAnonimization, JsonAnonimizationTests) {
  std::string imei = "\"imei\":\"123456789012345\"";

  std::string anon_imei = ::anonlog::anonymizeJson(imei);

  ASSERT_STREQ("\"imei\":\"*2*4*6*8*0*2*4*\"", anon_imei.c_str());
}

TEST(ImeisvInJsonAnonimization, JsonAnonimizationTests) {
  std::string imeisv = "\"imeisv\":\"1234567890123456\"";

  std::string anon_imeisv = ::anonlog::anonymizeJson(imeisv);

  ASSERT_STREQ("\"imeisv\":\"*2*4*6*8*0*2*4*6\"", anon_imeisv.c_str());
}