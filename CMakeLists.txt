#
# OS Core API
#
# Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
# 
# SPDX-License-Identifier: GPL-2.0-or-later
#
# For commercial licensing, contact: info.cyber@hensoldt.net
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
