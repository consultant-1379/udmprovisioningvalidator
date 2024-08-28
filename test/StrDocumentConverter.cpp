#include "StrDocumentConverter.hpp"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <sstream>

std::string StrDocumentConverter::documentToStr(
    const entities::genericValue_t& val) {
  rapidjson::Document doc;
  doc.CopyFrom(val, doc.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  std::ostringstream os;
  if (!doc.IsNull()) {
    doc.Accept(writer);
    os << buffer.GetString();
  } else {
    os << "";
  }
  return os.str();
}

rapidjson::Document StrDocumentConverter::strToDocument(
    const std::string& val) {
  rapidjson::Document vsDoc;
  vsDoc.Parse(val.c_str());
  return vsDoc;
}
