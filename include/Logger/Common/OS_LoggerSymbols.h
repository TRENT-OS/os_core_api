/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief       Defines the configuration of the log server internals.
 *
 * @details     This file contain configuration for the log databuffer and for
 *              the log format example implementation.
 *
 * @ingroup     OS_LoggerServer, OS_LoggerClient
 */
#pragma once

#define OS_Logger_STR_(x)                   #x
#define OS_Logger_STR(x)                    OS_Logger_STR_(x)

// include parameter for configuration file
#if !defined(OS_Logger_CONFIG_H_FILE)
#   error "A configuration file must be provided!"
#else
#   include OS_Logger_STR(OS_Logger_CONFIG_H_FILE)
#endif

//! Length of the log level (excluding the null terminator).
#define OS_Logger_LOG_LEVEL_LENGTH          3

//! Length of the id of the log client (excluding the null terminator). The
//! 32bit integer is displayed with at most 10 decimal digits.
#define OS_Logger_ID_LENGTH                 10

//! Length of the name of the log client (excluding the null terminator).
#define OS_Logger_NAME_LENGTH               13

//! Maximum log file name length (excluding the null terminator).
#define OS_Logger_MAX_FILENAME_LENGTH       19

// includes
#include <assert.h>

#define OS_Logger_CHECK_SELF(self)          assert(self)
