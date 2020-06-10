/*
 *  OS Network Stack interface to driver
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <limits.h>


typedef struct
{
    uint8_t  data[PAGE_SIZE - 16];
    size_t   len;
} OS_NetworkStack_RxBuffer_t;
