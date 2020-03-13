/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Signature.h
 *
 * @brief SEOS Crypto API library types, constants and enums for Signature object
 *
 */

#pragma once

#include "SeosError.h"

#include <stddef.h>

typedef enum
{
    SeosCryptoApi_Signature_ALG_NONE = 0,
    SeosCryptoApi_Signature_ALG_RSA_PKCS1_V15,
    SeosCryptoApi_Signature_ALG_RSA_PKCS1_V21
}
SeosCryptoApi_Signature_Alg;

typedef SeosCryptoApi_Proxy* SeosCryptoApi_SignatureH;

/**
 * @brief Initialize a signature object
 *
 * This function allocates a signature object and initializes it. It can hold a
 * private and public key (or both) and based on what is provided during
 * initialization the respective private operation (signing) or public operation
 * (verification) are possible.
 *
 * Currently supported algorithms are:
 * - RSA according to PKCS#1 v1.5
 * - RSA according to PKCS#1 v2.1
 *
 * @param hSig (required) pointer to handle of SEOS Crypto Signature object
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param hPrvKey (optional) handle of SEOS Crypto Key object to use as private key
 * @param hPubKey (optional) handle of SEOS Crypto Key object to use as public key
 * @param sigAlgorithm (required) signature algorithm to use
 * @param digAlgorithm (required) digest algorithm used for hashes processed with
 * this signature object (important for correct padding)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid, this
 * includes passing the wrong types of key
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the digest failed
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 */
seos_err_t
SeosCryptoApi_Signature_init(
    SeosCryptoApi_SignatureH*         hSig,
    const SeosCryptoApiH              hCrypto,
    const SeosCryptoApi_KeyH          hPrvKey,
    const SeosCryptoApi_KeyH          hPubKey,
    const SeosCryptoApi_Signature_Alg sigAlgorithm,
    const SeosCryptoApi_Digest_Alg    digAlgorithm);

/**
 * @brief Finish a signature object
 *
 * @param hSig (required) handle of SEOS Crypto Signature object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Signature_free(
    SeosCryptoApi_SignatureH hSig);

/**
 * @brief Sign a hash value
 *
 * Sign a digest value (typically 16-32 bytes) with the private key of the
 * signature object; for this the \p prvHandle param must be set during signature
 * initialization.
 *
 * @param hSig (required) handle of SEOS Crypto Signature object
 * @param hash (required) hash value to sign
 * @param hashSize (required) size of hash
 * @param signature (required) buffer for resulting signature
 * @param signatureSize (required) size of signature buffer, will be set to
 * actually written amount of bytes if function succeeds (or the minimum size
 * if it fails due to too small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if no private key was set during initialization or
 * if the cryptographic operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p signatureSize is too small to hold
 * the resulting \p signature
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p hashSize or \p signatureSize is
 * greater than `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Signature_sign(
    SeosCryptoApi_SignatureH hSig,
    const void*              hash,
    const size_t             hashSize,
    void*                    signature,
    size_t*                  signatureSize);

/**
 * @brief Verify signature over a hash
 *
 * Verify a signature for a given value, which is usually a message digest of
 * fixed size (16-32 bytes). For this operation to work, the  \p pubHandle
 * param must be set during signature initialization.
 *
 * @param hSig (required) handle of SEOS Crypto Signature object
 * @param hash (required) hash value to recompute signature for
 * @param hashSize (required) size of hash
 * @param signature (required) buffer for signature to verify
 * @param signatureSize (required) size of signature in buffer
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes passing a \p signatureSize that is unexpected (due to the key)
 * @retval SEOS_ERROR_ABORTED if no private key was set during initialization or
 * if the cryptographic operation failed (i.e., the signature was invalid)
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p signatureSize + \p hashSize is
 * greater than `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Signature_verify(
    SeosCryptoApi_SignatureH hSig,
    const void*              hash,
    const size_t             hashSize,
    const void*              signature,
    const size_t             signatureSize);

/** @} */
