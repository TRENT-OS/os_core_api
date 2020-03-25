/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup OS_Crypto OS Crypto API
 * @{
 *
 * @file OS_CryptoRng.h
 *
 * @brief OS Crypto API library types, constants and enums for RNG object
 *
 */

#pragma once

#include "SeosError.h"

#include <stddef.h>

/**
 * Special flags to use when generating random numbers.
 */
typedef enum
{
    OS_CryptoRng_FLAG_NONE = 0,
} OS_CryptoRng_Flag_t;

/**
 * Callback to be provided by user of Crypto API so the internal DRBG can be
 * enriched with entropy.
 */
typedef int (OS_CryptoRng_Entropy_func)(
    void* ctx, unsigned char* buf, size_t len);

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
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter is missing or invalid
 * @retval SEOS_ERROR_ABORTED if the internal RNG had a failure
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p flags are not supported by RNG
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p bufSize is greater than
 *  `OS_Crypto_SIZE_DATAPORT`
 */
seos_err_t
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
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter is missing or invalid
 * @retval SEOS_ERROR_ABORTED if the RNG had a failure
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p seedSize is greater than
 *  `OS_Crypto_SIZE_DATAPORT`
 */
seos_err_t
OS_CryptoRng_reseed(
    OS_Crypto_Handle_t hCrypto,
    const void*        seed,
    const size_t       seedSize);

/** @} */