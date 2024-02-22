/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#pragma once

#include "OS_Dataport.h"
#include "OS_Error.h"

#include <stdint.h>

typedef struct
{
    size_t (*read)(const size_t len);
    OS_Dataport_t dataport;
} if_OS_Entropy_t;

#define IF_OS_ENTROPY_ASSIGN(_rpc_, _port_)     \
{                                               \
    .read     = _rpc_ ## _read,                 \
    .dataport = OS_DATAPORT_ASSIGN(_port_)      \
}
