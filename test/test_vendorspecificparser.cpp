#include "StrDocumentConverter.hpp"
#include "entities/types.hpp"
#include "gtest/gtest.h"
#include "ports/json/ValidatorVendorSpecificParser.hpp"

TEST(ValidatorVendorSpecificParserTest, ParseVendorSpecific) {
  std::string jsonString(
      "{\"vendorSpecific-00193\":{\"key1\":[\"hello\",\"world\"],"
      "\"key2\":true,\"key3\":100},\"vendorSpecific-123456\":20,\"hello\":21}");

  auto data = StrDocumentConverter::strToDocument(jsonString);
  entities::vendorSpecific_t vendorSpecific;

  ::port::secondary::json::ValidatorVendorSpecificParser::getVendorSpecific(
      data, vendorSpecific);

  EXPECT_EQ(vendorSpecific.size(), 2);
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                vendorSpecific.at("vendorSpecific-00193")),
            "{\"key1\":[\"hello\",\"world\"],\"key2\":true,\"key3\":100}");
  EXPECT_EQ(StrDocumentConverter::documentToStr(
                vendorSpecific.at("vendorSpecific-123456")),
            "20");
}
