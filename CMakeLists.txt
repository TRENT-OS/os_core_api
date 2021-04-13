#
# OS Core API
#
# Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
# SPDX-License-Identifier: BSD-3-Clause
#

if (SDK_USE_CAMKES)

    CAmkESAddCPPInclude("camkes")
    CAmkESAddImportPath("camkes")

endif()


#-------------------------------------------------------------------------------
set(CMAKE_C_STANDARD 11)

project(os_core_api C)

add_library(${PROJECT_NAME}
    "OS_Error.c"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)

add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/error_codes_generator.py
)
