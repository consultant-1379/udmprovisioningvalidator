ARG version=3.29.0-10
FROM armdocker.rnd.ericsson.se/proj_hss/5g/docker_images/scratch:${version}

ARG             BUILD_TYPE
COPY            ${BUILD_TYPE}/bin/UdmProvisioningValidator.usvc.elf /bin/
COPY            schema/UDMProvisioningValidation.yaml /bin/

LABEL com.ericsson.product-number="CXU 101 0355"
LABEL com.ericsson.product-revision="1.0.0"
LABEL com.ericsson.product-name="eric-udm-udmprovisioningvalidator"
LABEL org.opencontainers.image.title="UDM Provisioning Validator"
LABEL org.opencontainers.image.created=""
LABEL org.opencontainers.image.revision=""
LABEL org.opencontainers.image.vendor="Ericsson"
LABEL org.opencontainers.image.version=""

ENTRYPOINT      ["/bin/UdmProvisioningValidator.usvc.elf"]


