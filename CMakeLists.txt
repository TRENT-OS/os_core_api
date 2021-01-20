#
# OS Core API
#
# Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
#

if (SDK_USE_CAMKES)

    CAmkESAddCPPInclude("camkes")
    CAmkESAddImportPath("camkes")

endif()


#-------------------------------------------------------------------------------
project(os_core_api C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)
