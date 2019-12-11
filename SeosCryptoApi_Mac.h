/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Mac.h
 *
 * @brief SEOS Crypto API library types, constants and enums for MAC object
 *
 */

#pragma once

#include "SeosError.h"

#include <stddef.h>

typedef struct SeosCryptoMac SeosCryptoMac;
typedef SeosCryptoMac* SeosCryptoApi_Mac;

#define SeosCryptoApi_Mac_SIZE_HMAC_MD5     16
#define SeosCryptoApi_Mac_SIZE_HMAC_SHA256  32

typedef enum
{
    SeosCryptoApi_Mac_ALG_NONE = 0,
    SeosCryptoApi_Mac_ALG_HMAC_MD5,
    SeosCryptoApi_Mac_ALG_HMAC_SHA256,
}
SeosCryptoApi_Mac_Alg;

/**
 * @brief Initialize a message authentication code (MAC) object
 *
 * This function allocates a MAC object and initializes it.
 *
 * Currently supported algorithms are:
 * - HMAC_MD5
 * - HMAC_SHA256
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pMacHandle (required) pointer to MAC handle
 * @param algorithm (required) MAC algorithm to use
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the internal state could not be initialized
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the MAC object failed
 */
seos_err_t
SeosCryptoApi_Mac_init(
    SeosCryptoApi_Context*      ctx,
    SeosCryptoApi_Mac*          pMacHandle,
    const SeosCryptoApi_Mac_Alg algorithm);

/**
 * @brief Finish a message authentication code (MAC) object
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param macHandle (required) initialized MAC handle
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Mac_free(
    SeosCryptoApi_Context*  ctx,
    const SeosCryptoApi_Mac macHandle);

/**
 * @brief Feed secret into message authentication code (MAC) algorithm
 *
 * In order to be used as authentication code, a MAC algorithm needs to be given
 * a secret which can be used to generate the MAC (and also to re-generate and
 * verify it).
 *
 * This function has to be called once at the beginning of each MAC computation.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param macHandle (required) initialized MAC handle
 * @param secret (required) secret to process
 * @param secretSize (required) length of data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if processing of \p secret failed or if MAC was
 * was already started
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p secretSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Mac_start(
    SeosCryptoApi_Context*  ctx,
    const SeosCryptoApi_Mac macHandle,
    const void*             secret,
    const size_t            secretSize);

/**
 * @brief Feed block of data into message authentication code (MAC) algorithm
 *
 * Feed blocks of data into the MAC algorithm.
 *
 * This function can be called multiple times after start and before finalizing
 * the MAC.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param macHandle (required) initialized MAC handle
 * @param data (required) data to process
 * @param dataSize (required) length of data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if processing of \p data failed or if MAC object
 * was already finalized or not yet started
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p dataSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Mac_process(
    SeosCryptoApi_Context*  ctx,
    const SeosCryptoApi_Mac macHandle,
    const void*             data,
    const size_t            dataSize);

/**
 * @brief Finish message authentication code (MAC) computation to produce MAC
 *
 * Write the MAC resulting from any preceding calls to start and process into a
 * buffer.
 *
 * This function will reset the MAC object, so it can compute a new MAC.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param macHandle (required) initialized MAC handle
 * @param mac (required) buffer to write MAC to
 * @param macSize (required) size of MAC buffer, will be set to the amount
 * of bytes written to \p mac (or the minimum size if it fails due too small
 * buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the MAC could not be produced or if no
 * blocks were processed before finalizing or if finalize was already called
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p macSize is too small for the
 * resulting MAC
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p macSize is greater than
 * `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Mac_finalize(
    SeosCryptoApi_Context*  ctx,
    const SeosCryptoApi_Mac macHandle,
    void*                   mac,
    size_t*                 macSize);

/** @} */