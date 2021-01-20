/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 *
 * @file
 * @brief   Network stack type definitions
 * @details Type definitions used by TRENTOS-M Network stack
 * @ingroup OS_Network
 */

#pragma once
#include <stdint.h>

/**
 * @brief   contains elements that must be filled
 *          out by the APP to configure it to listen on a given port.
 */
typedef struct
{
    int domain;  /**< domain is as of now AF_INET (IPv4)*/
    int type;    /**< type is as of now SOCK_STREAM (TCP)*/
    uint16_t listen_port; /**< port to listen to e.g. 5555 */
    int backlog; /**< # of connections accepted */
} OS_NetworkServer_Socket_t;

/**
 * @brief   contains elements that must be filled by APP
 *          to connect to a remote host.
 */

// Maximum length of an IPv6 address string can be 45 chars + 1 for '\0'
// See RFC 4291 Section 2.2 for IP address reperesentation as string, these
// formats are in use:
//   IPv4: 123.123.123.123 (15 chars)
//   IPv6: ABCD:EF01:2345:6789:ABCD:EF01:2345:6789 (38 chars)
//   IPv6/IPv4 mixed:  FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:129.144.152.138 (45 Chars)
#define IP_ADD_STR_MAX_LEN (45 + 1)

typedef struct
{
    int domain; /**< domain is as of now AF_INET (IPv4) */
    int type;   /**< type is as of now SOCK_STREAM (TCP) */
    uint16_t port;       /**< port is for e.g. HTTP port 80 */
    char     name[IP_ADD_STR_MAX_LEN]; /**< IP addr to connect to (e.g.
                                                       "10.0.0.1", "::1" ) */
} OS_Network_Socket_t;

#if !defined(CUSTOM_OS_NETWORK_STACK_API_TYPES)

// Protocol families.
/** IPv4 */
#define OS_AF_INET 2 // IP protocol family.
/** IPv6 */
#define OS_AF_INET6 10 // IP version 6.

// Socket definition
/** Stream socket - TCP */
#define OS_SOCK_STREAM 2
/** Datagram socket - UDP */
#define OS_SOCK_DGRAM 1

typedef void*    OS_Network_Context_t;
typedef int OS_NetworkSocket_Handle_t;
typedef int OS_NetworkServer_Handle_t;

#endif
