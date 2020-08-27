/* Copyright (C) 2019-2020, Hensoldt Cyber GmbH */

#pragma once

#include "OS_Dataport.h"
#include "OS_Error.h"

#include <stdint.h>

typedef struct
{
    OS_Error_t (*write)(size_t offset, size_t size, size_t* written);
    OS_Error_t (*read)(size_t offset, size_t size, size_t* read);
    OS_Error_t (*erase)(size_t offset, size_t size, size_t* erased);
    OS_Error_t (*getSize)(size_t* size);
    OS_Error_t (*getState)(uint32_t* flags);
    OS_Dataport_t dataport;
} if_OS_Storage_t;

#define IF_OS_STORAGE_ASSIGN(_rpc_, _port_)     \
{                                               \
    .write    = _rpc_ ## _write,                \
    .read     = _rpc_ ## _read,                 \
    .erase    = _rpc_ ## _erase,                \
    .getSize  = _rpc_ ## _getSize,              \
    .getState = _rpc_ ## _getState,             \
    .dataport = OS_DATAPORT_ASSIGN(_port_)      \
}
