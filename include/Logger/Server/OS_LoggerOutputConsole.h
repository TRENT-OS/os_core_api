/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server console backend implementation
 */
#pragma once

/**
 * @defgroup    OS_LoggerOutputConsole Log server console backend
 *
 * @brief       Derived class from class \link OS_LoggerOutput \endlink that
 *              provides an implemention for log server console backend.
 *
 * @details     This level provides an implementation for printing the log on
 *              the console.
 *
 *              It is a child class of \link OS_LoggerOutput_Handle_t \endlink.
 *
 *              For sending logs to console uses function update().
 *
 *              This layer implements the observer update() callback.
 *
 * @ingroup     OS_LoggerOutput
*/
#include "Logger/Server/OS_LoggerOutput.h"

/**
 * @brief   Child class constructor.
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - log_format is a NULL pointer.
 * @retval  OS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerOutputConsole
*/
OS_Error_t
OS_LoggerOutputConsole_ctor(
    OS_LoggerOutput_Handle_t* self,      //!< [in] This pointer.
    OS_LoggerFormat_Handle_t* logFormat  //!< [in] Printing format.
);
