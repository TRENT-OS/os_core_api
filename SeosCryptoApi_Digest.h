/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Digest.h
 *
 * @brief SEOS Crypto API library types, constants and enums for Digest object
 *
 */

#pragma once

#include "SeosError.h"

typedef struct SeosCryptoDigest SeosCryptoDigest;
typedef SeosCryptoDigest* SeosCryptoApi_Digest;

#define SeosCryptoApi_Digest_SIZE_MD5     16
#define SeosCryptoApi_Digest_SIZE_SHA256  32

/**
 * These need to be set to these exact values to match values expected by the
 * implementation of the Crypto API.
 */
typedef enum
{
    SeosCryptoApi_Digest_ALG_NONE       = 0,
    SeosCryptoApi_Digest_ALG_MD5        = 3,
    SeosCryptoApi_Digest_ALG_SHA256     = 6
}
SeosCryptoApi_Digest_Alg;

/**
 * @brief Initialize a digest object
 *
 * This function allocates a digest object and initializes it.
 *
 * Currently supported algorithms are:
 * - MD5
 * - SHA256
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pDigestHandle (required) pointer to digest handle
 * @param algorithm (required) digest algorithm to use
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the internal state could not be initialized
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the digest failed
 */
seos_err_t
SeosCryptoApi_Digest_init(
    SeosCryptoApi_Context*         ctx,
    SeosCryptoApi_Digest*          pDigestHandle,
    const SeosCryptoApi_Digest_Alg algorithm);

/**
 * @brief Finish a digest object
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param digestHandle (required) initialized digest handle
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Digest_free(
    SeosCryptoApi_Context*     ctx,
    const SeosCryptoApi_Digest digestHandle);

/**
 * @brief Clone a digest object
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param dstDigHandle (required) initialized handle used as target
 * @param srcDigHandle (required) initialized handle used as source
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Digest_clone(
    SeosCryptoApi_Context*     ctx,
    const SeosCryptoApi_Digest dstDigHandle,
    const SeosCryptoApi_Digest srcDigHandle);

/**
 * @brief Process block of data with digest algorithm
 *
 * Feed blocks of data into the internal state of the digest.
 *
 * This function can be called multiple times until the digest has been finalized.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param digestHandle (required) initialized digest handle
 * @param data (required) data to process
 * @param dataSize (required) length of data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if processing of \p data failed or if digest was
 * was already finalized
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p dataSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Digest_process(
    SeosCryptoApi_Context*     ctx,
    const SeosCryptoApi_Digest digestHandle,
    const void*                data,
    const size_t               dataSize);

/**
 * @brief Finish digest computation to produce digest
 *
 * Write the digest resulting from any preceding calls to process into a buffer.
 *
 * This function will re-set the digest object so it can process new blocks of
 * data for a new hash.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param digestHandle (required) initialized digest handle
 * @param digest (required) buffer to write digest to
 * @param digestSize (required) size of digest buffer, will be set to the amount
 * of bytes written to \p digest (or the minimum size if it fails due too small
 * buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the digest could not be produced or if no
 * blocks were processed before finalizing or if finalize was already called
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p digestSize is too small for the
 * resulting digest
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p digestSize is greater than
 * `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Digest_finalize(
    SeosCryptoApi_Context*     ctx,
    const SeosCryptoApi_Digest digestHandle,
    void*                      digest,
    size_t*                    digestSize);

/** @} */