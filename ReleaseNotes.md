# eric-udm-udmprovisioningvalidator 1.59.0 ReleaseNotes

[TOC]

## Functional content

- UDM Subscription Validation (UDM Static info and mandatory identities) for LA UDM NF of 2019 Q4
- https communication is not supported
- V-API version supported is 0.0.2
- P-API version supported is 1.6.0
- UDM Slice Profile and UDM Roaming Area Profile Validation
- Helm chart adapted in order to meet ADP helm design rules:
    - DR-HC-001. Helm chart version pattern
    - DR-HC-050. Restructure images section in values.yaml
    - DR-HC-060. Use updateStrategy parameter
    - DR-HC-062. Annotate every resource
    - DR-HC-101. Parameterize image stuff
    - DR-HC-110. Use pullSecret
    - DR-HC-120. Use nameOverride in resource names
    - DR-HC-140. matchLabels is set (and implications)
    - DR-HC-143. Pod def must specify resource requests and limits
    - DR-HC-144. k8s defacto labels shall be applied in every resource
- Support for delete operation
- Removed oaivalidator and logproxy sidecars
- Parameter available for setting timezone (UTC by default)
- Logging level can be edited via configmap
- Support to IPv6
- Reply with 422 error code when related resources required for validation are missing
- Added mechanism for overload protection
- Fixed pattern for susbcribedUeAmbr uplink and downlink properties to allow decimal values
- Fixed check for defaultDnnIndicator, which has to be true for every dnnList.
- Fixes to improve performance
- Adjust number of replicas to 2, enable Kubernetes taint based evictions option and equal requests and limits resources.
- Removed unusued libraries
- Update NF name in logs to Provisioning Function
- Add nodeSelector
- Add annotations in logging config map
- Add CXC product information
- Fixed DR-HC-120 for logging config-map
- Update memory limit and request to 20Mi for healthproxy
- Changed nodeSelector path to activation
- Fixed issue with timezone selection
- Updated 3pp libraries to latest versions
- Updated healthproxy version
- [UDM5GP-13771](https://eteamproject.internal.ericsson.com/browse/UDM5GP-13771): Allowed range to provision ueUsageType attribute is from 0 to 255
- Updated 3pp libraries to latest versions
- Enabled crash dumps collections by mounting /corefile under an emptyDir volume
- [HY55599](https://mhweb.ericsson.se/TREditWeb/faces/oo/object.xhtml?eriref=HY55599): Provisioning UDM Slice Profile with only 1 NSSAI
- [HY56079](https://mhweb.ericsson.se/TREditWeb/faces/oo/object.xhtml?eriref=HY56079): No validation on nssaiDnnIpLists definition without IP addresses
- Remove odbPacketServices from udmStaticData according to PAPI 1.1.0
- Updated 3pp libraries to latest versions
- Added additional requirements:
    - Operation and resource path must be strings. Otherwise, the service will return HTTP 400.
    - Operation must be either CREATE, UPDATE or DELETE. Otherwise, the service will return HTTP 400.
    - Resource path for CREATE or UPDATE operations is one of the expected ones. Otherwise, the service will return HTTP 422.
         - /subscribers/{mscId}/udmSubscription/udmStaticAndRegistrationData/udmStaticData
         - /profiles/udmProfiles/udmSliceProfiles/{udmSliceProfileId}
         - /profiles/udmProfiles/udmRoamingAreas/{udmRoamingAreaId}
    - For DELETE operations, any resource path is allowed and the service will always return HTTP 200.
- Log anonymization of sensitive information (user identities).
- Fixed some security issues and changed fsGroup to 1000.
- [UDM5GP-20916](https://eteamproject.internal.ericsson.com/browse/UDM5GP-20916) Fixed image pull policy configuration
- Updated maxUnavailable value for rollingUpdate.
- Support for vendor specific attributes.
- [UDM5GP-23939](https://eteamproject.internal.ericsson.com/browse/UDM5GP-23939) Check for duplicates in Nssais and dnnList of UdmSliceProfile is case insensitive
- [UDM5GP-23910](https://eteamproject.internal.ericsson.com/browse/UDM5GP-23910) If dnn contains the wildcard, the defaultDnnIndicator shall not be set to true
- [UDM5GP-23913](https://eteamproject.internal.ericsson.com/browse/UDM5GP-23913) The format of 3gppChargingCharacteristics is four hexadecimal characters
- Support for individual UDM Nssai and DNN subscription
- Updated 3pp libraries and healthproxy sidecar to latest versions
- Support for SMF information when DNN static IP address/prefix is used
- Support for SMS over NAS
- Support for individual 3gppChargingCharacteristics
- Support for SMF information when DNN static IP address/prefix is used
- [UDM5GP-23858](https://eteamproject.internal.ericsson.com/browse/UDM5GP-23858) eric-udm-udmprovisioningvalidator not ready after worker reboot
- [UDM5GP-32994](https://eteamproject.internal.ericsson.com/browse/UDM5GP-32994) allowedSessionTypes is not mandatory
- Nssai under PlmnIdSets is optional
- Updated 3pp libraries and healthproxy sidecar to latest versions

## Deliverables

### Helm Chart

| Product Number | Chart Name | Version | Description |
|--------------|------------|---------|-------------|
| CXD 101 0530 | eric-udm-udmprovisioningvalidator | [1.3.49](https://arm.lmera.ericsson.se/artifactory/proj-5g-udm-dev-helm/eric-udm-udmprovisioningvalidator-1.3.49.tgz) | UDM provisioning validator microservice deployment |

### Docker Images

| Product Number | Image Name | Version | Gerrit commit |
|--------------|------------|---------|-------------|
| CXU 101 0355 | eric-ccdmeda-udmprovisioningvalidator | [1.3.49](https://arm.epk.ericsson.se/artifactory/docker-v2-global-local/proj_hss/5g/docker_images/eric-ccdmeda-udmprovisioningvalidator/1.3.49) | [HSS/CCDM/EDA/udmprovisioningvalidator:c69c280](https://gerrit.ericsson.se/gitweb?p=HSS/CCDM/EDA/udmprovisioningvalidator.git;a=commit;h=c69c280) |
| CXU 101 0241 | eric-udm-healthproxy | [1.12.4](https://arm.epk.ericsson.se/artifactory/docker-v2-global-local/proj_hss/5g/docker_images/eric-udm-healthproxy/1.12.4) | [HSS/CCSM/UDM/healthproxy:a650b7e](https://gerrit.ericsson.se/gitweb?p=HSS/CCSM/UDM/healthproxy.git;a=commit;h=a650b7e) |

<style type="text/css">
/* automatic heading numbering */
  h1 { counter-reset: h2counter; }
  h2 { counter-reset: h3counter; }
  h3 { counter-reset: h4counter; }
  h4 { counter-reset: h5counter; }
  h5 { counter-reset: h6counter; }
  h6 { }
  h2:before {
    counter-increment: h2counter;
    content: counter(h2counter) ".\0000a0\0000a0";
  }
  h3:before {
    counter-increment: h3counter;
    content: counter(h2counter) "."
             counter(h3counter) ".\0000a0\0000a0";
  }
  h4:before {
    counter-increment: h4counter;
    content: counter(h2counter) "."
             counter(h3counter) "."
             counter(h4counter) ".\0000a0\0000a0";
  }
  h5:before {
    counter-increment: h5counter;
    content: counter(h2counter) "."
             counter(h3counter) "."
             counter(h4counter) "."
             counter(h5counter) ".\0000a0\0000a0";
  }
  h6:before {
    counter-increment: h6counter;
    content: counter(h2counter) "."
             counter(h3counter) "."
             counter(h4counter) "."
             counter(h5counter) "."
             counter(h6counter) ".\0000a0\0000a0";
  }
</style>
