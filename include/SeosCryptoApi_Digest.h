/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Digest.h
 *
 * @brief SEOS Crypto API library types, constants and enums for DIGEST object
 *
 */

#pragma once

#include "SeosError.h"

/**
 * Length of MD5 hash in bytes.
 */
#define SeosCryptoApi_Digest_SIZE_MD5     16
/**
 * Length of SHA256 hash in bytes.
 */
#define SeosCryptoApi_Digest_SIZE_SHA256  32

/**
 * These need to be set to these exact values to match values expected by the
 * implementation of the Crypto API.
 */
typedef enum
{
    SeosCryptoApi_Digest_ALG_NONE       = 0,
    /**
     * Use MD5 hash.
     */
    SeosCryptoApi_Digest_ALG_MD5        = 3,
    /**
     * Use SHA256 hash.
     */
    SeosCryptoApi_Digest_ALG_SHA256     = 6
}
SeosCryptoApi_Digest_Alg;

/**
 * Handle for SEOS Crypto API DIGEST objects.
 */
typedef SeosCryptoApi_Proxy* SeosCryptoApi_DigestH;

/**
 * @brief Initialize a DIGEST object.
 *
 * @param hDigest (required) pointer to handle of SEOS Crypto DIGEST object
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param algorithm (required) DIGEST algorithm to use
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the internal state could not be initialized
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the DIGEST failed
 */
seos_err_t
SeosCryptoApi_Digest_init(
    SeosCryptoApi_DigestH*         hDigest,
    const SeosCryptoApiH           hCrypto,
    const SeosCryptoApi_Digest_Alg algorithm);

/**
 * @brief Finish a DIGEST object.
 *
 * @param hDigest (required) handle of SEOS Crypto DIGEST object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Digest_free(
    SeosCryptoApi_DigestH hDigest);

/**
 * @brief Clone a DIGEST object.
 *
 * This function requires two initialized DIGEST objects; the internal state of
 * \p hDstDigest will be set to the internal state of \p hSrcDigest.
 *
 * @param hDstDigest (required) handle of SEOS Crypto DIGEST object
 * @param hSrcDigest (required) handle of SEOS Crypto DIGEST object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Digest_clone(
    SeosCryptoApi_DigestH       hDstDigest,
    const SeosCryptoApi_DigestH hSrcDigest);

/**
 * @brief Process block of data.
 *
 * Feed blocks of data into the internal state of the DIGEST object. Typically,
 * this function will be called multiple times until the DIGEST has been finalized.
 *
 * @param hDigest (required) handle of SEOS Crypto DIGEST object
 * @param data (required) data to process
 * @param dataSize (required) length of data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if processing of \p data failed or if DIGEST was
 *  was already finalized
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p dataSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Digest_process(
    SeosCryptoApi_DigestH hDigest,
    const void*           data,
    const size_t          dataSize);

/**
 * @brief Finish computation to produce digest/hash value.
 *
 * Write the digest/hash value resulting from any preceding calls to process into
 * a buffer.
 *
 * NOTE: This function will re-set the DIGEST object so it can be used to compute
 *       a new digest value with the algorithm given during initialization.
 *
 * @param hDigest (required) handle of SEOS Crypto DIGEST object
 * @param digest (required) buffer to write digest/hash value to
 * @param digestSize (required) size of buffer, will be set to the amount
 *  of bytes written to \p digest (or the minimum size if it fails due too small
 *  buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the digest could not be produced or if no
 *  blocks were processed before finalizing or if finalize was already called
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p digestSize is too small for the
 *  resulting digest
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p digestSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Digest_finalize(
    SeosCryptoApi_DigestH hDigest,
    void*                 digest,
    size_t*               digestSize);

/** @} */