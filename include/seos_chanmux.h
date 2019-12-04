/*
 *  SEOS Chanmux helper definitions
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "seos_types.h"
#include <stdint.h>


typedef struct
{
    unsigned int          id;
    seos_shared_buffer_t  port;
} ChanMux_channelCtx_t;


typedef struct
{
    unsigned int          id;
    seos_shared_buffer_t  port_read;
    seos_shared_buffer_t  port_write;
} ChanMux_channelDuplexCtx_t;
