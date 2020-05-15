/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Data buffer implementation.
 */
#pragma once

/**
 * @defgroup    OS_LoggerDataBuffer Data buffer implementation
 *
 * @brief       This layer provides functions to manage data buffer.
 * @details     There are functions for getting and setting information from an
 *              object to data buffer and vice versa.
 *
 * @ingroup     OS_LoggerServer
*/
#include "SeosError.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include "Logger/Server/OS_LoggerTimestamp.h"
#include <stdint.h>


/**
 * @details OS_LoggerDataBuffer_Handle_t contain informations from a data buffer
 *          in a structured way.
 *
 * @ingroup OS_LoggerDataBuffer
*/
typedef struct
{
    uint8_t log_level_srv;                         //!< server log level
    uint8_t log_level_client;                      //!< client log level
    char    log_message[OS_Logger_MESSAGE_LENGTH]; //!< buffer for log message
} OS_LoggerDataBuffer_Handle_t;


/**
 * @details OS_LoggerDataBuffer_info contains all information about a log.
 *
 * @ingroup OS_LoggerDataBuffer
*/
typedef struct
{
    char log_id_and_name[OS_Logger_ID_AND_NAME_LENGTH];
    OS_LoggerTimestamp_Handle_t timestamp;
    OS_LoggerDataBuffer_Handle_t log_databuffer;
} OS_LoggerDataBuffer_info;


/**
 * @details %OS_LoggerDataBuffer_setServerLogLevel copy the server log level to
 *          the data buffer.
 *
 * @param   buf:                data buffer
 * @param   log_level_server:   server log level
 *
 * @ingroup OS_LoggerDataBuffer
*/
void
OS_LoggerDataBuffer_setServerLogLevel(void* buf, uint8_t log_level_server);


/**
 * @details %OS_LoggerDataBuffer_setClientLogLevel copy the client log level to
 *          the data buffer.
 *
 * @param   buf:                data buffer
 * @param   log_level_client:   client log level
 *
 * @ingroup OS_LoggerDataBuffer
*/
void
OS_LoggerDataBuffer_setClientLogLevel(void* buf, uint8_t log_level_client);


/**
 * @details %OS_LoggerDataBuffer_setLogMessage copy the log message to the data
 *          buffer.
 *
 * @param   buf:    data buffer
 * @param   msg:    log message
 *
 * @return  An error code.
 *
 * @retval  SEOS_ERROR_BUFFER_TOO_SMALL  - Target buffer is too small.
 * @retval  SEOS_ERROR_GENERIC           - An encoding error (for string and
 *                                         character conversion specifiers)
 *                                         occurred.
 * @retval  SEOS_SUCCESS                 - Operation was successful.
 *
 *
 * @ingroup OS_LoggerDataBuffer
*/
seos_err_t
OS_LoggerDataBuffer_setLogMessage(void* buf, const char* msg);


/**
 * @details %OS_LoggerDataBuffer_getServerLogLevel copy the server log level
 *          from the data buffer to internal data buffer.
 *
 * @param   buf:            data buffer
 * @param   log_databuffer: pointer to internal data buffer
 *
 * @return  An error code.
 *
 * @retval  SEOS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                         pointer.
 * @retval  SEOS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerDataBuffer
*/
seos_err_t
OS_LoggerDataBuffer_getServerLogLevel(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer);


/**
 * @details %OS_LoggerDataBuffer_getClientLogLevel copy the client log level
 *          from the data buffer to internal data buffer.
 *
 * @param   buf:            data buffer
 * @param   log_databuffer: pointer to internal data buffer
 *
 * @return  An error code.
 *
 * @retval  SEOS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                         pointer.
 * @retval  SEOS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerDataBuffer
*/
seos_err_t
OS_LoggerDataBuffer_getClientLogLevel(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer);


/**
 * @details %OS_LoggerDataBuffer_getLogMessage copy the log message from the
 *          data buffer to internal data buffer.
 *
 * @param   buf:            data buffer
 * @param   log_databuffer: pointer to internal data buffer
 *
 * @return  An error code.
 *
 * @retval  SEOS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                         pointer.
 * @retval  SEOS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerDataBuffer
*/
seos_err_t
OS_LoggerDataBuffer_getLogMessage(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer);


/**
 * @details %OS_LoggerDataBuffer_getInfo is an helper function, which getting
 *          all informatations from the data buffer by calling:
 *
 *          \code
 *              OS_LoggerDataBuffer_getServerLogLevel
 *              OS_LoggerDataBuffer_getClientLogLevel
 *              OS_LoggerDataBuffer_getLogMessage
 *          \endcode
 *
 * @param   buf:            data buffer
 * @param   log_databuffer: pointer to internal data buffer
 *
 * @return  An error code.
 *
 * @retval  SEOS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                         pointer.
 * @retval  SEOS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerDataBuffer
*/
seos_err_t
OS_LoggerDataBuffer_getInfo(
    void* buf,
    OS_LoggerDataBuffer_Handle_t* log_databuffer);


/**
 * @details %OS_LoggerDataBuffer_clear clean the data buffer.
 *
 * @param   buf:            data buffer
 *
 * @return  An error code.
 *
 * @retval  SEOS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                         pointer.
 * @retval  SEOS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerDataBuffer
*/
seos_err_t
OS_LoggerDataBuffer_clear(void* buf);
