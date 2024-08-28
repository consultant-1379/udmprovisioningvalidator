#ifndef __SMS_VALIDATION_DATA__
#define __SMS_VALIDATION_DATA__

#include "ValidationData.hpp"
#include "entities/types.hpp"

namespace entities {

/**
 * This is to be used for solving conflicting response codes
 * from different rulesets.
 *
 * In general, the highest response code shall be returned.
 */
entities::validation_response_t getWorstResponseCode(
    std::vector<entities::validation_response_t>&);

class SmsValidationData final {
 private:
  ValidationData* data;
  const Change* change;

 public:
  SmsValidationData(ValidationData* data, const Change& change);
  entities::validation_response_t virtual applyValidationRules();
};

class CommonSmsValidationRules final {
 private:
  ValidationData* data;
  const Change* change;

  // R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed or
  // moSmsSubscribed is true
  bool checkSmsSubscribed();
  // R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
  // or mtSmsBarringRoaming is true
  bool checkMtSmsSubscribed();
  // R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll
  // or moSmsBarringRoaming is true
  bool checkMoSmsSubscribed();

 public:
  CommonSmsValidationRules(ValidationData* data, const Change& change);
  entities::validation_response_t applyValidationRules();
};

class UpdateSpecificSmsValidationRules final {
 private:
  ValidationData* data;
  const Change* change;

  bool isRelatedResourcesStaticDataProvided();

 public:
  UpdateSpecificSmsValidationRules(ValidationData* data, const Change& change);
  entities::validation_response_t applyValidationRules();
};

class CreateSpecificSmsValidationRules final {
 private:
  ValidationData* data;
  const Change* change;

  bool provJournalIsIncluded();

  // R3: Create: IMSI is included in provJournal for the subscriber.
  bool checkImsiIsInProvJournal();

 public:
  CreateSpecificSmsValidationRules(ValidationData* data, const Change& change);
  entities::validation_response_t applyValidationRules();
};

};      // namespace entities
#endif  //__SMS_VALIDATION_DATA__