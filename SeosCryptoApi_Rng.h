/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Rng.h
 *
 * @brief SEOS Crypto API library types, constants and enums for RNG object
 *
 */

#pragma once

#include "SeosError.h"

#include <stddef.h>

typedef enum
{
    SeosCryptoApi_Rng_FLAG_NONE = 0,
} SeosCryptoApi_Rng_Flag;

typedef int (SeosCryptoApi_Rng_EntropyFunc)(
    void* ctx, unsigned char* buf, size_t len);

/**
 * @brief Generate random numbers
 *
 * The internal RNG is based on CTR_DRBG using AES. It is fed from an entropy
 * source that has to be provided at the initialization of the crypto API.
 *
 * Whenever this function is called, it will internally try to add more entropy
 * from the entropy source into the RNG state to enhance prediction resistance.
 * This behavior can be modified by passing respective \p flags.
 *
 * @param api (required) pointer to the seos crypto context
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
 * `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Rng_getBytes(
    SeosCryptoApi*               api,
    const SeosCryptoApi_Rng_Flag flags,
    void*                        buf,
    const size_t                 bufSize);

/**
 * @brief Reseed the internal RNG
 *
 * @param api (required) pointer to the seos crypto context
 * @param seed (required) additional seed to feed into RNG state
 * @param seedSize (required) length of seed data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter is missing or invalid
 * @retval SEOS_ERROR_ABORTED if the RNG had a failure
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p seedSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Rng_reseed(
    SeosCryptoApi* api,
    const void*    seed,
    const size_t   seedSize);

/** @} */