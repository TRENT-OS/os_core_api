/*
 *  OS type definitions
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
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
