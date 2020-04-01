/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log emitter flow control interface.
 */
#pragma once

/**
 * @defgroup    OS_LoggerEmitterCallback Log emitter flow control interface
 *
 * @brief       This layer provides functions for log emitter flow control.
 * @details     The functions of this level serve to control the flow of log
 *              emitters.
 *
 *              This layer only defines the interface functions.
 *              The implementation itself does not take place
 *
 * @ingroup     OS_LoggerClient
*/
#include <stdarg.h>
#include <stdbool.h>

/**
 * @details OS_LoggerEmitterCallback_emit_t defines the interface for function
 *          pointer to emit a signal, when client site logging process is done.
 *
 * @ingroup OS_LoggerEmitterCallback
*/
typedef void
(*OS_LoggerEmitterCallback_emit_t)(void);


/**
 * @details OS_LoggerEmitterCallback_wait_t defines the interface for function
 *          pointer to wait for a signal.
 *
 * @ingroup OS_LoggerEmitterCallback
*/
typedef void
(*OS_LoggerEmitterCallback_wait_t)(void);


/**
 * @details OS_LoggerEmitterCallback_Handle_t contain interface functions.
 *
 * @ingroup OS_LoggerEmitterCallback
*/
typedef struct
{
    OS_LoggerEmitterCallback_emit_t client_emit;
    OS_LoggerEmitterCallback_wait_t client_wait;
} OS_LoggerEmitterCallback_Handle_t;


/**
 * @details %OS_LoggerEmitterCallback_ctor is the constructor.
 *
 * @param   self:           pointer to the class
 * @param   client_wait:    function pointer to client wait callback function
 * @param   client_emit:    function pointer to client emit callback function
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup OS_LoggerEmitterCallback
*/
bool
OS_LoggerEmitterCallback_ctor(
    OS_LoggerEmitterCallback_Handle_t* self,
    OS_LoggerEmitterCallback_wait_t      client_wait,
    OS_LoggerEmitterCallback_emit_t      client_emit);


/**
 * @details %OS_LoggerEmitterCallback_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerEmitterCallback
*/
void
OS_LoggerEmitterCallback_dtor(OS_LoggerEmitterCallback_Handle_t* self);

