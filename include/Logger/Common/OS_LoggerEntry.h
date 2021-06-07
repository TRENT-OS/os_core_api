/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief Log entry data structure
 *
 * @details     Contains all the data describing a log entry.
 *
 *              Reference to the instance of this structure is passed around
 *              the logger stack.
 *
 * @note        It contains flexible array member with a message string which
 *              must be placed at the end of the structure.
 *
 * @ingroup     OS_LoggerServer, OS_LoggerClient
*/
#pragma once

#include "Logger/Common/OS_LoggerSymbols.h"
#include <stdint.h>

/**
 * @brief Log entry metadata which are set on the emitter side.
 */
typedef struct __attribute__((packed))
{
    uint8_t level;          //!< Log level of the current entry.
    uint8_t filteringLevel; //!< The emitter's filtering level.
}
OS_LoggerEmitterMetadata_t;

/**
 * @brief Log entry metadata which are set on the consumer side.
 */
typedef struct __attribute__((packed))
{
    //! Timestamp of the entry.
    uint64_t timestamp;
    //! Identifier assigned to the emitter.
    uint32_t id;
    //! Name of the emitter (including null terminator).
    char name[OS_Logger_NAME_LENGTH + 1];
    //! The consumer's filtering level.
    uint8_t filteringLevel;
}
OS_LoggerConsumerMetadata_t;

/**
 * @brief Log entry with all the metadata and the log message itself.
 */
typedef struct __attribute__((packed))
{
    /** Metadata assigned by emitter */
    OS_LoggerEmitterMetadata_t  emitterMetadata;
    /** Metadata assigned by consumer */
    OS_LoggerConsumerMetadata_t consumerMetadata;
    /** Log entry's message */
    char msg[];
}
OS_LoggerEntry_t;
