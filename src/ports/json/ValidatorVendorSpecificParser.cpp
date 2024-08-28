#include "ValidatorVendorSpecificParser.hpp"

#include <boost/regex.hpp>

namespace port {
namespace secondary {
namespace json {

static const auto VENDOR_SPECIFIC_REGEXP =
    boost::regex{"^vendorSpecific-(.*?)"};

bool ValidatorVendorSpecificParser::isVendorSpecific(const std::string& vs) {
  boost::cmatch cm;
  return boost::regex_match(vs.c_str(), cm, VENDOR_SPECIFIC_REGEXP);
}

void ValidatorVendorSpecificParser::getVendorSpecific(
    const rapidjson::Value& attrData,
    entities::vendorSpecific_t& vendorSpecific) {
  for (rapidjson::Value::ConstMemberIterator itr = attrData.MemberBegin();
       itr != attrData.MemberEnd(); ++itr) {
    const std::string& attrName = itr->name.GetString();
    const auto& attrValue = itr->value;
    if (isVendorSpecific(attrName)) {
      vendorSpecific.emplace(attrName, attrValue);
    }
  }
}

}  // namespace json
}  // namespace secondary
}  // namespace port
