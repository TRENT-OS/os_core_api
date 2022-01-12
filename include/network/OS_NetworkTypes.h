/*
 * OS Network definitions
 *
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

/**
 * Maximum length of an IPv6 address string can be 45 chars (excluding null
 * terminator).
 * See RFC 4291 Section 2.2 for IP address reperesentation as string, these
 * formats are in use:
 *   - IPv4: 123.123.123.123 (15 chars)
 *   - IPv6: ABCD:EF01:2345:6789:ABCD:EF01:2345:6789 (38 chars)
 *   - IPv6/IPv4 mixed: FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:129.144.152.138 (45 Chars)
 */
#define IP_ADD_STR_MAX_LEN (45)

/** MAC Address size */
#define MAC_SIZE    6

/**
 * Ethernet frames are used on layer 2. IEEE 802.3 originally defined the max
 * frame size as 1518 bytes and the min size as 64 byte. Payload must be at
 * least 46 bytes and padding has to be used if the data is less. There is no
 * dedicated length field in the frames, the field type/len can contain a
 * length or a type. There may be start/stop conditiona on lower layers, so a
 * frame can be detected properly. Purely on layer 2 without any meta data from
 * the lower layers, one cannot accurately determine what a frame is. For
 * practical usage, this can be determines with reasonable accuracy, though.
 *
 *  | dst MAC | src MAC | type/len | payload   | CRC    |
 *  |---------|---------|----------|-----------|--------|
 *  | 6 byte  | 6 byte  | 2 byte   | 1500 byte | 4 byte |
 */
#define ETHERNET_MTU                    1500

/** Ethernet "jumbo frames", where the payload can be 9000 byte. */
#define ETHERNET_JUMBO_FRAME_MTU        9000

/**
 * With 802.1Q, the VLAN tags ("Q-tags") where added, so the max size increased
 * to 1522 bytes. The values for the tag field are well defined, so they can
 * distingueshed from a type/len field practically. But it's not trivial.
 *
 *  | dst MAC | src MAC | tag    | type/len | payload   | CRC    |
 *  |---------|---------|--------|----------|-----------|--------|
 *  | 6 byte  | 6 byte  | 4 byte | 2 byte   | 1500 byte | 4 byte |
 *
 * And with 802.1ad "Q-in-Q", multiple Q-tags can be present, so there is no
 * longer a max frame length. But for practical reasons, we can assume there
 * is just one tag - and thus we don't see frames longer than 1522 byte byte.
 */
#define ETHERNET_FRAME_MAX_SIZE     ( (2 * MAC_SIZE) + 4 + 2 + ETHERNET_MTU + 4 )


#if !defined(CUSTOM_OS_NETWORK_TYPES)

// Protocol families.
/** IPv4 protocol family. */
#define OS_AF_INET 2
/** IPv6 protocol family. */
#define OS_AF_INET6 10

// Socket definitions.
/** Stream socket - TCP. */
#define OS_SOCK_STREAM 1
/** Datagram socket - UDP. */
#define OS_SOCK_DGRAM 2

// Special network addresses.
/** Any address. */
#define OS_INADDR_ANY_STR      "0.0.0.0"
/** Loopback address. */
#define OS_INADDR_LOOPBACK_STR "127.0.0.1"

#endif
