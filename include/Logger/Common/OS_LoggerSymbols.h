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

#define OS_Logger_STR_(x)                         #x
#define OS_Logger_STR(x)                          OS_Logger_STR_(x)

// include parameter for configuration file
#if !defined(OS_Logger_CONFIG_H_FILE)
#   error "A configuration file must be provided!"
#else
#   include OS_Logger_STR(OS_Logger_CONFIG_H_FILE)
#endif


// configuration of log server
#if !defined (DATABUFFER_SIZE)
#define DATABUFFER_SIZE             4096
#endif

#define OS_Logger_LOG_LEVEL_SERVER_OFFSET   0
#define OS_Logger_LOG_LEVEL_CLIENT_OFFSET   OS_Logger_LOG_LEVEL_LENGTH

#define OS_Logger_MESSAGE_OFFSET            OS_Logger_LOG_LEVEL_CLIENT_OFFSET \
                                            + OS_Logger_LOG_LEVEL_LENGTH

#define OS_Logger_LOG_LEVEL_LENGTH          3 // Without the null terminator!
#define OS_Logger_MESSAGE_LENGTH            (DATABUFFER_SIZE \
                                            - (OS_Logger_LOG_LEVEL_LENGTH \
                                                + OS_Logger_LOG_LEVEL_LENGTH))

/**
 * @brief   Length of the string containing name and the id of the log client.
 *
 *          Size of it has been chosen arbitrary so that there is enough place
 *          for the id in the decimal format, and a descriptive name.
 *
 *          Size is fixed so that the fields of the log entries are nicely
 *          aligned.
 *
 *          If changed make sure there is enough place for both the name and id.
 */
#define OS_Logger_ID_AND_NAME_LENGTH        24

/**
 * @brief   Length of the string containing id of the log client.
 *
 *          The 32bit integer consists at max of 10 digits when displayed in
 *          decimal.
 */
#define OS_Logger_ID_LENGTH                 10

#define OS_Logger_NAME_LENGTH               (OS_Logger_ID_AND_NAME_LENGTH \
                                            - OS_Logger_ID_LENGTH)

//! Maximum log file name length (excluding the null terminator).
#define OS_Logger_MAX_FILENAME_LENGTH       19

// includes
#include <assert.h>

#define OS_Logger_CHECK_SELF(self)          assert(self)

