/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log consumer flow control interface.
 */
#pragma once

/**
 * @defgroup    OS_LoggerConsumerCallback Log consumer flow control interface
 *
 * @brief       This layer provides functions for log consumer flow control.
 * @details     The functions of this level serve to control the flow of log
 *              consumers.
 *
 *              This layer only defines the interface functions.
 *              The implementation itself does not take place
 *
 * @ingroup     OS_LoggerConsumer
*/
#include <stdint.h>
#include <stdbool.h>


/**
 * @details OS_LoggerConsumerCallback_emit_t defines the interface for the
 *          function pointer to emit a signal, when server site logging process
 *          is done.
 *
 * @ingroup OS_LoggerConsumerCallback
*/
typedef void
(*OS_LoggerConsumerCallback_emit_t)(void);


/**
 * @details OS_LoggerConsumerCallback_getSenderId_t defines the interface for
 *          the function pointer to get sender id by calling RPC function.
 *
 * @return  sender id
 *
 * @ingroup OS_LoggerConsumerCallback
*/
typedef uint32_t
(*OS_LoggerConsumerCallback_getSenderId_t)(void);


/**
 * @details OS_LoggerConsumerCallback_getTimestamp_t defines the interface for
 *          the function pointer to get timestamp.
 *
 * @return  timestamp
 *
 * @ingroup OS_LoggerConsumerCallback
*/
typedef uint64_t
(*OS_LoggerConsumerCallback_getTimestamp_t)(void);


/**
 * @details OS_LoggerConsumerCallback_t contain interface functions.
 *
 * @ingroup OS_LoggerConsumerCallback
*/
typedef struct
{
    OS_LoggerConsumerCallback_emit_t         server_emit;
    OS_LoggerConsumerCallback_getSenderId_t  get_sender_id;
    OS_LoggerConsumerCallback_getTimestamp_t get_timestamp;
} OS_LoggerConsumerCallback_t;


/**
 * @details %OS_LoggerConsumer_dtor is the constructor.
 *
 * @param   self:           pointer to the class
 * @param   server_emit:    function pointer to emit callback function
 * @param   get_sender_id:  function pointer to get sender id callback function
 * @param   get_timestamp:  function pointer to get timestamp callback function
 *
 * @return  a status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has occurred
 *
 * @ingroup OS_LoggerConsumerCallback
*/
bool
OS_LoggerConsumerCallback_ctor(
    OS_LoggerConsumerCallback_t*             self,
    OS_LoggerConsumerCallback_emit_t         server_emit,
    OS_LoggerConsumerCallback_getSenderId_t  get_sender_id,
    OS_LoggerConsumerCallback_getTimestamp_t get_timestamp);


/**
 * @details %OS_LoggerConsumerCallback_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerConsumerCallback
*/
void
OS_LoggerConsumerCallback_dtor(OS_LoggerConsumerCallback_t* self);
