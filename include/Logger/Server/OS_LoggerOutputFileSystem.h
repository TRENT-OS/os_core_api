/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @file
 * @brief       Log server filesystem backend implementation
 *
 * @details     Derived class from class \link OS_LoggerOutput \endlink and
 *              provides an implemention for log server filesystem backend.
 *
 *              This level provides an implementation for saving logs in a
 *              file.
 *
 *              It is a child class of \link OS_LoggerOutput_Handle_t \endlink.
 *
 *              Function update() is used for storing logs in a file.
 *
 *              This layer implements the observer update() callback.
 *
 * @ingroup     OS_LoggerOutput
*/
#pragma once

#include "Logger/Server/OS_LoggerOutput.h"

/**
 * @brief   Child class constructor.
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - log_format is a NULL pointer.
 * @retval  OS_SUCCESS                 - Operation was successful.
*/
OS_Error_t
OS_LoggerOutputFileSystem_ctor(
    OS_LoggerOutput_Handle_t* self,      //!< [in] This pointer.
    OS_LoggerFormat_Handle_t* logFormat  //!< [in] Printing format.
);
