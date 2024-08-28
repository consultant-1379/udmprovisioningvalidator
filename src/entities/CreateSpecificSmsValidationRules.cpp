#include <boost/algorithm/string.hpp>

#include "SmsValidationData.hpp"
#include "ports/HTTPcodes.hpp"

namespace entities {

CreateSpecificSmsValidationRules::CreateSpecificSmsValidationRules(
    ValidationData* data, const Change& change) {
  this->data = data;
  this->change = &change;
}

bool CreateSpecificSmsValidationRules::provJournalIsIncluded() {
  std::string mscId = data->getSmsStaticDataMscId(change->resourcePath);
  std::string pathToFind = data->buildProvJournalResourcePathFromMscId(mscId);
  if (not data->relatedResources.contains(pathToFind)) {
    data->addError("Unprocessable entity",
                   {{"resource_path", change->resourcePath},
                    {"description",
                     "provJournal for subscriber mscId=" + mscId +
                         " not included. Needed to check if IMSI is defined: " +
                         pathToFind}});
    return false;
  }
  return true;
}

// R3: Create: IMSI is included in provJournal for the subscriber.
bool CreateSpecificSmsValidationRules::checkImsiIsInProvJournal() {
  std::string mscId = data->getSmsStaticDataMscId(change->resourcePath);
  std::string pathToFind = data->buildProvJournalResourcePathFromMscId(mscId);
  entities::ProvJournal journal = boost::get<entities::prov_journal_t>(
      data->relatedResources.at(pathToFind));

  if (journal.imsi.empty()) {
    data->addError(
        "Constraint Violation",
        {{"resource_path", change->resourcePath},
         {"description",
          "There must be an IMSI defined for the subscriber mscId=" + mscId}});
    return false;
  }
  return true;
}

entities::validation_response_t
CreateSpecificSmsValidationRules::applyValidationRules() {
  if (not this->provJournalIsIncluded()) {
    return {false, ::port::HTTP_UNPROCESSABLE_ENTITY};
  } else {
    if (not this->checkImsiIsInProvJournal())
      return {false, ::port::HTTP_CONFLICT};
  }
  return {true, ::port::HTTP_OK};
}

};  // namespace entities
