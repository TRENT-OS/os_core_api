/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log server console backend implementation
 */
#pragma once

/**
 * @defgroup   OS_LoggerOutputFileSystem Log server filesystem backend
 *             implementation
 *
 * @brief      Derived class from class \link abstract_log_output \endlink and
 *             provides an implemention for log server filesystem backend.
 *
 * @details    This level provides an implementation save logs in a filesystem.
 *             \n
 *             It uses as class interface "OS_LoggerOutput_Handle_t".
 *             \n
 *             There is a dependency on "seos_filesystem_core" for storing the
 *             logs in the file system.
 *             \n \n
 *             Further implements this layer the observer functions.
 *
 * @ingroup    OS_LoggerOutput
*/
#include "Logger/Server/OS_LoggerOutput.h"


/**
 * @details %OS_LoggerOutputFileSystem_ctor is the constructor.
 *
 * @param   self:       pointer to the class
 * @param   log_format: pointer to log format layer
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - log_format is a NULL pointer.
 * @retval  OS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerOutputFileSystem
*/
OS_Error_t
OS_LoggerOutputFileSystem_ctor(
    OS_LoggerOutput_Handle_t* self,
    OS_LoggerFormat_Handle_t* log_format);