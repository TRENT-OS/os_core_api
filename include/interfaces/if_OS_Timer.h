/*
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "OS_Error.h"

#include <stdint.h>
#include <camkes.h>

typedef struct
{
    OS_Error_t (*completed)(uint32_t* tmr);
    OS_Error_t (*periodic)(int tid, uint64_t ns);
    OS_Error_t (*oneshot_absolute)(int tid, uint64_t ns);
    OS_Error_t (*oneshot_relative)(int tid, uint64_t ns);
    OS_Error_t (*stop)(int tid);
    OS_Error_t (*time)(uint64_t* ns);
    void (*notify_wait)(void);
} if_OS_Timer_t;

#define IF_OS_TIMER_ASSIGN(_rpc_, _evt_)                \
{                                                       \
    .completed          = _rpc_ ## _completed,          \
    .periodic           = _rpc_ ## _periodic,           \
    .oneshot_absolute   = _rpc_ ## _oneshot_absolute,   \
    .oneshot_relative   = _rpc_ ## _oneshot_relative,   \
    .stop               = _rpc_ ## _stop,               \
    .time               = _rpc_ ## _time,               \
    .notify_wait        = _evt_ ## _wait,               \
}
