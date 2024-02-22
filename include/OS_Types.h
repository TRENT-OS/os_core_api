/*
 * OS type definitions
 *
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#pragma once

#include <stddef.h>

typedef void (*event_wait_func_t)(void);
typedef void (*event_notify_func_t)(void);
typedef int (*mutex_lock_func_t)(void);
typedef int (*mutex_unlock_func_t)(void);

typedef struct
{
    void*   buffer;
    size_t  len;
} OS_SharedBuffer_t;
