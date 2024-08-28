#include "SmsValidationData.hpp"

#include <boost/regex.hpp>
#include <regex>

#include "ports/HTTPcodes.hpp"
#include "ports/json/JsonConstants.hpp"

namespace entities {

entities::validation_response_t getWorstResponseCode(
    std::vector<entities::validation_response_t>& responses) {
  bool successful = true;
  auto code = ::port::HTTP_OK;  // Smallest response code.
  for (auto res : responses) {
    successful &= std::get<VALIDATION>(res);
    code = std::max(code, std::get<CODE>(res));
  }
  return {successful, code};
}

SmsValidationData::SmsValidationData(ValidationData* data,
                                     const Change& change) {
  this->data = data;
  this->change = &change;
}

entities::validation_response_t SmsValidationData::applyValidationRules() {
  std::vector<entities::validation_response_t> responses;
  entities::validation_response_t resp = {true, ::port::HTTP_OK};
  responses.push_back(resp);

  if (change->operation.compare(JSON_OPERATION_UPDATE) == 0) {
    UpdateSpecificSmsValidationRules updateRules(data, *change);
    CommonSmsValidationRules commonRules(data, *change);
    auto common = commonRules.applyValidationRules();
    auto update = updateRules.applyValidationRules();
    responses.push_back(common);
    responses.push_back(update);

  } else if (change->operation.compare(JSON_OPERATION_CREATE) == 0) {
    CommonSmsValidationRules commonRules(data, *change);
    CreateSpecificSmsValidationRules createRules(data, *change);
    auto common = commonRules.applyValidationRules();
    auto create = createRules.applyValidationRules();
    responses.push_back(common);
    responses.push_back(create);
  }

  return getWorstResponseCode(responses);
}
};  // namespace entities