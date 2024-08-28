#ifndef __STR_DOCUMENT_CONVERTER_HPP__
#define __STR_DOCUMENT_CONVERTER_HPP__

#include "entities/types.hpp"
#include "rapidjson/document.h"

class StrDocumentConverter final {
 public:
  static std::string documentToStr(const entities::genericValue_t &);
  static rapidjson::Document strToDocument(const std::string &);
};

#endif  //__STR_DOCUMENT_CONVERTER_HPP__
