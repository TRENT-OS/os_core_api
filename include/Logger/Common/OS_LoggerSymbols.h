/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Defines of log server internals
 */
#pragma once

/**
 * @defgroup    OS_LoggerSymbols Defines of log server internals
 *
 * @brief       Defines the configuration of the log server internals.
 * @details     This file contain configuration for the log databuffer and for
 *              the log format example implementation.
 *
 * @ingroup     OS_LoggerServer, OS_LoggerClient
 */

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

#define OS_Logger_ID_AND_NAME_LENGTH        20
#define OS_Logger_ID_LENGTH                 6
#define OS_Logger_NAME_LENGTH               (OS_Logger_ID_AND_NAME_LENGTH \
                                            - OS_Logger_ID_LENGTH)

#define OS_Logger_FILENAME_LENGTH           20

// includes
#include <assert.h>

#define OS_Logger_CHECK_SELF(self)          assert(self)

