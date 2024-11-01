/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @file
 * @ingroup OS_CryptoSignature
 */

/**
 * @defgroup OS_CryptoSignature Crypto API library signature functionality
 * @{
 * @ingroup OS_Crypto
 * @brief OS Crypto API library signature functionality
 */

#pragma once

#include "OS_Error.h"

#include <stddef.h>

/**
 * Type of SIGNATURE algorithm to use.
 */
typedef enum
{
    OS_CryptoSignature_ALG_NONE = 0,

    /**
     * Use RSA with PKCS#1 V1.5 padding.
     */
    OS_CryptoSignature_ALG_RSA_PKCS1_V15,

    /**
     * Use RSA with PKCS#1 V2.1 padding; resulting signatures are probabilistic,
     * e.g. the value-to-be-signed includes some randomness.
     */
    OS_CryptoSignature_ALG_RSA_PKCS1_V21
} OS_CryptoSignature_Alg_t;

/// @cond INTERNAL
//------------------------------------------------------------------------------
typedef OS_Crypto_Object_t* OS_CryptoSignature_Handle_t;
//------------------------------------------------------------------------------
/// @endcond

/**
 * @brief Initialize a SIGNATURE object
 *
 * This function allocates a SIGNATURE object and initializes it. It can hold a
 * private and public key (or both).
 *
 * Based on what is provided during initialization the respective private operation
 * (signing) or public operation (verification) are possible.
 *
 * @param hSig (required) pointer to handle of OS Crypto SIGNATURE object
 * @param hCrypto (required) handle of OS Crypto API
 * @param hPrvKey (optional) handle of OS Crypto Key object to use as private key
 * @param hPubKey (optional) handle of OS Crypto Key object to use as public key
 * @param sigAlgorithm (required) signature algorithm to use
 * @param digAlgorithm (required) digest algorithm used for hashes processed with
 *  this SIGNATURE object (important for correct padding)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid, this
 *  includes passing the wrong types of key, an oversized or too small buffer
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of the digest failed
 * @retval OS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 */
OS_Error_t
OS_CryptoSignature_init(
    OS_CryptoSignature_Handle_t*   hSig,
    const OS_Crypto_Handle_t       hCrypto,
    const OS_CryptoKey_Handle_t    hPrvKey,
    const OS_CryptoKey_Handle_t    hPubKey,
    const OS_CryptoSignature_Alg_t sigAlgorithm,
    const OS_CryptoDigest_Alg_t    digAlgorithm);

/**
 * @brief Finish a SIGNATURE object.
 *
 * @param hSig (required) handle of OS Crypto SIGNATURE object
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_CryptoSignature_free(
    OS_CryptoSignature_Handle_t hSig);

/**
 * @brief Sign a hash value.
 *
 * Sign a hash/digest value (typically 16-32 bytes) with the private key of the
 * SIGNATURE object; for this the \p hPrvKey param must be set during SIGNATURE
 * initialization.
 *
 * @param hSig (required) handle of OS Crypto SIGNATURE object
 * @param hash (required) hash value to sign
 * @param hashSize (required) size of hash
 * @param signature (required) buffer for resulting signature
 * @param signatureSize (required) size of signature buffer, will be set to
 *  actually written amount of bytes if function succeeds (or the minimum size
 *  if it fails due to too small buffer)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes passing an oversized or too small buffer
 * @retval OS_ERROR_ABORTED if no private key was set during initialization or
 *  if the cryptographic operation failed
 */
OS_Error_t
OS_CryptoSignature_sign(
    OS_CryptoSignature_Handle_t hSig,
    const void*                 hash,
    const size_t                hashSize,
    void*                       signature,
    size_t*                     signatureSize);

/**
 * @brief Verify signature over a hash.
 *
 * Verify a signature for a given value, which is usually a message digest/hash of
 * fixed size (16-32 bytes). For this operation to work, the \p hPubKey param
 * must be set during SIGNATURE initialization.
 *
 * @param hSig (required) handle of OS Crypto SIGNATURE object
 * @param hash (required) hash value to recompute signature for
 * @param hashSize (required) size of hash
 * @param signature (required) buffer for signature to verify
 * @param signatureSize (required) size of signature in buffer
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes passing an oversized or too small buffer
 * @retval OS_ERROR_ABORTED if no private key was set during initialization or
 *  if the cryptographic operation failed (i.e., the signature was invalid)
 */
OS_Error_t
OS_CryptoSignature_verify(
    OS_CryptoSignature_Handle_t hSig,
    const void*                 hash,
    const size_t                hashSize,
    const void*                 signature,
    const size_t                signatureSize);

/** @} */