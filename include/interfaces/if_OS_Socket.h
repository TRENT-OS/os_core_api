/*
 * Copyright (C) 2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "OS_Dataport.h"
#include "OS_Error.h"
#include "OS_Types.h"
#include "network/OS_SocketTypes.h"
#include "network/OS_NetworkStackTypes.h"

typedef struct
{
    OS_Error_t (*socket_create)(
        const int domain,
        const int type,
        int* const pHandle);

    OS_Error_t (*socket_accept)(
        const int handle,
        int* const pHandleClient,
        OS_Socket_Addr_t* const srcAddr);

    OS_Error_t (*socket_bind)(
        const int handle,
        const OS_Socket_Addr_t* const localAddr);

    OS_Error_t (*socket_listen)(
        const int handle,
        const int backlog);

    OS_Error_t (*socket_connect)(
        const int handle,
        const OS_Socket_Addr_t* const dstAddr);

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
        OS_Socket_Addr_t* const srcAddr);

    OS_Error_t (*socket_sendto)(
        const int handle,
        size_t* const pLen,
        const OS_Socket_Addr_t* const dstAddr);

    OS_NetworkStack_State_t (*socket_getStatus)(
        void);

    OS_Error_t (*socket_getPendingEvents)(
        const size_t bufSize,
        int* const pNumberOfEvents);

    void (*socket_wait)(
        void);

    int (*socket_poll)(
        void);

    int (*socket_regCallback)(
        void (*callback)(void*),
        void* arg);

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
    .socket_wait             = _prefix_##_event_notify_wait,                   \
    .socket_poll             = _prefix_##_event_notify_poll,                   \
    .socket_regCallback      = _prefix_##_event_notify_reg_callback,           \
                                                                               \
    .shared_resource_mutex_lock   = _prefix_##_shared_resource_mutex_lock,     \
    .shared_resource_mutex_unlock = _prefix_##_shared_resource_mutex_unlock,   \
                                                                               \
    .dataport = OS_DATAPORT_ASSIGN_FUNC((void*)_prefix_##_rpc_get_buf,         \
                                        _prefix_##_rpc_get_size)               \
}
