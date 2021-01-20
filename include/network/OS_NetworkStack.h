/*
 * OS Network Stack interface to driver
 *
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

// This struct is currently used to synchronize between network driver and the
// network stack. When the len is set to 0 it means the frame stored in data
// has been processed and the buffer can be reused. If len is non-zero it
// indicates the length of the data stored in the buffer, which will be used
// by the network stack.
//

typedef struct
{
    uint8_t         data[PAGE_SIZE - 16];
    volatile size_t len;
} OS_NetworkStack_RxBuffer_t;
