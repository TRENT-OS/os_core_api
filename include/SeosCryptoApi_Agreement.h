/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Agreement.h
 *
 * @brief SEOS Crypto API library types, constants and enums for AGREEMENT object
 *
 */

#pragma once

#include "SeosError.h"

#include <stddef.h>

/**
 * Type of AGREEMENT algorithm to use.
 */
typedef enum
{
    SeosCryptoApi_Agreement_ALG_NONE = 0,
    /**
     * Use Diffie-Hellman(-Merkle) key exchange.
     */
    SeosCryptoApi_Agreement_ALG_DH,
    /**
     * Use Elliptic Curve Diffie-Hellman(-Merkle) key exchange.
     */
    SeosCryptoApi_Agreement_ALG_ECDH
}
SeosCryptoApi_Agreement_Alg;

/**
 * Handle for SEOS Crypto API (key) AGREEMENT objects.
 */
typedef SeosCryptoApi_Proxy* SeosCryptoApi_AgreementH;

/**
 * @brief Initialize an AGREEMENT object.
 *
 * This function allocates a key AGREEMENT object and initializes it. A private
 * key needs to be given, which will be used during the agreement phase to derive
 * a shared secret.
 *
 * @param hAgree (required) pointer to handle of SEOS Crypto AGREEMENT object
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param hPrvKey (required) handle of SEOS Crypto Key object to use as our own
 *  private key
 * @param algorithm (required) key agreement algorithm to use
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes provding the wrong type of key (e.g. AES key for DH algorithm)
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the underlying
 *  object failed
 */
seos_err_t
SeosCryptoApi_Agreement_init(
    SeosCryptoApi_AgreementH*         hAgree,
    const SeosCryptoApiH              hCrypto,
    const SeosCryptoApi_KeyH          hPrvKey,
    const SeosCryptoApi_Agreement_Alg algorithm);

/**
 * @brief Finish an AGREEMENT object.
 *
 * @param hAgree (required) handle of SEOS Crypto AGREEMENT object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Agreement_free(
    SeosCryptoApi_AgreementH hAgree);

/**
 * @brief Agree on a shared value.
 *
 * Computes the underlying key agreement based on the private key assosciated with
 * the object and a public key given here.
 *
 * The resulting shared secret is a bignum specific to the underlying algorithm,
 * e.g. in the case of DH \p shared represents an integer smaller than the prime
 * chosen for DH. A final processing step should be applied to the agreed key in
 * order to produce a symmetric key of suitable size.
 *
 * @param hAgree (required) handle of SEOS Crypto AGREEMENT object
 * @param hPubKey (required) handle of SEOS Crypto Key object to use as the other
 *  side's public key
 * @param shared (required) buffer to hold shared secret
 * @param sharedSize (required) size of buffer, will be set to actual amount of
 *  bytes written if function succeeds (or the minimum size if it fails due to too
 *  small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes passing the wrong type of key
 * @retval SEOS_ERROR_ABORTED if the underlying AGREEMENT operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p sharedSize is too small to hold
 *  the full result in the \p shared buffer
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p sharedSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Agreement_agree(
    SeosCryptoApi_AgreementH hAgree,
    const SeosCryptoApi_KeyH hPubKey,
    void*                    shared,
    size_t*                  sharedSize);

/** @} */