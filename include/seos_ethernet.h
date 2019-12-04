/*
 *  SEOS Ethernet definitions
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once


#define MAC_SIZE    6

// IEEE 802.3 origianlly defined the maximum Ethernet frame size as 1518 bytes,
// but this was later increased to 1522 bytes to allow for VLAN tagging. So an
// ethernet frame today looks like this:
//
//  +---------+---------+--------+----------+-----------+--------+
//  | dst MAC | src MAC | tag    | type/len | payload   | CRC    |
//  | 6 byte  | 6 byte  | 4 byte | 2 byte   | 1500 byte | 4 byte |
//  +---------+---------+--------+----------+-----------+--------+

#define ETHERNET_PAYLOAD_MAX_SIZE   1500

#define ETHERNET_FRAME_MAX_SIZE     ( (2 * MAC_SIZE) + 4 + 2 + \
                                      ETHERNET_PAYLOAD_MAX_SIZE + 4 )
