/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup OS_Crypto OS Crypto API
 * @{
 *
 * @file OS_CryptoMac.h
 *
 * @brief OS Crypto API library types, constants and enums for MAC object
 *
 */

#pragma once

#include "OS_Error.h"

#include <stddef.h>

/**
 * The output size of HMAC-MD5 in bytes.
 */
#define OS_CryptoMac_SIZE_HMAC_MD5     16
/**
 * The output size of HMAC-SHA256 in bytes.
 */
#define OS_CryptoMac_SIZE_HMAC_SHA256  32

/**
 * Type of MAC algorithm to use.
 */
typedef enum
{
    OS_CryptoMac_ALG_NONE = 0,
    /**
     * Use HMAC with MD5 as hash algorithm.
     */
    OS_CryptoMac_ALG_HMAC_MD5,
    /**
     * Use HMAC with SHA256 as hash algorithm.
     */
    OS_CryptoMac_ALG_HMAC_SHA256,
} OS_CryptoMac_Alg_t;

/**
 * Handle for OS Crypto API MAC objects.
 */
typedef OS_Crypto_Object_t* OS_CryptoMac_Handle_t;

/**
 * @brief Initialize a message authentication code (MAC) object.
 *
 * @param hMac (required) pointer to handle of OS Crypto MAC object
 * @param hCrypto (required) handle of OS Crypto API
 * @param algorithm (required) MAC algorithm to use
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the internal state could not be initialized
 * @retval OS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of the MAC object failed
 */
OS_Error_t
OS_CryptoMac_init(
    OS_CryptoMac_Handle_t*   hMac,
    const OS_Crypto_Handle_t hCrypto,
    const OS_CryptoMac_Alg_t algorithm);

/**
 * @brief Finish a message authentication code (MAC) object
 *
 * @param hMac (required) handle of OS Crypto MAC object
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_CryptoMac_free(
    OS_CryptoMac_Handle_t hMac);

/**
 * @brief Start MAC computation by feeding a secret into the MAC's state.
 *
 * In order to be used as authentication code, a MAC algorithm needs to be given
 * a secret which can be used to generate the MAC (and also to re-generate and
 * verify it).
 *
 * This function has to be called once at the beginning of each MAC computation.
 *
 * @param hMac (required) handle of OS Crypto MAC object
 * @param secret (required) secret to process
 * @param secretSize (required) length of data
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if processing of \p secret failed or if MAC was
 *  was already started
 * @retval OS_ERROR_INSUFFICIENT_SPACE if \p secretSize is greater than
 *   `OS_Crypto_SIZE_DATAPORT`
 */
OS_Error_t
OS_CryptoMac_start(
    OS_CryptoMac_Handle_t hMac,
    const void*           secret,
    const size_t          secretSize);

/**
 * @brief Feed block of data into MACs internal state.
 *
 * Feed blocks of data into the MAC algorithm. Typically, this function will be
 * called multiple times after start and before finalizing the MAC.
 *
 * @param hMac (required) handle of OS Crypto MAC object
 * @param data (required) data to process
 * @param dataSize (required) length of data
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if processing of \p data failed or if MAC object
 *  was already finalized or not yet started
 * @retval OS_ERROR_INSUFFICIENT_SPACE if \p dataSize is greater than
 *   `OS_Crypto_SIZE_DATAPORT`
 */
OS_Error_t
OS_CryptoMac_process(
    OS_CryptoMac_Handle_t hMac,
    const void*           data,
    const size_t          dataSize);

/**
 * @brief Finish MAC computation to produce authentication code.
 *
 * Write the MAC resulting from any preceding calls to start and process into a
 * buffer.
 *
 * NOTE: This function will re-set the MAC object, so it can compute a new MAC
 *       with the algorithm given during initialization.
 *
 * @param hMac (required) handle of OS Crypto MAC object
 * @param auth (required) buffer to write authenticaion code to
 * @param authSize (required) size of buffer, will be set to the amount of bytes
 *  written to \p auth (or the minimum size if it fails due too small buffer)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if \p auth could not be produced or if no
 *  blocks were processed before finalizing or if finalize was already called
 * @retval OS_ERROR_BUFFER_TOO_SMALL if \p authSize is too small for the
 *  resulting MAC
 * @retval OS_ERROR_INSUFFICIENT_SPACE if \p authSize is greater than
 *  `OS_Crypto_SIZE_DATAPORT`
 */
OS_Error_t
OS_CryptoMac_finalize(
    OS_CryptoMac_Handle_t hMac,
    void*                 auth,
    size_t*               authSize);

/** @} */