#include "ValidatorVendorSpecificEncoder.hpp"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace port {
namespace secondary {
namespace json {

rapidjson::Document ValidatorVendorSpecificEncoder::getVendorSpecific(
    const rapidjson::Value& val,
    rapidjson::Document::AllocatorType& allocator) {
  rapidjson::Document doc;
  doc.CopyFrom(val, allocator);
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  if (!doc.IsNull()) {
    doc.Accept(writer);
  }
  return doc;
}

void ValidatorVendorSpecificEncoder::vendorSpecificToJson(
    rapidjson::Value& data, const entities::vendorSpecific_t& vendorSpecific,
    rapidjson::Document::AllocatorType& allocator) {
  for (auto const& [key, val] : vendorSpecific) {
    data.AddMember(rapidjson::StringRef(key.c_str()),
                   getVendorSpecific(val, allocator), allocator);
  }
}

}  // namespace json
}  // namespace secondary
}  // namespace port
