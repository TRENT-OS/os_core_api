/*
 *  SEOS return codes
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#define SEOS_ERROR_MIN -13

typedef enum
{
    SEOS_ERROR_CONNECTION_CLOSED = SEOS_ERROR_MIN,
    SEOS_ERROR_OVERFLOW_DETECTED,
    SEOS_ERROR_INSUFFICIENT_SPACE,
    SEOS_ERROR_BUFFER_TOO_SMALL,
    SEOS_ERROR_ABORTED,
    SEOS_ERROR_OPERATION_DENIED,
    SEOS_ERROR_ACCESS_DENIED,
    SEOS_ERROR_NOT_FOUND,
    SEOS_ERROR_INVALID_HANDLE,
    SEOS_ERROR_INVALID_NAME,
    SEOS_ERROR_INVALID_PARAMETER,
    SEOS_ERROR_NOT_SUPPORTED,
    SEOS_ERROR_GENERIC,
    SEOS_SUCCESS
}
seos_err_t;

static_assert(SEOS_SUCCESS == 0, "SEOS_SUCCESS_MUST BE 0");

#ifdef __cplusplus
}
#endif
