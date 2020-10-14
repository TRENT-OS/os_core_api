#
# OS Core API
#
# Copyright (C) 2019-2020, Hensoldt Cyber GmbH
#

if (SDK_USE_CAMKES)

    CAmkESAddCPPInclude(${OS_LIBS_DIR}/os_core_api/camkes)
    CAmkESAddImportPath(${OS_LIBS_DIR}/os_core_api/camkes)

endif()


#-------------------------------------------------------------------------------
project(os_core_api C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)
