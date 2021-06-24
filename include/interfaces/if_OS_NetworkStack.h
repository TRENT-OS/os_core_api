/*
 * Copyright (C) 2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "OS_Dataport.h"
#include "OS_Error.h"
#include "network/OS_Network_types.h"

typedef struct
{
    OS_Error_t (*socket_create)(int domain, int type, int* pHandle);

    OS_Error_t (*socket_accept)(int handle, int* pHandleClient, uint16_t port);

    OS_Error_t (*socket_bind)(int handle, uint16_t port);

    OS_Error_t (*socket_listen)(int handle, int backlog);

    OS_Error_t (*socket_connect)(int handle, const char* name, uint16_t port);

    OS_Error_t (*socket_close)(int handle);

    OS_Error_t (*socket_write)(int handle, size_t* pLen);

    OS_Error_t (*socket_read)(int handle, size_t* pLen);

    OS_Error_t (*socket_recvfrom)(
        int                  handle,
        size_t*              plen,
        OS_Network_Socket_t* src_socket);

    OS_Error_t (*socket_sendto)(
        int                 handle,
        size_t*             pLen,
        OS_Network_Socket_t dst_socket);

    OS_Dataport_t dataport;
} if_OS_NetworkStack_t;

#define IF_OS_NETWORKSTACK_ASSIGN(_rpc_, _port_)                               \
    {                                                                          \
        .socket_create   = _rpc_##_socket_create,                              \
        .socket_accept   = _rpc_##_socket_accept,                              \
        .socket_bind     = _rpc_##_socket_bind,                                \
        .socket_listen   = _rpc_##_socket_listen,                              \
        .socket_connect  = _rpc_##_socket_connect,                             \
        .socket_close    = _rpc_##_socket_close,                               \
        .socket_write    = _rpc_##_socket_write,                               \
        .socket_read     = _rpc_##_socket_read,                                \
        .socket_sendto   = _rpc_##_socket_sendto,                              \
        .socket_recvfrom = _rpc_##_socket_recvfrom,                            \
                                                                               \
        .dataport = OS_DATAPORT_ASSIGN(_port_)                                 \
    }
