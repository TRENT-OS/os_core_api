/*
 * Copyright (C) 2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "OS_Dataport.h"
#include "OS_Error.h"
#include "OS_Types.h"
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

    OS_NetworkStack_State_t (*socket_getStatus)(
        void);

    OS_Error_t (*socket_getPendingEvents)(
        const size_t bufSize,
        int* const pNumberOfEvents);

    mutex_lock_func_t   shared_resource_mutex_lock;
    mutex_unlock_func_t shared_resource_mutex_unlock;

    OS_Dataport_t dataport;
} if_OS_Socket_t;

#define IF_OS_SOCKET_ASSIGN(_prefix_)                                          \
{                                                                              \
    .socket_create           = _prefix_##_rpc_socket_create,                   \
    .socket_accept           = _prefix_##_rpc_socket_accept,                   \
    .socket_bind             = _prefix_##_rpc_socket_bind,                     \
    .socket_listen           = _prefix_##_rpc_socket_listen,                   \
    .socket_connect          = _prefix_##_rpc_socket_connect,                  \
    .socket_close            = _prefix_##_rpc_socket_close,                    \
    .socket_write            = _prefix_##_rpc_socket_write,                    \
    .socket_read             = _prefix_##_rpc_socket_read,                     \
    .socket_sendto           = _prefix_##_rpc_socket_sendto,                   \
    .socket_recvfrom         = _prefix_##_rpc_socket_recvfrom,                 \
    .socket_getStatus        = _prefix_##_rpc_socket_getStatus,                \
    .socket_getPendingEvents = _prefix_##_rpc_socket_getPendingEvents,         \
                                                                               \
    .shared_resource_mutex_lock   = _prefix_##_shared_resource_mutex_lock,     \
    .shared_resource_mutex_unlock = _prefix_##_shared_resource_mutex_unlock,   \
                                                                               \
    .dataport = OS_DATAPORT_ASSIGN_FUNC((void*)_prefix_##_rpc_get_buf,         \
                                        _prefix_##_rpc_get_size)               \
}
