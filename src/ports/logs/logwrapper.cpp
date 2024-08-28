#include "logwrapper.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <filesystem>

#include "formatter.hpp"

namespace anonlog {

static boost::regex userRegExp{
    "("
    "((msisdn|MSISDN)-[0-9]{5,15})|"
    "((imei|IMEI)-[0-9]{15})|"
    "((impi|IMPI)-[0-9]{5,15})|"
    "((imeisv|IMEISV)-[0-9]{16}|"
    "(imsi|IMSI)-[0-9]{5,15}|nai-.+)|"
    "(suci-(0-[0-9]{3}-[0-9]{2,3}|[1-7]-.+)-[0-9]{1,4}-(0-0-\\S+))"
    ")"};

static boost::regex userJsonRegExp{
    "("
    "(\"(msisdn|MSISDN)\" *: *\"[0-9]{5,15}\")|"
    "(\"(imei|IMEI)\" *: *\"[0-9]{15}\")|"
    "(\"(imeisv|IMEISV)\" *: *\"[0-9]{16}\")|"
    "(\"(imsi|IMSI)\" *: *\"[0-9]{5,15}\")|"
    "(\"(impi|IMPI)\" *: *\"[0-9]{5,15}\")|"
    "(\"(username|USERNAME)\" *: *\"[a-zA-Z0-9]*\")|"
    "(\"(TrafficId)\" *: *\"[a-zA-Z0-9]*\")|"
    "(\"(trafficIdList)\" *: *\\[(\"[0-9]*\",? ?)*\\])|"
    "(\"(suci|SUCI)\" *: *\"(0-[0-9]{3}-[0-9]{2,3})\")"
    ")"};

std::string anonymizeString(const std::string &str) {
  static anonlog::Formatter fmt;
  return ::boost::regex_replace(str, userRegExp, fmt,
                                ::boost::match_default | ::boost::format_all);
}

std::string anonymizeJson(const std::string &str) {
  static anonlog::JsonFormatter json_fmt;
  std::string def_filter = anonymizeString(str);
  return ::boost::regex_replace(def_filter, userJsonRegExp, json_fmt,
                                ::boost::match_default | ::boost::format_all);
}

}  // namespace anonlog