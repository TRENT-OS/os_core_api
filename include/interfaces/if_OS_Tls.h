/*
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "OS_Dataport.h"
#include "OS_Error.h"

#include <stdint.h>

typedef struct
{
    OS_Error_t (*handshake)(void);
    OS_Error_t (*write)(size_t* dataSize);
    OS_Error_t (*read)(size_t* dataSize);
    OS_Error_t (*reset)(void);
    OS_Dataport_t dataport;
} if_OS_Tls_t;

#define IF_OS_TLS_ASSIGN(_prefix_)                                             \
{                                                                              \
    .handshake  = _prefix_##_rpc_handshake,                                    \
    .write      = _prefix_##_rpc_write,                                        \
    .read       = _prefix_##_rpc_read,                                         \
    .reset      = _prefix_##_rpc_reset,                                        \
    .dataport   = OS_DATAPORT_ASSIGN_FUNC((void*)_prefix_##_rpc_get_buf,       \
                                          _prefix_##_rpc_get_size)             \
}
