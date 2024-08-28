#include "SmsValidationData.hpp"
#include "ports/HTTPcodes.hpp"

namespace entities {

CommonSmsValidationRules::CommonSmsValidationRules(ValidationData* data,
                                                   const Change& change) {
  this->data = data;
  this->change = &change;
}

// R5: Create/Update: smsSubscribed must be true if either mtSmsSubscribed or
// moSmsSubscribed is true
bool CommonSmsValidationRules::checkSmsSubscribed() {
  bool applies = false;

  bool smsSubscribed = false;
  bool mtSmsSubscribed = false;
  bool moSmsSubscribed = false;

  if (change->smsStaticData.fieldExistenceSmsStaticData.hasSmsSubscribed) {
    smsSubscribed = change->smsStaticData.smsSubscriptionData.smsSubscribed;
  }
  if (change->smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed) {
    applies = true;
    mtSmsSubscribed =
        change->smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed;
  }

  if (change->smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed) {
    applies = true;
    moSmsSubscribed =
        change->smsStaticData.smsManagementSubscriptionData.moSmsSubscribed;
  }

  if (applies and (mtSmsSubscribed or moSmsSubscribed)) {
    if (not smsSubscribed) {
      data->addError("Constraint Violation",
                     {{"resource_path", change->resourcePath},
                      {"description",
                       "smsSubscribed must be true if either mtSmsSubscribed "
                       "or moSmsSubscribed is true"}});
      return false;
    }
  }

  return true;
}

// R6: Create/Update: mtSmsSubscribed must be true if either mtSmsBarringAll
// or mtSmsBarringRoaming is true
bool CommonSmsValidationRules::checkMtSmsSubscribed() {
  bool applies = false;

  bool mtSmsSubscribed = false;
  bool mtSmsBarringAll = false;
  bool mtSmsBarringRoaming = false;

  if (change->smsStaticData.fieldExistenceSmsStaticData.hasMtSmsSubscribed) {
    mtSmsSubscribed =
        change->smsStaticData.smsManagementSubscriptionData.mtSmsSubscribed;
  }

  if (change->smsStaticData.fieldExistenceSmsStaticData.hasMtSmsBarringAll) {
    applies = true;
    mtSmsBarringAll =
        change->smsStaticData.smsManagementSubscriptionData.mtSmsBarringAll;
  }

  if (change->smsStaticData.fieldExistenceSmsStaticData
          .hasMtSmsBarringRoaming) {
    applies = true;
    mtSmsBarringRoaming =
        change->smsStaticData.smsManagementSubscriptionData.mtSmsBarringRoaming;
  }

  if (applies and (mtSmsBarringAll or mtSmsBarringRoaming)) {
    if (not mtSmsSubscribed) {
      data->addError("Constraint Violation",
                     {{"resource_path", change->resourcePath},
                      {"description",
                       "mtSmsSubscribed must be true if either mtSmsBarringAll "
                       "or mtSmsBarringRoaming is true"}});
      return false;
    }
  }

  return true;
}

// R7: Create/Update: moSmsSubscribed must be true if either moSmsBarringAll or
// moSmsBarringRoaming is true
bool CommonSmsValidationRules::checkMoSmsSubscribed() {
  bool applies = false;

  bool moSmsSubscribed = false;
  bool moSmsBarringAll = false;
  bool moSmsBarringRoaming = false;

  if (change->smsStaticData.fieldExistenceSmsStaticData.hasMoSmsSubscribed) {
    moSmsSubscribed =
        change->smsStaticData.smsManagementSubscriptionData.moSmsSubscribed;
  }
  if (change->smsStaticData.fieldExistenceSmsStaticData.hasMoSmsBarringAll) {
    applies = true;
    moSmsBarringAll =
        change->smsStaticData.smsManagementSubscriptionData.moSmsBarringAll;
  }

  if (change->smsStaticData.fieldExistenceSmsStaticData
          .hasMoSmsBarringRoaming) {
    applies = true;
    moSmsBarringRoaming =
        change->smsStaticData.smsManagementSubscriptionData.moSmsBarringRoaming;
  }

  if (applies and (moSmsBarringAll or moSmsBarringRoaming)) {
    if (not moSmsSubscribed) {
      data->addError("Constraint Violation",
                     {{"resource_path", change->resourcePath},
                      {"description",
                       "moSmsSubscribed must be true if either moSmsBarringAll "
                       "or moSmsBarringRoaming is true"}});
      return false;
    }
  }

  return true;
}

entities::validation_response_t
CommonSmsValidationRules::applyValidationRules() {
  bool successful = true;
  successful &= this->checkSmsSubscribed();
  successful &= this->checkMtSmsSubscribed();
  successful &= this->checkMoSmsSubscribed();
  if (not successful) return {false, ::port::HTTP_CONFLICT};
  return {true, ::port::HTTP_OK};
}

};  // namespace entities