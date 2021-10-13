/*
 * OS Network Stack type definitions
 *
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "OS_NetworkTypes.h"
#include "OS_Dataport.h"

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

/**
 * Abstracts the states a Network Stack component can be in.
 */
typedef enum
{
    UNINITIALIZED,
    INITIALIZED,
    RUNNING,
    FATAL_ERROR
} OS_NetworkStack_State_t;

/**
 * Abstracts the IP Address Configuration of a Network Stack.
 */
typedef struct
{
    char dev_addr[IP_ADD_STR_MAX_LEN]; //!< Device IP Address String.
    char gateway_addr[IP_ADD_STR_MAX_LEN]; //!< Gateway IP Address String.
    char subnet_mask[IP_ADD_STR_MAX_LEN]; //!< SubnetMask IP Address String.
} OS_NetworkStack_AddressConfig_t;

/**
 * Currently used to synchronize between NIC drivers using the legacy interface
 * and the Network Stack.
 * When the len is set to 0 it means the frame stored in data has been processed
 * and the buffer can be reused. If len is non-zero it indicates the length of
 * the data stored in the buffer, which will be used by the Network Stack.
 */
typedef struct
{
    uint8_t         data[OS_DATAPORT_DEFAULT_SIZE - 16]; //!< Data buffer.
    volatile size_t len; //!< Length of data stored in buffer.
} OS_NetworkStack_RxBuffer_t;
