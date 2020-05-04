/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Implementation of concrete log emitter.
 */
#pragma once

/**
 * @defgroup    OS_LoggerEmitter Log emitter implementation
 *
 * @brief       This layer provides a implementation of concrete log emitter.
 * @details     On the client side, a log emitter is the object that transceives
 *              log messages to a server.
 *
 *              A separate log emitter object must be created for each client.
 *              It will be automatically created by calling a Debug_LOG
 *              function.
 *              \n \n
 *              Log emitter starts to transceives log messages, when it got the
 *              signal from the server.
 *
 *              The log emitter copy the log message and the client log level in
 *              the given data buffer.
 *
 *              Afterwards it sends a signal to server.
 *              \n \n
 *              The log emitter object is the central object that offers
 *              interfaces for:
 *              \code
 *                  log filter
 *                  flow control functions
 *              \endcode
 *
 *              The latter is the class \link log_emitter_callback \endlink .
 *
 * @see         log_emitter_callback
 *
 * @ingroup     OS_LoggerClient
*/
#include "SeosError.h"
#include "Logger/Common/OS_LoggerFilter.h"
#include "seos_types.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @details OS_LoggerEmitter_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerEmitter
*/
typedef struct OS_LoggerEmitter_Handle OS_LoggerEmitter_Handle_t;

/**
 * @details %OS_LoggerEmitter_getInstance is a getter to instantiate this object.
 *
 *          This is an singleton implementation i.e. it will internally create a
 *          log emitter object.
 *
 *          If the object has been successfully created, the pointer to this
 *          object will returned, otherwise this functions will return a NULL
 *          pointer.
 *
 *          All layer functions are interchangeable due to their special
 *          implementation.
 *
 * @return  pointer to OS_LoggerEmitter_Handle_t object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup OS_LoggerEmitter
*/
OS_LoggerEmitter_Handle_t*
OS_LoggerEmitter_getInstance(
    void* buffer,                         //!< The logging data exchange buffer.
    OS_LoggerFilter_Handle_t* log_filter, //!< The client's logging filter.
    event_notify_func_t emit);            //!< The logging emit callback.

/**
 * @details %OS_LoggerEmitter_dtor is the destructor.
 *
 * @ingroup OS_LoggerEmitter
*/
void
OS_LoggerEmitter_dtor(void);

/**
 * @details %OS_LoggerEmitter_log provides the client log action.
 *
 *          This function is called by the debug log interface function
 *          Debug_LOG _XXX.
 *
 *          The function has variable parameters so that a log message can be
 *          called up as usual with the syntax of the printf function.
 *
 * @param   log_level:  client log level id
 * @param   format:     log message
 * @param   ...:        optional parameters for log message
 *
 * @return  An error code.
 *
 * @retval  SEOS_ERROR_INVALID_HANDLE    - OS_LoggerEmitter_getInstance was not
 *                                         called.
 * @retval  SEOS_ERROR_INVALID_PARAMETER - Given format is null or incorrect.
 * @retval  SEOS_ERROR_GENERIC           - String encoding error occurred.
 * @retval  SEOS_ERROR_BUFFER_TOO_SMALL  - Destination log buffer too small.
 * @retval  SEOS_SUCCESS                 - Entry logged or filtered out.
 *
 * @ingroup OS_LoggerEmitter
*/
seos_err_t
OS_LoggerEmitter_log(uint8_t log_level, const char* format, ...);
