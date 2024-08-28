#ifndef __JSON_CONSTANTS_HPP__
#define __JSON_CONSTANTS_HPP__

// request
constexpr auto JSON_CHANGES = "changes";
constexpr auto JSON_RELATED_RESOURCES = "relatedResources";
constexpr auto JSON_OPERATION = "operation";
constexpr auto JSON_RESOURCE_PATH = "resource_path";
constexpr auto JSON_DATA = "data";
constexpr auto JSON_OPERATION_CREATE = "CREATE";
constexpr auto JSON_OPERATION_UPDATE = "UPDATE";
constexpr auto JSON_OPERATION_DELETE = "DELETE";

// response
constexpr auto JSON_ERRORS = "errors";
constexpr auto JSON_ERROR_MESSAGE = "errorMessage";
constexpr auto JSON_ERROR_DETAILS = "errorDetails";

// UdmStaticData
constexpr auto JSON_UDM_STATIC_DATA = "udmStaticData";
constexpr auto JSON_SUBSCRIBED_UE_AMBR = "subscribedUeAmbr";
constexpr auto JSON_RAT_RESTRICTIONS = "ratRestrictions";
constexpr auto JSON_UPLINK = "uplink";
constexpr auto JSON_DOWNLINK = "downlink";
constexpr auto JSON_FORBIDDEN_AREAS = "forbiddenAreas";
constexpr auto JSON_ID = "id";
constexpr auto JSON_SERVICE_AREA_RESTRICTION = "serviceAreaRestriction";
constexpr auto JSON_RESTRICTION_TYPE = "restrictionType";
constexpr auto JSON_AREAS = "areas";
constexpr auto JSON_MAX_NUM_OF_TAS = "maxNumOfTAs";
constexpr auto JSON_CORE_NETWORK_TYPE_RESTRICTIONS =
    "coreNetworkTypeRestrictions";
constexpr auto JSON_MPS_PRIORITY = "mpsPriority";
constexpr auto JSON_MCS_PRIORITY = "mcsPriority";
constexpr auto JSON_DL_PACKET_COUNT = "dlPacketCount";
constexpr auto JSON_MICO_ALLOWED = "micoAllowed";
constexpr auto JSON_UDM_SLICE_PROFILE_ID = "udmSliceProfileId";
constexpr auto JSON_STATIC_IP_ADDRESS = "staticIpAddress";
constexpr auto JSON_NSS_AI_DNN_IP_LISTS = "nssaiDnnIpLists";
constexpr auto JSON_NSSAI = "nssai";
constexpr auto JSON_DNN_IP_LIST = "dnnIpList";
constexpr auto JSON_SST = "sst";
constexpr auto JSON_SD = "sd";
constexpr auto JSON_DNN = "dnn";
constexpr auto JSON_IPADDRESS = "ipAddress";
constexpr auto JSON_IPV4_ADDR = "ipv4Addr";
constexpr auto JSON_IPV6_ADDR = "ipv6Addr";
constexpr auto JSON_IPV6_PREFIX = "ipv6Prefix";
constexpr auto JSON_ROAMING_ODB = "roamingOdb";
constexpr auto JSON_UE_USAGE_TYPE = "ueUsageType";
constexpr auto JSON_RFS_INDEX = "rfspIndex";
constexpr auto JSON_SUBS_REG_TIMER = "subsRegTimer";
constexpr auto JSON_NOT_ALLOWED_AREAS = "NOT_ALLOWED_AREAS";
constexpr auto JSON_ALLOWED_AREAS = "ALLOWED_AREAS";
constexpr auto JSON_NR = "NR";
constexpr auto JSON_EUTRA = "EUTRA";
constexpr auto JSON_WLAN = "WLAN";
constexpr auto JSON_VIRTUAL = "VIRTUAL";
constexpr auto JSON_5GC = "5GC";
constexpr auto JSON_EPC = "EPC";
constexpr auto JSON_OUTSIDE_HOME_PLMN = "OUTSIDE_HOME_PLMN";
constexpr auto JSON_OUTSIDE_HOME_PLMN_COUNTRY = "OUTSIDE_HOME_PLMN_COUNTRY";
constexpr auto JSON_UDM_SLICE_DATA = "udmSliceData";

// ProvJournal
constexpr auto JSON_PROV_JOURNAL = "provJournal";
constexpr auto JSON_NOTIF_REF = "notifRef";
constexpr auto JSON_IMSI = "imsi";
constexpr auto JSON_IMSI_MASK = "imsiMask";
constexpr auto JSON_IMSI_EXT_MASK = "imsiExtMask";
constexpr auto JSON_MSISDN = "msisdn";
constexpr auto JSON_MSISDN_MASK = "msisdnMask";
constexpr auto JSON_MSISDN_EXT_MASK = "msisdnExtMask";
constexpr auto JSON_IMSI_AUX = "imsiAux";
constexpr auto JSON_IMSI_AUX_MASK = "imsiAuxMask";
constexpr auto JSON_IMSI_AUX_EXT_MASK = "imsiAuxExtMask";
constexpr auto JSON_IMPI = "impi";
constexpr auto JSON_IMPI_MASK = "impiMask";
constexpr auto JSON_IMPI_EXT_MASK = "impiExtMask";
constexpr auto JSON_SEC_IMPI = "secImpi";
constexpr auto JSON_IMPI_AUX = "impiAux";
constexpr auto JSON_USERNAME = "username";
constexpr auto JSON_USERNAME_MASK = "usernameMask";
constexpr auto JSON_USERNAME_EXT_MASK = "usernameExtMask";
constexpr auto JSON_IMSICHO_STATUS = "imsiChoStatus";
constexpr auto JSON_IMSI_EXPIRY_DATE = "imsiExpiryDate";
constexpr auto JSON_IMSICHO_EXEC = "imsiChoExec";
constexpr auto JSON_IMPUCHO_IDS = "impuChoIds";
constexpr auto JSON_MSC_ID_AUX = "mscIdAux";
constexpr auto JSON_NOTIF_INFO = "notifInfo";
constexpr auto JSON_UE_FUNCTION_MASK = "ueFunctionMask";
constexpr auto JSON_EXT_ID_LIST = "extIdList";
constexpr auto JSON_NAI = "nai";
constexpr auto JSON_NAI_MASK = "naiMask";
constexpr auto JSON_NAI_EXT_MASK = "naiExtMask";
constexpr auto JSON_SUBS_ID_LIST = "subsIdList";
constexpr auto JSON_SUBS_ID = "id";
constexpr auto JSON_SUBS_PREFIX = "prefix";

// UdmSliceProfile

constexpr auto JSON_UDM_SLICE_PROFILES = "udmSliceProfiles";
constexpr auto JSON_UDM_SLICE_PROFILE = "UdmSliceProfile";
constexpr auto JSON_PLMN_ID_SETS = "plmnIdSets";
constexpr auto JSON_PLMN_ID_SET_IDENTIFIER = "plmnIdSetIdentifier";
constexpr auto JSON_DEFAULT_SINGLE_NSSAIS = "defaultSingleNssais";
constexpr auto JSON_DNN_LIST_ID = "dnnListId";
constexpr auto JSON_SINGLE_NSSAIS = "singleNssais";
constexpr auto JSON_DNN_LISTS = "dnnLists";
constexpr auto JSON_DNN_DATA_LIST = "dnnDataList";
constexpr auto JSON_DEFAULT_DNN_INDICATOR = "defaultDnnIndicator";
constexpr auto JSON_LBO_ROAMING_ALLOWED = "lboRoamingAllowed";
constexpr auto JSON_IWK_EPS_IND = "iwkEpsInd";
constexpr auto JSON_LADN_INDICATOR = "ladnIndicator";
constexpr auto JSON_PDU_SESSION_TYPES = "pduSessionTypes";
constexpr auto JSON_DEFAULT_SESSION_TYPE = "defaultSessionType";
constexpr auto JSON_ALLOWED_SESSION_TYPES = "allowedSessionTypes";
constexpr auto JSON_SSC_MODES = "sscModes";
constexpr auto JSON_DEFAULT_SSC_MODE = "defaultSscMode";
constexpr auto JSON_ALLOWED_SSC_MODES = "allowedSscModes";
constexpr auto JSON_5G_QOS_PROFILE = "5gQosProfile";
constexpr auto JSON_5QI = "5qi";
constexpr auto JSON_ARP = "arp";
constexpr auto JSON_PRIORITY_LEVEL = "priorityLevel";
constexpr auto JSON_PREEMPTCAP = "preemptCap";
constexpr auto JSON_PREEMPTVULN = "preemptVuln";
constexpr auto JSON_SESSION_AMBR = "sessionAmbr";
constexpr auto JSON_3GPP_CHARGING_CHARACTERISTICS =
    "3gppChargingCharacteristics";
constexpr auto JSON_UP_SECURITY = "upSecurity";
constexpr auto JSON_UP_INTEGR = "upIntegr";
constexpr auto JSON_UP_CONFID = "upConfid";
constexpr auto JSON_SSC_MODE_1 = "SSC_MODE_1";
constexpr auto JSON_SSC_MODE_2 = "SSC_MODE_2";
constexpr auto JSON_SSC_MODE_3 = "SSC_MODE_3";
constexpr auto JSON_IPV4 = "IPV4";
constexpr auto JSON_IPV6 = "IPV6";
constexpr auto JSON_IPV4_V6 = "IPV4V6";
constexpr auto JSON_UNSTRUCTURED = "UNSTRUCTURED";
constexpr auto JSON_ETHERNET = "ETHERNET";
constexpr auto JSON_NOT_PREEMPT = "NOT_PREEMPT";
constexpr auto JSON_MAY_PREEMPT = "MAY_PREEMPT";
constexpr auto JSON_NOT_PREEMPTABLE = "NOT_PREEMPTABLE";
constexpr auto JSON_PREEMPTABLE = "PREEMPTABLE";
constexpr auto JSON_REQUIRED = "REQUIRED";
constexpr auto JSON_PREFERRED = "PREFERRED";
constexpr auto JSON_NOT_NEEDED = "NOT_NEEDED";
constexpr auto JSON_SMF_LIST = "smfList";
constexpr auto JSON_SAME_SMF_IND = "sameSmfInd";

// UdmRoamingArea
constexpr auto JSON_UDM_ROAMING_AREAS = "udmRoamingAreas";
constexpr auto JSON_UDM_ROAMING_AREA_ID = "udmRoamingAreaId";
constexpr auto JSON_AREA_LIST = "areaList";
constexpr auto JSON_TACS = "tacs";
constexpr auto JSON_AREA_CODE = "areaCodes";

// SmsSubscription
constexpr auto JSON_SMS_STATIC_DATA = "smsStaticData";
constexpr auto JSON_SMS_SUBSCRIPTION_DATA = "smsSubscriptionData";
constexpr auto JSON_SMS_MANAGEMENT_SUBSCRIPTION_DATA =
    "smsManagementSubscriptionData";
constexpr auto JSON_SMS_SUBSCRIBED = "smsSubscribed";
constexpr auto JSON_MT_SMS_SUBSCRIBED = "mtSmsSubscribed";
constexpr auto JSON_MT_SMS_BARRING_ALL = "mtSmsBarringAll";
constexpr auto JSON_MT_SMS_BARRING_ROAMING = "mtSmsBarringRoaming";
constexpr auto JSON_MO_SMS_SUBSCRIBED = "moSmsSubscribed";
constexpr auto JSON_MO_SMS_BARRING_ALL = "moSmsBarringAll";
constexpr auto JSON_MO_SMS_BARRING_ROAMING = "moSmsBarringRoaming";

#endif  // __JSON_CONSTANTS_HPP__
