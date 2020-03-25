/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup OS_Crypto OS Crypto API
 * @{
 *
 * @file OS_CryptoAgreement.h
 *
 * @brief OS Crypto API library types, constants and enums for AGREEMENT object
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
    OS_CryptoAgreement_ALG_NONE = 0,
    /**
     * Use Diffie-Hellman(-Merkle) key exchange.
     */
    OS_CryptoAgreement_ALG_DH,
    /**
     * Use Elliptic Curve Diffie-Hellman(-Merkle) key exchange.
     */
    OS_CryptoAgreement_ALG_ECDH
} OS_CryptoAgreement_Alg_t;

/**
 * Handle for OS Crypto API (key) AGREEMENT objects.
 */
typedef OS_Crypto_Object_t* OS_CryptoAgreement_Handle_t;

/**
 * @brief Initialize an AGREEMENT object.
 *
 * This function allocates a key AGREEMENT object and initializes it. A private
 * key needs to be given, which will be used during the agreement phase to derive
 * a shared secret.
 *
 * @param hAgree (required) pointer to handle of OS Crypto AGREEMENT object
 * @param hCrypto (required) handle of OS Crypto API
 * @param hPrvKey (required) handle of OS Crypto Key object to use as our own
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
OS_CryptoAgreement_init(
    OS_CryptoAgreement_Handle_t*   hAgree,
    const OS_Crypto_Handle_t       hCrypto,
    const OS_CryptoKey_Handle_t    hPrvKey,
    const OS_CryptoAgreement_Alg_t algorithm);

/**
 * @brief Finish an AGREEMENT object.
 *
 * @param hAgree (required) handle of OS Crypto AGREEMENT object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
OS_CryptoAgreement_free(
    OS_CryptoAgreement_Handle_t hAgree);

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
 * @param hAgree (required) handle of OS Crypto AGREEMENT object
 * @param hPubKey (required) handle of OS Crypto Key object to use as the other
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
 *  `OS_Crypto_SIZE_DATAPORT`
 */
seos_err_t
OS_CryptoAgreement_agree(
    OS_CryptoAgreement_Handle_t hAgree,
    const OS_CryptoKey_Handle_t hPubKey,
    void*                       shared,
    size_t*                     sharedSize);

/** @} */