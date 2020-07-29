/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief All common logging interfaces are placed here.
 *
 * If it is desired to use Logger module, this file shall be included in the
 * config file of the project and Debug_Config_PRINT_TO_LOG_SERVER must be
 * defined, what is done automatically when Logger library is linked.
 *
 */
#pragma once

/**
 * @defgroup    OS_Logger OS Logger API
 *
 * @brief       An abstraction of the logging functionality
 *
 * Allows setting up a logger server to which clients can send logging
 * entries.
 *
 */

/**
 * @defgroup    OS_LoggerClient OS Logger Client API
 *
 * @brief       Client's specific functionality of the Logger library.
 *
 * @ingroup     OS_Logger
 */

#if defined(Debug_Config_PRINT_TO_LOG_SERVER)

#include "Logger/Client/OS_LoggerEmitter.h"

#define Debug_PRINT__(LEVEL, ...) \
            OS_LoggerEmitter_log(LEVEL, __VA_ARGS__)

#endif // defined(Debug_Config_PRINT_TO_LOG_SERVER)
