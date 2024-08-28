cmake_minimum_required(VERSION 3.0.1)

hss_add_lib(
        jsonport
        SRC
                ValidatorRapidJsonParser.cpp
                ValidatorRapidJsonEncoder.cpp
                ValidatorSmsSubsParser.cpp
                ValidatorVendorSpecificParser.cpp
                ValidatorVendorSpecificEncoder.cpp
        INCLUDE
                ${BASE_INCLUDES}
        STATIC
)
