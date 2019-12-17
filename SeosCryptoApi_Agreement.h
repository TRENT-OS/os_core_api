/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Agreement.h
 *
 * @brief SEOS Crypto API library types, constants and enums for Agreement object
 *
 */

#pragma once

#include "SeosError.h"

#include <stddef.h>

typedef enum
{
    SeosCryptoApi_Agreement_ALG_NONE = 0,
    SeosCryptoApi_Agreement_ALG_DH,
    SeosCryptoApi_Agreement_ALG_ECDH
}
SeosCryptoApi_Agreement_Alg;

typedef struct SeosCryptoLib_Agreement SeosCryptoLib_Agreement;
typedef struct SeosCryptoApi_Context SeosCryptoApi_Context;
typedef struct
{
    SeosCryptoLib_Agreement* agreement;
    SeosCryptoApi_Context* api;
} SeosCryptoApi_Agreement;

/**
 * @brief Initialize an agreement object
 *
 * This function allocates a key agreement object and initializes it. A private
 * key needs to be given, which will be used during the agreement phase to derive
 * a shared secret.
 *
 * Currently supported algorithms are:
 * - DH
 * - ECDH
 *
 * @param api (required) pointer to the seos crypto context
 * @param obj (required) pointer to Agreement object
 * @param algorithm (required) key agreement algorithm to use
 * @param prvKey (required) handle of private key to use for key agreement
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes provding the wrong type of key (e.g. AES key for DH algorithm)
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the agreement
 * object failed
 */
seos_err_t
SeosCryptoApi_Agreement_init(
    SeosCryptoApi_Context*            api,
    SeosCryptoApi_Agreement*          obj,
    const SeosCryptoApi_Agreement_Alg algorithm,
    const SeosCryptoApi_Key*          prvKey);

/**
 * @brief Finish a agreement object
 *
 * @param obj (required) pointer to the Agreement object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Agreement_free(
    SeosCryptoApi_Agreement* obj);

/**
 * @brief Agree on a shared value
 *
 * Computes the underlying key agreement based on the private key assosciated with
 * the object and a public key given here.
 *
 * The resulting shared secret is a bignum specific to the underlying algorithm,
 * e.g. in the case of DH \p shared represents an integer smaller than the prime
 * chosen for DH. A final processing step should be applied to the agreed key in
 * order to produce a symmetric key of suitable size.
 *
 * @param obj (required) pointer to the Agreement object
 * @param pubKey (required) public key to use for key agreement
 * @param shared (required) buffer to hold shared secret
 * @param sharedSize (required) size of buffer, will be set to actual amount of
 * bytes written if function succeeds (or the minimum size if it fails due to too
 * small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes passing the wrong type of key
 * @retval SEOS_ERROR_ABORTED if the underlying agreement operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p sharedSize is too small to hold
 * the full result in the \p shared buffer
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p sharedSize is greater than
 * `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Agreement_agree(
    SeosCryptoApi_Agreement* obj,
    const SeosCryptoApi_Key* pubKey,
    void*                    shared,
    size_t*                  sharedSize);

/** @} */
