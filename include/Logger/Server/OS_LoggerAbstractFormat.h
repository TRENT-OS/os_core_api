/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @file
 * @brief       Abstract log format interface
 *
 * @ingroup     OS_LoggerAbstractFormat
 */
#pragma once

/**
 * @defgroup    OS_LoggerAbstractFormat OS Logger Abstract Format API
 *
 * @brief       OS Logger API library abstract log format definition
 *
 * @details     This level provides an abstract interface for converting and
 *              printing log format.
 *
 *              This interface is abstract, so an implementation is necessary
 *              in a derived layer.
 *              \n
 *              Print function is a helper function to print the temporary log
 *              format.
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_Error.h"
#include "Logger/Common/OS_LoggerEntry.h"
#include <stdbool.h>
#include <string.h>


/**
 * @details OS_LoggerAbstractFormat_Handle_t defines the class datatype.
*/
typedef struct OS_LoggerAbstractFormat_Handle OS_LoggerAbstractFormat_Handle_t;

/**
 * @brief   Defines the interface for functions pointer to the convert function.
 *
 *          The data of log level id, log level string and the log message are
 *          converted into a defined format.
 *
 * @return  An error code.
*/
typedef OS_Error_t
(*OS_LoggerAbstractFormat_convert_t)(
    OS_LoggerAbstractFormat_Handle_t* self, /*!< [out] Result of the conversion
                                                       will be stored in the
                                                       underlying buffer */
    OS_LoggerEntry_t const* const     entry /*!< [in]  Log entry to be converted
                                                       to the given format. */
);

/**
 * @details OS_LoggerAbstractFormat_print defines the interface for functions
 *          pointer to the print function.
 *
 *          Serves as helper function to print the temporary log format.
 *
 * @param   self:   pointer to the class
*/
typedef void
(*OS_LoggerAbstractFormat_print_t)(OS_LoggerAbstractFormat_Handle_t* self);


/**
 * @details OS_LoggerAbstractFormat_vtable_t contain the member functions to his
 *          class.
*/
typedef struct
{
    OS_LoggerAbstractFormat_convert_t convert;
    OS_LoggerAbstractFormat_print_t   print;
} OS_LoggerAbstractFormat_vtable_t;


/**
 * @details OS_LoggerAbstractFormat_Handle_t contain the vtable to his class.
*/
struct OS_LoggerAbstractFormat_Handle
{
    const OS_LoggerAbstractFormat_vtable_t* vtable; /**< vtable */
};

/**
 * @brief   Calls `convert` function implementation.
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - log_info is a NULL pointer.
 * @retval  OS_SUCCESS                 - Operation was successful.
 * @retval  other                        - Implementation specific.
*/
OS_Error_t
FormatT_convert(
    OS_LoggerAbstractFormat_Handle_t* self, /*!< [out] Result of the conversion
                                                       will be stored in the
                                                       underlying buffer */
    OS_LoggerEntry_t const* const     entry /*!< [in]  Log entry to be converted
                                                       to the given format. */
);

/**
 * @details %FormatT_print is an abstract function for the print function.
 *
 * @param   self:       pointer to the class
*/
void
FormatT_print(OS_LoggerAbstractFormat_Handle_t* self);

