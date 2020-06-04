/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup OS_Crypto OS Crypto API
 * @{
 *
 * @file OS_CryptoDigest.h
 *
 * @brief OS Crypto API library types, constants and enums for DIGEST object
 *
 */

#pragma once

#include "OS_Error.h"

/**
 * Length of MD5 hash in bytes.
 */
#define OS_CryptoDigest_SIZE_MD5     16
/**
 * Length of SHA256 hash in bytes.
 */
#define OS_CryptoDigest_SIZE_SHA256  32

/**
 * These need to be set to these exact values to match values expected by the
 * implementation of the Crypto API.
 */
typedef enum
{
    OS_CryptoDigest_ALG_NONE       = 0,
    /**
     * Use MD5 hash.
     */
    OS_CryptoDigest_ALG_MD5        = 3,
    /**
     * Use SHA256 hash.
     */
    OS_CryptoDigest_ALG_SHA256     = 6
} OS_CryptoDigest_Alg_t;

/**
 * Handle for OS Crypto API DIGEST objects.
 */
typedef OS_Crypto_Object_t* OS_CryptoDigest_Handle_t;

/**
 * @brief Initialize a DIGEST object.
 *
 * @param hDigest (required) pointer to handle of OS Crypto DIGEST object
 * @param hCrypto (required) handle of OS Crypto API
 * @param algorithm (required) DIGEST algorithm to use
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the internal state could not be initialized
 * @retval OS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of the DIGEST failed
 */
OS_Error_t
OS_CryptoDigest_init(
    OS_CryptoDigest_Handle_t*   hDigest,
    const OS_Crypto_Handle_t    hCrypto,
    const OS_CryptoDigest_Alg_t algorithm);

/**
 * @brief Initialize and clone a DIGEST object.
 *
 * This function initializes \p hDigest from the already existing DIGEST object
 * in \p hSrcDigest and and copies its entire internal state.
 *
 * @param hDigest (required) pointer to handle of OS Crypto DIGEST object
 * @param hCrypto (required) handle of OS Crypto API
 * @param hSrcDigest (required) handle of OS Crypto DIGEST object
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the internal state could not be initialized
 * @retval OS_ERROR_INVALID_HANDLE if \p hSrcDigest is not a valid handle
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of the DIGEST failed
 */
OS_Error_t
OS_CryptoDigest_clone(
    OS_CryptoDigest_Handle_t*      hDigest,
    const OS_Crypto_Handle_t       hCrypto,
    const OS_CryptoDigest_Handle_t hSrcDigest);

/**
 * @brief Finish a DIGEST object.
 *
 * @param hDigest (required) handle of OS Crypto DIGEST object
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_CryptoDigest_free(
    OS_CryptoDigest_Handle_t hDigest);

/**
 * @brief Process block of data.
 *
 * Feed blocks of data into the internal state of the DIGEST object. Typically,
 * this function will be called multiple times until the DIGEST has been finalized.
 *
 * @param hDigest (required) handle of OS Crypto DIGEST object
 * @param data (required) data to process
 * @param dataSize (required) length of data
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if processing of \p data failed or if DIGEST was
 *  was already finalized
 * @retval OS_ERROR_INSUFFICIENT_SPACE if \p dataSize is greater than
 *  `OS_Crypto_SIZE_DATAPORT`
 */
OS_Error_t
OS_CryptoDigest_process(
    OS_CryptoDigest_Handle_t hDigest,
    const void*              data,
    const size_t             dataSize);

/**
 * @brief Finish computation to produce digest/hash value.
 *
 * Write the digest/hash value resulting from any preceding calls to process into
 * a buffer.
 *
 * NOTE: This function will re-set the DIGEST object so it can be used to compute
 *       a new digest value with the algorithm given during initialization.
 *
 * @param hDigest (required) handle of OS Crypto DIGEST object
 * @param digest (required) buffer to write digest/hash value to
 * @param digestSize (required) size of buffer, will be set to the amount
 *  of bytes written to \p digest (or the minimum size if it fails due too small
 *  buffer)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the digest could not be produced or if no
 *  blocks were processed before finalizing or if finalize was already called
 * @retval OS_ERROR_BUFFER_TOO_SMALL if \p digestSize is too small for the
 *  resulting digest
 * @retval OS_ERROR_INSUFFICIENT_SPACE if \p digestSize is greater than
 *  `OS_Crypto_SIZE_DATAPORT`
 */
OS_Error_t
OS_CryptoDigest_finalize(
    OS_CryptoDigest_Handle_t hDigest,
    void*                    digest,
    size_t*                  digestSize);

/** @} */