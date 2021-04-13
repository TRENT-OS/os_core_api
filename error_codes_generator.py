import json

os_error_codes = json.loads(open('/host/OS-SDK/pkg/os_core_api/OS_Error.json').read())['OS_Error_t']

with open('/host/OS-SDK/pkg/os_core_api/include/OS_Error.test.h', "w") as os_error_header_file:
    os_error_header_file.write(
"""
/*
 * OS return codes
 *
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   OS error codes
 *
 * @ingroup OS_error
*/
typedef enum
{""")

    with open('/host/OS-SDK/pkg/os_core_api/OS_Error.c', "w") as os_error_source_file:
        os_error_source_file.write(
"""
#include "OS_Error.h"

const char*
OS_Error_toString(
    OS_Error_t err)
{
""")

        for error_domain in os_error_codes:
            os_error_header_file.write(
"""

    //--------------------------------------------------------------------------
""")
            os_error_header_file.write("    // " + error_domain['description'])
            os_error_header_file.write(
"""
    //--------------------------------------------------------------------------
""")
            for index, error_domain_member in enumerate(error_domain['members']):

                error_domain_member_value = int(error_domain['offset']) - index
                os_error_header_file.write(
                    "    "
                    + error_domain_member['name']
                    + " = " + str(error_domain_member_value) + ","
                    + " ///< " + error_domain_member['description']
                    + "\n")

        os_error_source_file.write("}")

    os_error_header_file.write(
"""
}
OS_Error_t;

#ifdef __cplusplus
}
#endif
""")
