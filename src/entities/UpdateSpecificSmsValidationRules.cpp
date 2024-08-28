#include "SmsValidationData.hpp"
#include "ports/HTTPcodes.hpp"

namespace entities {

UpdateSpecificSmsValidationRules::UpdateSpecificSmsValidationRules(
    ValidationData* data, const Change& change) {
  this->data = data;
  this->change = &change;
}

bool UpdateSpecificSmsValidationRules::isRelatedResourcesStaticDataProvided() {
  ::entities::resource_path_t path = change->resourcePath;
  if (not data->hasRelatedResource(path)) {
    data->addError(
        "Unprocessable entity",
        {{"resource_path", path},
         {"description", "There is no associated relatedResource: " + path}});
    return false;
  }
  return true;
}

entities::validation_response_t
UpdateSpecificSmsValidationRules::applyValidationRules() {
  if (not this->isRelatedResourcesStaticDataProvided()) {
    return {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
  }

  return {true, ::port::HTTP_OK};
}
};  // namespace entities