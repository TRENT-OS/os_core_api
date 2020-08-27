/* Copyright (C) 2019-2020, Hensoldt Cyber GmbH */

#pragma once

#include "OS_Dataport.h"
#include "OS_Error.h"

#include <stdint.h>

typedef struct
{
    OS_Error_t (*handshake)(void);
    OS_Error_t (*write)(size_t dataSize);
    OS_Error_t (*read)(size_t* dataSize);
    OS_Error_t (*reset)(void);
    OS_Dataport_t dataport;
} if_OS_Tls_t;

#define IF_OS_TLS_ASSIGN(_rpc_, _port_)         \
{                                               \
    .handshake  = _rpc_ ## _handshake,          \
    .write      = _rpc_ ## _write,              \
    .read       = _rpc_ ## _read,               \
    .reset      = _rpc_ ## _reset,              \
    .dataport   = OS_DATAPORT_ASSIGN(_port_)    \
}
