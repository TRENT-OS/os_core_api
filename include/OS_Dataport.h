/*
 * OS dataport definitions
 *
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    void**  io;
    size_t  size;
    void*   (*get_p_func)(void);
    size_t  (*get_size_func)(void);
} OS_Dataport_t;

// Access the dataport
static __attribute__((unused)) void*
OS_Dataport_getBuf(
    const OS_Dataport_t dp)
{
    if (dp.get_p_func)
    {
        return dp.get_p_func();
    }
    return *(dp.io);
}
static __attribute__((unused)) size_t
OS_Dataport_getSize(
    const OS_Dataport_t dp)
{
    if (dp.get_size_func)
    {
        return dp.get_size_func();
    }
    return dp.size;
}
static __attribute__((unused)) bool
OS_Dataport_isUnset(
    const OS_Dataport_t dp)
{
    return (((dp.io == NULL) || (*(dp.io) == NULL)) && (dp.get_p_func == NULL));
}

// Assign the dataport
#define OS_DATAPORT_ASSIGN(p) {          \
    .io            = (void**)( &(p) ),   \
    .size          = sizeof( *(p) ),     \
    .get_p_func    = NULL,               \
    .get_size_func = NULL                \
}
// Macro to be used when the Dataport has a custom size and the sizeof operator
// fails to return the right value due to pointer decay.
#define OS_DATAPORT_ASSIGN_SIZE(p, _size_) { \
    .io            = (void**)(&(p)),         \
    .size          = (_size_),               \
    .get_p_func    = NULL,                   \
    .get_size_func = NULL                    \
}
// Macro to be used when the size and the pointer of the Dataport are returned
// by specific CAmkES functions.
#define OS_DATAPORT_ASSIGN_FUNC(_p_func_, _size_func_) { \
    .io            = NULL,                               \
    .size          = 0,                                  \
    .get_p_func    = _p_func_,                           \
    .get_size_func = _size_func_                         \
}

#define OS_DATAPORT_NONE {  \
    .io            = NULL,  \
    .size          = 0,     \
    .get_p_func    = NULL,  \
    .get_size_func = NULL   \
}

/*
 * Ideally, we would like to include <camkes/dataport.h> but it is not available
 * for non-CAmkES builds; so we derive the size in the same way it is done in the
 * actual dataport definition.
 *
 * NOTE: The following are copied from sel4_util_libs/libutils...
 */
#ifndef PAGE_SIZE_4K
#   define BIT(n) (1ul<<(n))
#   define SIZE_BITS_TO_BYTES(size_bits) (BIT(size_bits))
#   define PAGE_BITS_4K 12
#   define PAGE_SIZE_4K (SIZE_BITS_TO_BYTES(PAGE_BITS_4K))
#endif
#define OS_DATAPORT_DEFAULT_SIZE PAGE_SIZE_4K

// Fake dataport to be used on the host
typedef uint8_t FakeDataport_t[PAGE_SIZE_4K];
