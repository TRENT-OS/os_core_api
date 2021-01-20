/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 */

#pragma once

#define IF_OS_TIMER_CAMKES  \
    OS_Error_t              \
    completed(              \
        out uint32_t tmr);  \
    \
    OS_Error_t              \
    periodic(               \
        in int tid,         \
        in uint64_t ns);    \
    \
    OS_Error_t              \
    oneshot_absolute(       \
        in int tid,         \
        in uint64_t ns);    \
    \
    OS_Error_t              \
    oneshot_relative(       \
        in int tid,         \
        in uint64_t ns);    \
    \
    OS_Error_t              \
    stop(                   \
        in int tid);        \
    \
    OS_Error_t              \
    time(                   \
        out uint64_t ns);
