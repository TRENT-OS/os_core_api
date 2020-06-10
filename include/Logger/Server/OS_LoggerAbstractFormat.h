/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Abstract log format interface
 */
#pragma once

/**
 * @defgroup    OS_LoggerAbstractFormat Abstract log format interface
 *
 * @brief       Defined abstract format interface for log format.
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
 *
 * @ingroup OS_LoggerAbstractFormat
*/
typedef struct OS_LoggerAbstractFormat_Handle OS_LoggerAbstractFormat_Handle_t;

/**
 * @brief   Defines the interface for functions pointer to the convert function.
 *
 *          The data of log level id, log level string and the log message are
 *          converted into a defined format.
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
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
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
typedef void
(*OS_LoggerAbstractFormat_print_t)(OS_LoggerAbstractFormat_Handle_t* self);


/**
 * @details OS_LoggerAbstractFormat_vtable_t contain the member functions to his
 *          class.
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
typedef struct
{
    OS_LoggerAbstractFormat_convert_t convert;
    OS_LoggerAbstractFormat_print_t   print;
} OS_LoggerAbstractFormat_vtable_t;


/**
 * @details OS_LoggerAbstractFormat_Handle_t contain the vtable to his class.
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
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
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
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
 *
 * @ingroup OS_LoggerAbstractFormat_Handle_t
*/
void
FormatT_print(OS_LoggerAbstractFormat_Handle_t* self);

