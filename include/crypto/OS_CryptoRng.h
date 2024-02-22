/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @file
 * @ingroup OS_CryptoRng
 */

/**
 * @defgroup    OS_CryptoRng Crypto API library RNG functionality
 *
 * @{
 * @ingroup OS_Crypto
 * @brief OS Crypto API library Random Number Generator functionality
 */

#pragma once

#include "OS_Error.h"

#include <stddef.h>

/**
 * Special flags to use when generating random numbers.
 */
typedef enum
{
    OS_CryptoRng_FLAG_NONE = 0,
} OS_CryptoRng_Flag_t;

/**
 * @brief Extract random numbers from internal RNG.
 *
 * The internal RNG is based on CTR_DRBG using AES. It is fed from an entropy
 * source that has to be provided at the initialization of the Crypto API.
 *
 * Whenever this function is called, it will internally try to add more entropy
 * from the entropy source into the RNG state to enhance prediction resistance.
 * This behavior can be modified by passing respective \p flags.
 *
 * @param hCrypto (required) handle of OS Crypto API
 * @param flags (optional) flags for RNG operation
 * @param buf (required) buffer for random bytes
 * @param bufSize (required) amount of random bytes requested
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes passing an oversized or too small buffer
 * @retval OS_ERROR_ABORTED if the internal RNG had a failure
 * @retval OS_ERROR_NOT_SUPPORTED if \p flags are not supported by RNG
 */
OS_Error_t
OS_CryptoRng_getBytes(
    OS_Crypto_Handle_t        hCrypto,
    const OS_CryptoRng_Flag_t flags,
    void*                     buf,
    const size_t              bufSize);

/**
 * @brief Reseed the internal RNG.
 *
 * Feed an arbitrary string of bytes into the DRBG's internal state at any time
 * via this function. This can be used, for instance, to add a device-specific
 * seed to the RNG's state.
 *
 * @param hCrypto (required) handle of OS Crypto API
 * @param seed (required) additional seed to feed into RNG state
 * @param seedSize (required) length of seed data
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes passing an oversized or too small buffer
 * @retval OS_ERROR_ABORTED if the RNG had a failure
 */
OS_Error_t
OS_CryptoRng_reseed(
    OS_Crypto_Handle_t hCrypto,
    const void*        seed,
    const size_t       seedSize);

/** @} */