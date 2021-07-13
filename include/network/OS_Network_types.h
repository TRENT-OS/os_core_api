/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
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
 * @brief   contains elements that must be filled by APP
 *          to connect to a remote host.
 */

// Maximum length of an IPv6 address string can be 45 chars (excluding null
// terminator).
// See RFC 4291 Section 2.2 for IP address reperesentation as string, these
// formats are in use:
//   IPv4: 123.123.123.123 (15 chars)
//   IPv6: ABCD:EF01:2345:6789:ABCD:EF01:2345:6789 (38 chars)
//   IPv6/IPv4 mixed:  FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:129.144.152.138 (45 Chars)
#define IP_ADD_STR_MAX_LEN (45)

typedef struct
{
    uint8_t  addr[IP_ADD_STR_MAX_LEN + 1];
    uint16_t port;
} OS_NetworkSocket_Addr_t;

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

#endif
