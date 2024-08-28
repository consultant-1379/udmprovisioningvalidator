#ifndef __UDM_PROVISIONING_VALIDATOR_ENTITIES_TYPES__
#define __UDM_PROVISIONING_VALIDATOR_ENTITIES_TYPES__

#include <map>
#include <string>
#include <vector>

#include "boost/variant.hpp"
#include "rapidjson/document.h"

namespace entities {

struct fieldExistence {
  fieldExistence()
      : fieldExistence{false, false, false, false, false, false,
                       false, false, false, false, false, false} {}
  fieldExistence(const bool& mpsPriority_exists, const bool& mcsPriority_exists,
                 const bool& dlPacketCount_exists,
                 const bool& micoAllowed_exists, const bool& ueUsageType_exists,
                 const bool& rfspIndex_exists, const bool& subsRegTimer_exists,
                 const bool& serviceAreaRestriction_exists,
                 const bool& maxNumOfTas_exists,
                 const bool& staticIpAddress_exists,
                 const bool& subscribedUeAmbr_exists,
                 const bool& udmSliceData_exists)
      : hasMpsPriority{mpsPriority_exists},
        hasMcsPriority{mcsPriority_exists},
        hasDlPacketCount{dlPacketCount_exists},
        hasMicoAllowed{micoAllowed_exists},
        hasUeUsageType{ueUsageType_exists},
        hasRfspIndex{rfspIndex_exists},
        hasSubsRegTimer{subsRegTimer_exists},
        hasServiceAreaRestriction{serviceAreaRestriction_exists},
        hasMaxNumOfTas{maxNumOfTas_exists},
        hasStaticIpAddress{staticIpAddress_exists},
        hasSubscribedUeAmbr{subscribedUeAmbr_exists},
        hasUdmSliceData{udmSliceData_exists} {}

  bool hasMpsPriority;
  bool hasMcsPriority;
  bool hasDlPacketCount;
  bool hasMicoAllowed;
  bool hasUeUsageType;
  bool hasRfspIndex;
  bool hasSubsRegTimer;
  bool hasServiceAreaRestriction;
  bool hasMaxNumOfTas;
  bool hasStaticIpAddress;
  bool hasSubscribedUeAmbr;
  bool hasUdmSliceData;
};

struct fieldExistenceUdmSliceProfile {
  fieldExistenceUdmSliceProfile()
      : fieldExistenceUdmSliceProfile{false, false} {}

  fieldExistenceUdmSliceProfile(const bool& nssai_exists,
                                const bool& any_nssai_exists)
      : hasNssai{nssai_exists}, hasAnyNssai{any_nssai_exists} {}
  bool hasNssai;
  bool hasAnyNssai;
};

struct fieldExistenceDnnData {
  fieldExistenceDnnData()
      : fieldExistenceDnnData{false, false, false, false, false, false,
                              false, false, false, false, false, false} {}

  fieldExistenceDnnData(
      const bool& defaultDnnIndicator_exists,
      const bool& lboRoamingAllowed_exists, const bool& iwkEpsInd_exists,
      const bool& ladnIndicator_exists, const bool& pduSessionTypes_exists,
      const bool& allowedSessionTypes_exists, const bool& sscModes_exists,
      const bool& sessionAmbr_exists, const bool& fivegQosProfile_exists,
      const bool& upSecurity_exists, const bool& smfList_exists,
      const bool& sameSmfInd_exists)
      : hasDefaultDnnIndicator{defaultDnnIndicator_exists},
        hasLboRoamingAllowed{lboRoamingAllowed_exists},
        hasIwkEpsInd{iwkEpsInd_exists},
        hasLadnIndicator{ladnIndicator_exists},
        hasPduSessionTypes{pduSessionTypes_exists},
        hasAllowedSessionTypes{allowedSessionTypes_exists},
        hasSscModes{sscModes_exists},
        hasSessionAmbr{sessionAmbr_exists},
        hasFivegQosProfile{fivegQosProfile_exists},
        hasUpSecurity{upSecurity_exists},
        hasSmfList{smfList_exists},
        hasSameSmfInd{sameSmfInd_exists} {}

  bool hasDefaultDnnIndicator;
  bool hasLboRoamingAllowed;
  bool hasIwkEpsInd;
  bool hasLadnIndicator;
  bool hasPduSessionTypes;
  bool hasAllowedSessionTypes;
  bool hasSscModes;
  bool hasSessionAmbr;
  bool hasFivegQosProfile;
  bool hasUpSecurity;
  bool hasSmfList;
  bool hasSameSmfInd;
};

struct fieldExistenceFivegQosProfile {
  fieldExistenceFivegQosProfile() : fieldExistenceFivegQosProfile{false} {}

  fieldExistenceFivegQosProfile(const bool& priorityLevel_exists)
      : hasPriorityLevel{priorityLevel_exists} {}

  bool hasPriorityLevel;
};

struct fieldExistenceSmsStaticData {
  fieldExistenceSmsStaticData()
      : fieldExistenceSmsStaticData{false, false, false, false, false,
                                    false, false, false, false} {}

  fieldExistenceSmsStaticData(const bool& smsSubscriptionData_exists,
                              const bool& smsManagementSubscriptionData_exists,
                              const bool& smsSubscribed_exists,
                              const bool& mtSmsSubscribed_exists,
                              const bool& mtSmsBarringAll_exists,
                              const bool& mtSmsBarringRoaming_exists,
                              const bool& moSmsSubscribed_exists,
                              const bool& moSmsBarringAll_exists,
                              const bool& moSmsBarringRoaming_exists)
      : hasSmsSubscriptionData{smsSubscriptionData_exists},
        hasSmsManagementSubscriptionData{smsManagementSubscriptionData_exists},
        hasSmsSubscribed{smsSubscribed_exists},
        hasMtSmsSubscribed{mtSmsSubscribed_exists},
        hasMtSmsBarringAll{mtSmsBarringAll_exists},
        hasMtSmsBarringRoaming{mtSmsBarringRoaming_exists},
        hasMoSmsSubscribed{moSmsSubscribed_exists},
        hasMoSmsBarringAll{moSmsBarringAll_exists},
        hasMoSmsBarringRoaming{moSmsBarringRoaming_exists} {}

  bool hasSmsSubscriptionData;
  bool hasSmsManagementSubscriptionData;
  bool hasSmsSubscribed;
  bool hasMtSmsSubscribed;
  bool hasMtSmsBarringAll;
  bool hasMtSmsBarringRoaming;
  bool hasMoSmsSubscribed;
  bool hasMoSmsBarringAll;
  bool hasMoSmsBarringRoaming;
};

using field_existence_t = fieldExistence;
using field_existence_udm_slice_profile_t = fieldExistenceUdmSliceProfile;
using field_existence_dnn_data_t = fieldExistenceDnnData;
using field_existence_fiveg_qos_profile_t = fieldExistenceFivegQosProfile;
using field_existence_sms_static_data_t = fieldExistenceSmsStaticData;

using genericValue_t = rapidjson::GenericValue<rapidjson::UTF8<> >;
using vendorSpecific_t = std::map<std::string, const genericValue_t&>;

using bit_rate_t = std::string;
using restriction_type_t = std::string;
using id_roaming_area_reference_t = std::string;
using max_num_of_tas_t = int;
using mps_priority_t = bool;
using mcs_priority_t = bool;
using dl_packet_count_t = int;
using mico_allowed_t = bool;
using udm_slice_profile_id_t = std::string;
using sst_t = int;
using sd_t = std::string;
using dnn_t = std::string;
using roaming_odb_t = std::string;
using ue_usage_type_t = int;
using rfs_index_t = int;
using subs_reg_timer_t = int;
using ipv4_t = std::string;
using ipv6_t = std::string;
using ipv6_prefix_t = std::string;

enum class IpAddrType { IPv4, IPv6, IPv6Prefix };

struct IpAddr {
  boost::variant<ipv4_t, ipv6_t, ipv6_prefix_t> ip;
  IpAddrType ipType;
  vendorSpecific_t vendorSpecific;
};

using ip_addr_t = std::vector<IpAddr>;

struct Ambr {
  bit_rate_t uplink;
  bit_rate_t downlink;
  vendorSpecific_t vendorSpecific;
};

struct UdmRoamingAreaReference {
  id_roaming_area_reference_t id;
  vendorSpecific_t vendorSpecific;
};

struct UdmSliceProfileId {
  udm_slice_profile_id_t id;
  vendorSpecific_t vendorSpecific;
};

using areas_t = std::vector<UdmRoamingAreaReference>;

struct ServiceAreaRestriction {
  restriction_type_t restrictionType;
  areas_t areas;
  max_num_of_tas_t maxNumOfTas;
  vendorSpecific_t vendorSpecific;
};

struct Snssai {
  sst_t sst;
  sd_t sd;
  vendorSpecific_t vendorSpecific;
};

struct DnnIps {
  dnn_t dnn;
  ip_addr_t ipAddress;
  vendorSpecific_t vendorSpecific;
};

using DnnIpList_t = std::vector<DnnIps>;

struct NssaiDnnIps {
  Snssai snssai;
  DnnIpList_t dnnIpList;
  vendorSpecific_t vendorSpecific;
};

using subscribed_ue_ambr_t = Ambr;
using rat_type_t = std::vector<std::string>;
using forbidden_areas_t = std::vector<UdmRoamingAreaReference>;
using service_area_restriction_t = ServiceAreaRestriction;
using udm_slice_profile_t = UdmSliceProfileId;
using core_network_type_restrictions_t = std::vector<std::string>;
using nssaiDnnIpList_t = std::vector<NssaiDnnIps>;

struct StaticIpAddress {
  nssaiDnnIpList_t nssaiDnnIpLists;
  vendorSpecific_t vendorSpecific;
};

using staticIpAddress_t = StaticIpAddress;

using plmn_id_set_identifier_t = std::string;
using dnn_list_id_t = std::string;
using pair_t = std::tuple<plmn_id_set_identifier_t, dnn_list_id_t>;
using pair_nssai_key_t = std::pair<plmn_id_set_identifier_t, std::string>;
using tuple_nssai_t = std::tuple<sst_t, sd_t, dnn_list_id_t>;

using threegpp_charging_characteristics_t = std::string;

struct NssaiDetails {
  Snssai nssai;
  dnn_list_id_t dnnListId;
  vendorSpecific_t vendorSpecific;
  threegpp_charging_characteristics_t threegppChargingCharacteristics;
};

using single_nssais_t = std::vector<NssaiDetails>;

struct SliceInfoNssai {
  single_nssais_t defaultSingleNssais;
  single_nssais_t singleNssais;
  vendorSpecific_t vendorSpecific;
};

using slice_info_nassai_t = SliceInfoNssai;
using dnn_t = std::string;
using default_dnn_indicator_t = bool;
using lbo_roaming_allowed_t = bool;
using iwk_eps_ind_t = bool;
using ladn_indicator_t = bool;
using pdu_session_type_t = std::string;
using allowed_session_type_t = std::vector<std::string>;
using ssc_mode_t = std::string;
using allowed_ssc_modes_t = std::vector<std::string>;
using same_smf_ind_t = bool;
using smf_list_t = std::vector<std::string>;

struct PduSessionTypes {
  pdu_session_type_t defaultSessionType;
  allowed_session_type_t allowedSessionTypes;
  vendorSpecific_t vendorSpecific;
};

using pdu_session_types_t = PduSessionTypes;
struct SscModes {
  ssc_mode_t defaultSscMode;
  allowed_ssc_modes_t allowedSscModes;
  vendorSpecific_t vendorSpecific;
};

using ssc_modes_t = SscModes;
using fiveqi_t = int;
using priority_level_t = int;
using preemption_capabilitiy_t = std::string;
using preemption_vulnerability_t = std::string;

struct Arp {
  priority_level_t priorityLevel;
  preemption_capabilitiy_t preemptCap;
  preemption_vulnerability_t preemptVuln;
  vendorSpecific_t vendorSpecific;
};
using arp_t = Arp;

struct FivegQosProfile {
  fiveqi_t fiveqi;
  arp_t arp;
  priority_level_t priorityLevel;
  field_existence_fiveg_qos_profile_t fieldExistence;
  vendorSpecific_t vendorSpecific;
};

using fiveg_qos_profile_t = FivegQosProfile;

using up_integrity_t = std::string;
using up_confidentiality_t = std::string;

struct UpSecurity {
  up_integrity_t upIntegr;
  up_confidentiality_t upConfid;
  vendorSpecific_t vendorSpecific;
};

using up_security_t = UpSecurity;

struct DnnData {
  dnn_t dnn;
  default_dnn_indicator_t defaultDnnIndicator;
  lbo_roaming_allowed_t lboRoamingAllowed;
  iwk_eps_ind_t iwkEpsInd;
  ladn_indicator_t ladnIndicator;
  pdu_session_types_t pduSessionTypes;
  ssc_modes_t sscModes;
  fiveg_qos_profile_t fivegQosProfile;
  subscribed_ue_ambr_t sessionAmbr;
  threegpp_charging_characteristics_t threegppChargingCharacteristics;
  up_security_t upSecurity;
  smf_list_t smfList;
  same_smf_ind_t sameSmfInd;
  field_existence_dnn_data_t fieldExistence;
  vendorSpecific_t vendorSpecific;
};

using dnn_data_list_t = std::vector<DnnData>;

struct DnnList {
  dnn_list_id_t dnnListId;
  dnn_data_list_t dnnDataList;
  vendorSpecific_t vendorSpecific;
};

using dnn_list_t = std::vector<DnnList>;

struct PlmnIdSets {
  plmn_id_set_identifier_t plmnIdSetIdentifier;
  slice_info_nassai_t nssai;
  dnn_list_t dnnLists;
  field_existence_udm_slice_profile_t fieldExistenceUdmSliceProfile;
  vendorSpecific_t vendorSpecific;
};

using area_code_t = std::string;
using tacs_t = std::vector<std::string>;
using area_t = boost::variant<area_code_t, tacs_t>;

struct AreaList {
  area_t area;
  vendorSpecific_t vendorSpecific;
};

using area_list_t = AreaList;
using area_list_array_t = std::vector<area_list_t>;

struct UdmRoamingArea {
  id_roaming_area_reference_t udmRoamingAreaId;
  area_list_array_t areaList;
  vendorSpecific_t vendorSpecific;
};

using operation_t = std::string;
using resource_path_t = std::string;
using plmn_id_sets_t = std::vector<PlmnIdSets>;
using udm_roaming_area_t = UdmRoamingArea;

struct UdmSliceProfile {
  udm_slice_profile_id_t udmSliceProfileId;
  plmn_id_sets_t plmnIdSets;
  vendorSpecific_t vendorSpecific;
};

using udm_slice_prof_t = UdmSliceProfile;

struct UdmSliceData {
  plmn_id_sets_t plmnIdSets;
  vendorSpecific_t vendorSpecific;
};

using udm_slice_data_t = UdmSliceData;

struct UdmStaticData {
  subscribed_ue_ambr_t subscribedUeAmbr;
  rat_type_t ratRestrictions;
  forbidden_areas_t forbiddenAreas;
  service_area_restriction_t serviceAreaRestriction;
  core_network_type_restrictions_t coreNetworkTypeRestrictions;
  mps_priority_t mpsPriority;
  mcs_priority_t mcsPriority;
  dl_packet_count_t dlPacketCount;
  mico_allowed_t micoAllowed;
  udm_slice_profile_t udmSliceProfileId;
  staticIpAddress_t staticIpAddress;
  roaming_odb_t roamingOdb;
  ue_usage_type_t ueUsageType;
  rfs_index_t rfspIndex;
  subs_reg_timer_t subsRegTimer;
  udm_slice_data_t udmSliceData;
  field_existence_t fieldExistence;
  vendorSpecific_t vendorSpecific;
};

using mt_sms_subscribed_t = bool;
using mt_sms_barring_all_t = bool;
using mt_sms_barring_roaming_t = bool;
using mo_sms_subscribed_t = bool;
using mo_sms_barring_all_t = bool;
using mo_sms_barring_roaming_t = bool;

struct SmsManagementSubscriptionData {
  mt_sms_subscribed_t mtSmsSubscribed;
  mt_sms_barring_all_t mtSmsBarringAll;
  mt_sms_barring_roaming_t mtSmsBarringRoaming;
  mo_sms_subscribed_t moSmsSubscribed;
  mo_sms_barring_all_t moSmsBarringAll;
  mo_sms_barring_roaming_t moSmsBarringRoaming;
  vendorSpecific_t vendorSpecific;
};

using sms_subscribed_t = bool;

struct SmsSubscriptionData {
  sms_subscribed_t smsSubscribed;
  vendorSpecific_t vendorSpecific;
};

using sms_subscription_data_t = SmsSubscriptionData;

using sms_management_subscription_data_t = SmsManagementSubscriptionData;

struct SmsStaticData {
  sms_subscription_data_t smsSubscriptionData;
  sms_management_subscription_data_t smsManagementSubscriptionData;
  vendorSpecific_t vendorSpecific;
  field_existence_sms_static_data_t fieldExistenceSmsStaticData;
};

using udm_static_data_t = UdmStaticData;
using sms_static_data_t = SmsStaticData;

struct Change {
  operation_t operation;
  resource_path_t resourcePath;
  udm_static_data_t udmStaticData;
  udm_slice_prof_t udmSliceProfile;
  udm_roaming_area_t udmRoamingArea;
  sms_static_data_t smsStaticData;
};

using changes_t = std::vector<Change>;
using path_t = std::string;

using notif_ref_t = std::string;
using imsi_type_t = std::string;
using service_mask_t = std::string;
using ext_service_mask_t = std::string;
using msisdn_type_t = std::string;
using impi_type_t = std::string;
using user_name_t = std::string;
using imsi_cho_status_t = int;
using imsi_expity_date_t = std::string;
using impu_cho_ids_t = std::vector<std::string>;
using msc_id_type_t = std::string;
using notif_info_t = std::string;
using ue_function_mask_t = std::string;
using id_t = std::string;
using identities_id_t = std::vector<id_t>;
using nai_type_t = std::string;
using prefix_t = std::string;

struct SubscriberIdentitiesId {
  id_t id;
  prefix_t prefix;
};

using subscriber_identities_id_t = SubscriberIdentitiesId;
using subs_id_list_t = std::vector<subscriber_identities_id_t>;

struct ProvJournal {
  notif_ref_t notifRef;
  imsi_type_t imsi;
  service_mask_t imsiMask;
  ext_service_mask_t imsiExtMask;
  msisdn_type_t msisdn;
  service_mask_t msisdnMask;
  ext_service_mask_t msisdnExtMask;
  imsi_type_t imsiAux;
  service_mask_t imsiAuxMask;
  ext_service_mask_t imsiAuxExtMask;
  impi_type_t impi;
  service_mask_t impiMask;
  ext_service_mask_t impiExtMask;
  impi_type_t secImpi;
  impi_type_t impiAux;
  user_name_t username;
  service_mask_t usernameMask;
  ext_service_mask_t usernameExtMask;
  imsi_cho_status_t imsiChoStatus;
  imsi_expity_date_t imsiExpiryDate;
  service_mask_t imsiChoExec;
  impu_cho_ids_t impuChoIds;
  msc_id_type_t mscIdAux;
  notif_info_t notifInfo;
  ue_function_mask_t ueFunctionMask;
  identities_id_t extIdList;
  nai_type_t nai;
  service_mask_t naiMask;
  ext_service_mask_t naiExtMask;
  subs_id_list_t subsIdList;
};

using prov_journal_t = ProvJournal;
using resource_t =
    boost::variant<udm_static_data_t, prov_journal_t, udm_slice_prof_t,
                   udm_roaming_area_t, sms_static_data_t>;
using related_resources_t = std::map<path_t, resource_t>;
using error_details_t = std::map<std::string, std::string>;
using error_message_t = std::string;

struct Error {
  error_message_t errorMessage;
  error_details_t errorDetails;
};

using errors_t = std::vector<Error>;

struct Response {
  changes_t changes;
  errors_t errors;
};

using response_t = Response;

using validation_response_t = std::tuple<bool, int>;

using existance_t = bool;

}  // namespace entities

#endif  // __UDM_PROVISIONING_VALIDATOR_ENTITIES_TYPES__
