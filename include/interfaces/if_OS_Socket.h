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
    OS_Error_t (*socket_create)(
        const int domain,
        const int type,
        int* const pHandle);

    OS_Error_t (*socket_accept)(
        const int handle,
        int* const pHandleClient,
        OS_NetworkSocket_Addr_t* const srcAddr);

    OS_Error_t (*socket_bind)(
        const int handle,
        const OS_NetworkSocket_Addr_t* const localAddr);

    OS_Error_t (*socket_listen)(
        const int handle,
        const int backlog);

    OS_Error_t (*socket_connect)(
        const int handle,
        const OS_NetworkSocket_Addr_t* const dstAddr);

    OS_Error_t (*socket_close)(
        const int handle);

    OS_Error_t (*socket_write)(
        const int handle,
        size_t* const pLen);

    OS_Error_t (*socket_read)(
        const int handle,
        size_t* const pLen);

    OS_Error_t (*socket_recvfrom)(
        const int handle,
        size_t* const pLen,
        OS_NetworkSocket_Addr_t* const srcAddr);

    OS_Error_t (*socket_sendto)(
        const int handle,
        size_t* const pLen,
        const OS_NetworkSocket_Addr_t* const dstAddr);

    OS_Dataport_t dataport;
} if_OS_Socket_t;

#define IF_OS_SOCKET_ASSIGN(_rpc_, _port_)         \
{                                                  \
    .socket_create   = _rpc_##_socket_create,      \
    .socket_accept   = _rpc_##_socket_accept,      \
    .socket_bind     = _rpc_##_socket_bind,        \
    .socket_listen   = _rpc_##_socket_listen,      \
    .socket_connect  = _rpc_##_socket_connect,     \
    .socket_close    = _rpc_##_socket_close,       \
    .socket_write    = _rpc_##_socket_write,       \
    .socket_read     = _rpc_##_socket_read,        \
    .socket_sendto   = _rpc_##_socket_sendto,      \
    .socket_recvfrom = _rpc_##_socket_recvfrom,    \
    .dataport = OS_DATAPORT_ASSIGN(_port_)         \
}
