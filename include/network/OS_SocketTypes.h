/*
 * Socket API type definitions
 *
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#pragma once

#include "OS_Error.h"
#include "OS_NetworkTypes.h"

#include <stdint.h>

/**
 * Possible events that can occur on a socket.
 */
#define OS_SOCK_EV_NONE      (0<<0) //!< No event happened.
#define OS_SOCK_EV_CONN_EST  (1<<0) //!< Connection established (TCP only).
#define OS_SOCK_EV_CONN_ACPT (1<<1) //!< Connection accepted (TCP only).
#define OS_SOCK_EV_READ      (1<<2) //!< Data arrived on the socket.
#define OS_SOCK_EV_WRITE     (1<<3) //!< Ready to write to the socket (TCP only).
#define OS_SOCK_EV_FIN       (1<<4) //!< FIN segment received (TCP only).
#define OS_SOCK_EV_CLOSE     (1<<5) //!< Socket is closed (TCP only).
#define OS_SOCK_EV_ERROR     (1<<6) //!< An error occurred.

/**
 * Abstracts a socket IP address.
 */
typedef struct
{
    char     addr[IP_ADD_STR_MAX_LEN + 1]; //!< IP Address String.
    uint16_t port; //!< IP Port.
} OS_Socket_Addr_t;

/**
 * Abstracts a socket event package exchanged by a client and a Network Stack
 * component.
 */
typedef struct __attribute__((packed))
{
    int        socketHandle; //!< Handle ID of the socket.
    int        parentSocketHandle; //!< Handle ID of the parent socket.
    uint8_t    eventMask; //!< Event mask of the socket.
    OS_Error_t currentError; //!< Current error of the socket.
}
OS_Socket_Evt_t;
