/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @file
 * @brief       Log format implementation.
 *
 * @details     Derived class from class \link OS_LoggerAbstractFormat_Handle
 *              \endlink and provides an implemention for log format.
 *
 *              This level provides an implementation for converting and
 *              printing log format.
 *
 *              This class is derived by abstract format.
 *              \n
 *              The print function is a helper function to print the temporary
 *              log format from "OS_LoggerFormat_Handle_t->buffer" via
 *              "printf().
 *              \n
 *              Before print the log message to an backend, convert the log
 *              message in the defined format.
 *
 * @ingroup     OS_LoggerAbstractFormat
*/
#pragma once

#include "Logger/Server/OS_LoggerAbstractFormat.h"

/**
 * @brief   Length of the timestamp string
 *
 *          Timestamp has the following format ' dd.mm.yyyy-hh:mm:ss'.
 *
 * @note    Altough null terminator is not included in the calculation the value
 *          includes blank space in front to separate it from the previous
 *          field.
 */
#define OS_Logger_FORMAT_TIMESTAMP_LENGTH 20

/**
 * @brief   Required size for the buffer used to store the complete log message
 *          consisting of the message header and the mesage itself.
 *
 * @note    The spaces between the different segments of the header and the null
 *          terminator are also accounted for.
 */
#define OS_Logger_FORMAT_BUFFER_SIZE  ((OS_Logger_ID_LENGTH + 1)               \
                                      + (OS_Logger_NAME_LENGTH + 1)            \
                                      + (OS_Logger_FORMAT_TIMESTAMP_LENGTH + 1)\
                                      + (OS_Logger_LOG_LEVEL_LENGTH + 1)       \
                                      + (OS_Logger_LOG_LEVEL_LENGTH + 1)       \
                                      + OS_Logger_ENTRY_MESSAGE_LENGTH         \
                                      + 1) // including null terminator

/**
 * @details OS_LoggerFormat_Handle_t contain informations about the log format
 *          and a pointer the parent vtable.
*/
typedef struct
{
    const OS_LoggerAbstractFormat_vtable_t*
    vtable;                     /**< pointer to parent vtable */

    char
    buffer[OS_Logger_FORMAT_BUFFER_SIZE]; /**< buffer for log format message */
} OS_LoggerFormat_Handle_t;


/**
 * @details %OS_LoggerFormat_ctor is the constructor.
 *
 * @param   self:   pointer to the class
*/
void
OS_LoggerFormat_ctor(OS_LoggerFormat_Handle_t* self);

/**
 * @brief Prints data in the given format.
 * @param self:   pointer to the class
 */
void
OS_LoggerFormat_print(OS_LoggerAbstractFormat_Handle_t* self);

