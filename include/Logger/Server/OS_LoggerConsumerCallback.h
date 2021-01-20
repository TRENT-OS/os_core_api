/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief       Log consumer flow control interface.
 *
 * @details     The functions of this level serve to control the flow of log
 *              consumers.
 *
 *              This layer only defines the interface functions.
 *              The implementation itself does not take place
 *
 * @ingroup     OS_LoggerConsumer
*/
#pragma once

#include "OS_Error.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details OS_LoggerConsumerCallback_emit_t defines the interface for the
 *          function pointer to emit a signal, when server site logging process
 *          is done.
*/
typedef void
(*OS_LoggerConsumerCallback_emit_t)(void);


/**
 * @details OS_LoggerConsumerCallback_getSenderId_t defines the interface for
 *          the function pointer to get sender id by calling RPC function.
 *
 * @return  sender id
*/
typedef uint32_t
(*OS_LoggerConsumerCallback_getSenderId_t)(void);


/**
 * @details OS_LoggerConsumerCallback_getTimestamp_t defines the interface for
 *          the function pointer to get timestamp.
 *
 * @return  timestamp
*/
typedef uint64_t
(*OS_LoggerConsumerCallback_getTimestamp_t)(void);


/**
 * @details OS_LoggerConsumerCallback_t contain interface functions.
*/
typedef struct
{
    OS_LoggerConsumerCallback_getSenderId_t  get_sender_id;
    OS_LoggerConsumerCallback_getTimestamp_t get_timestamp;
} OS_LoggerConsumerCallback_t;


/**
 * @details %OS_LoggerConsumer_ctor is the constructor.
 *
 * @param   self:           pointer to the class
 * @param   get_sender_id:  function pointer to get sender id callback function
 * @param   get_timestamp:  function pointer to get timestamp callback function
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - get_sender_id is a NULL pointer.
 * @retval  OS_SUCCESS                 - Operation was successful.
 * @retval  other                      - Implementation specific.
*/
OS_Error_t
OS_LoggerConsumerCallback_ctor(
    OS_LoggerConsumerCallback_t*             self,
    OS_LoggerConsumerCallback_getSenderId_t  get_sender_id,
    OS_LoggerConsumerCallback_getTimestamp_t get_timestamp);
