/*
 * OS Ethernet definitions
 *
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 */

#pragma once


#define MAC_SIZE    6

// Ethernet frames are used on layer 2. IEEE 802.3 originally defined the max
// frame size as 1518 bytes and the min size as 64 byte. Payload must be at
// least 46 bytes and padding has to be used if the data is less. There is no
// dedicated length field in the frames, the field type/len can contain a
// length or a type. There may be start/stop conditiona on lower layers, so a
// frame can be detected properly. Purely on layer 2 without any meta data from
// the lower layers, one cannot accurately determine what a frame is. For
// practical usage, this can be determines with reasonable accuracy, though.
//
//  +---------+---------+----------+-----------+--------+
//  | dst MAC | src MAC | type/len | payload   | CRC    |
//  | 6 byte  | 6 byte  | 2 byte   | 1500 byte | 4 byte |
//  +---------+---------+----------+-----------+--------+

#define ETHERNET_MTU                    1500

// There are also "jumbo frames", where the payload can be 9000 byte.
#define ETHERNET_JUMBO_FRAME_MTU        9000


// With 802.1Q, the VLAN tags ("Q-tags") where added, so the max size increased
// to 1522 bytes. The values for the tag field are well defined, so they can
// distingueshed from a type/len field practically. But it's not trivial.
//
//  +---------+---------+--------+----------+-----------+--------+
//  | dst MAC | src MAC | tag    | type/len | payload   | CRC    |
//  | 6 byte  | 6 byte  | 4 byte | 2 byte   | 1500 byte | 4 byte |
//  +---------+---------+--------+----------+-----------+--------+
//
// And with 802.1ad "Q-in-Q", multiple Q-tags can be present, so there is no
// longer a max frame length. But for practical reasons, we can assume there
// is just one tag - and thus we don't see frames longer than 1522 byte byte.

#define ETHERNET_FRAME_MAX_SIZE     ( (2 * MAC_SIZE) + 4 + 2 + ETHERNET_MTU + 4 )
