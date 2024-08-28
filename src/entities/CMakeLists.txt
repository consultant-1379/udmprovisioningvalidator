cmake_minimum_required(VERSION 3.0.1)

hss_add_lib(
        entities
        SRC
                ValidationData.cpp
                SmsValidationData.cpp
                CommonSmsValidationRules.cpp
                UpdateSpecificSmsValidationRules.cpp
                CreateSpecificSmsValidationRules.cpp
                Context.cpp
        INCLUDE
                ${BASE_INCLUDES}
                ${LOG_INCLUDES}
        STATIC
)
