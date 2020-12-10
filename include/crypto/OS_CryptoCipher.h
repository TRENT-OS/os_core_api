/* Copyright (C) 2019-2020, Hensoldt Cyber GmbH */

/**
 * @file
 * @ingroup OS_CryptoCipher
 */

/**
 * @defgroup OS_CryptoCipher
 * @{
 * @ingroup OS_Crypto
 * @brief OS Crypto API library symmetric cipher functionality
 */

#pragma once

#include "OS_Error.h"

#include <stddef.h>

/**
 * Sizes relevant for the respective operation modes.
 */
#define OS_CryptoCipher_SIZE_AES_BLOCK         16
#define OS_CryptoCipher_SIZE_AES_CBC_IV        16
#define OS_CryptoCipher_SIZE_AES_GCM_IV        12
#define OS_CryptoCipher_SIZE_AES_GCM_TAG_MIN   4
#define OS_CryptoCipher_SIZE_AES_GCM_TAG_MAX   OS_CryptoCipher_SIZE_AES_BLOCK

/**
 * Type and mode of encryption algorithm to use for CIPHER object.
 */
typedef enum
{
    OS_CryptoCipher_ALG_NONE = 0,

    /**
     * Use AES in ECB mode for encryption.
     */

    OS_CryptoCipher_ALG_AES_ECB_ENC,

    /**
     * Use AES in ECB mode for decryption.
     */
    OS_CryptoCipher_ALG_AES_ECB_DEC,

    /**
     * Use AES in CBC mode for encryption.
     */
    OS_CryptoCipher_ALG_AES_CBC_ENC,

    /**
     * Use AES in CBC mode for decryption.
     */
    OS_CryptoCipher_ALG_AES_CBC_DEC,

    /**
     * Use AES in GCM mode for encryption.
     */
    OS_CryptoCipher_ALG_AES_GCM_ENC,

    /**
     * Use AES in GCM mode for decryption.
     */
    OS_CryptoCipher_ALG_AES_GCM_DEC,
} OS_CryptoCipher_Alg_t;

///@cond INTERNAL --------------------------------------------------------------
typedef OS_Crypto_Object_t* OS_CryptoCipher_Handle_t;
///@endcond --------------------------------------------------------------------

/**
 * @brief Initialize a CIPHER object.
 *
 * This function allocates a CIPHER object and initializes its internal state.
 *
 * Some algorithms do require an IV:
 * - AES-GCM requires 12 bytes of IV
 * - AES-CBC requires 16 bytes of IV
 *
 * @param hCipher (required) pointer to handle of OS Crypto CIPHER object
 * @param hCrypto (required) handle of OS Crypto API
 * @param hKey (required) handle of OS Crypto Key object
 * @param algorithm (required) algorithm to use
 * @param iv (optional) initialization vector required for some ciphers
 * @param ivSize (optional) length of initialization vector
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes if an iv is given for an algorithm that does not require an IV
 *  or if \p iv is NOT set for an algorithm that does require an IV; also includes
 *  mismatching IV sizes or passing a key that is not matching the algorithm or
 *  an oversized buffer
 * @retval OS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval OS_ERROR_ABORTED if setting the key internally failed
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of the CIPHER failed
 */
OS_Error_t
OS_CryptoCipher_init(
    OS_CryptoCipher_Handle_t*   hCipher,
    const OS_Crypto_Handle_t    hCrypto,
    const OS_CryptoKey_Handle_t hKey,
    const OS_CryptoCipher_Alg_t algorithm,
    const void*                 iv,
    const size_t                ivSize);

/**
 * @brief Finish CIPHER object.
 *
 * @param hCipher (required) handle of OS Crypto CIPHER object
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_CryptoCipher_free(
    OS_CryptoCipher_Handle_t hCipher);

/**
 * @brief Process data blocks with the CIPHER object.
 *
 * Process blocks of data with the selected algorithm. If the CIPHER is operated
 * in ENC mode, \p input will hold the plaintext and \p output will be the ciphertext.
 * If operated in DEC mode, it will be vice versa.
 *
 * NOTE: Special attention needs to be paid to the alignment of inputs:
 * - AES-ECB and AES-CBC require all inputs to be aligned to 16 byte blocks.
 * - AES-GCM can deal with non-aligned blocks, but only in the last call to
 *           this function.
 *
 * @param hCipher (required) handle of OS Crypto CIPHER object
 * @param input (required) input data
 * @param inputSize (required) length of input data
 * @param output (required) buffer for resulting output data
 * @param outputSize (required) size of output buffer, will be set to actual
 *  amount of bytes written if function succeeds (or to the minimum size if it
 *  fails)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes passing \p inputSize that is not aligned with the underlying
 *  blocksize or an oversized buffer
 * @retval OS_ERROR_ABORTED if the cryptographic operation failed or if process
 *  was called without calling start (e.g., for GCM mode) or if process is called
 *  after the CIPHER was already finalized
 */
OS_Error_t
OS_CryptoCipher_process(
    OS_CryptoCipher_Handle_t hCipher,
    const void*              input,
    const size_t             inputSize,
    void*                    output,
    size_t*                  outputSize);

/**
 * @brief Start processing of data (only relevant for some algorithms).
 *
 * This functions starts a computation for certain algorithms. One example is
 * AES-GCM, where besides encrypting data, additional data can be added for
 * authentication.
 *
 * @param hCipher (required) handle of OS Crypto CIPHER object
 * @param input (optional) input data
 * @param inputSize (optional) length of input data
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes passing an oversized or too small buffer
 * @retval OS_ERROR_ABORTED if CIPHER object does not require start, or if it
 *  was already started or if the internal cryptographic operation failed
 * @retval OS_ERROR_INSUFFICIENT_SPACE if \p inputSize is greater than
 *  the size of the dataport
 */
OS_Error_t
OS_CryptoCipher_start(
    OS_CryptoCipher_Handle_t hCipher,
    const void*              input,
    const size_t             inputSize);

/**
 * @brief Finish processing of data (only relevant for some algorithms).
 *
 * This function finishes a computation for certain algorithms. One example is
 * AES-GCM: in encryption mode, finalize(() then writes the authentication tag
 * to \p tag. In decryption mode it re-computes the tag internally and compares
 * it with a tag that must be provided in \p tag.
 *
 * For GCM in encryption mode, the \p tagSize must be >= 4, as the resulting tag
 * can be shortened if desired.
 *
 * @param hCipher (required) handle of OS Crypto CIPHER object
 * @param tag (required) input/output buffer for final operation
 * @param tagSize (required) length of input/size of output buffer, will be set
 *  to actual amount of bytes written if function succeeds (or the minimum size
 *  fails)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 *  this includes passing an oversized or too small buffer
 * @retval OS_ERROR_ABORTED if CIPHER does not require finalize, or if CIPHER
 *  was already finalized or if it was not started and did not process any data yet
 *  or if underlying operation failed
 */
OS_Error_t
OS_CryptoCipher_finalize(
    OS_CryptoCipher_Handle_t hCipher,
    void*                    tag,
    size_t*                  tagSize);

/** @} */
