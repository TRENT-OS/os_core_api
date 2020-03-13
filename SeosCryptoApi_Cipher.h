/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Cipher.h
 *
 * @brief SEOS Crypto API library types, constants and enums for Cipher object
 *
 */

#pragma once

#include "SeosError.h"

#include <stddef.h>

#define SeosCryptoApi_Cipher_SIZE_AES_BLOCK         16
#define SeosCryptoApi_Cipher_SIZE_AES_CBC_IV        16
#define SeosCryptoApi_Cipher_SIZE_AES_GCM_IV        12
#define SeosCryptoApi_Cipher_SIZE_AES_GCM_TAG_MIN   4
#define SeosCryptoApi_Cipher_SIZE_AES_GCM_TAG_MAX   SeosCryptoApi_Cipher_SIZE_AES_BLOCK

typedef enum
{
    SeosCryptoApi_Cipher_ALG_NONE = 0,
    SeosCryptoApi_Cipher_ALG_AES_ECB_ENC,
    SeosCryptoApi_Cipher_ALG_AES_ECB_DEC,
    SeosCryptoApi_Cipher_ALG_AES_CBC_ENC,
    SeosCryptoApi_Cipher_ALG_AES_CBC_DEC,
    SeosCryptoApi_Cipher_ALG_AES_GCM_ENC,
    SeosCryptoApi_Cipher_ALG_AES_GCM_DEC,
    SeosCryptoApi_Cipher_ALG_RSA_PKCS1_ENC,
    SeosCryptoApi_Cipher_ALG_RSA_PKCS1_DEC
}
SeosCryptoApi_Cipher_Alg;

typedef SeosCryptoApi_Proxy* SeosCryptoApi_CipherH;

/**
 * @brief Initialize a cipher object
 *
 * This function allocates a cipher object and initializes its internal state.
 *
 * Currently supported algorithms are:
 * - AES-ECB
 * - AES-CBC (no padding)
 * - AES-GCM
 *
 * Some algorithms do require an IV:
 * - AES-GCM requires 12 bytes of IV
 * - AES-CBC requires 16 bytes of IV
 *
 * @param hCipher (required) pointer to handle of SEOS Crypto Cipher object
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param hKey (required) handle of SEOS Crypto Key object
 * @param algorithm (required) cipher algorithm to use
 * @param iv (optional) initialization vector required for some ciphers
 * @param ivSize (optional) length of initialization vector
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes if an iv is given for an algorithm that does not require an IV
 * or if \p iv is NOT set for an algorithm that does require an IV; also includes
 * mismatching IV sizes or passing a key that is not matching the algorithm
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_ABORTED if setting the key internally failed
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the cipher failed or if
 * \p ivSize is greater than `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Cipher_init(
    SeosCryptoApi_CipherH*         hCipher,
    const SeosCryptoApiH           hCrypto,
    const SeosCryptoApi_KeyH       hKey,
    const SeosCryptoApi_Cipher_Alg algorithm,
    const void*                    iv,
    const size_t                   ivSize);

/**
 * @brief Finish cipher object
 *
 * @param hCipher (required) handle of SEOS Crypto Cipher object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Cipher_free(
    SeosCryptoApi_CipherH hCipher);

/**
 * @brief Process data blocks with the cipher
 *
 * Process blocks of data with the selected algorithm. If the cipher is operated
 * in ENC mode, \p input will hold the plaintext and \p output will be the cipertext.
 * If operated in DEC mode, it will be vice versa.
 *
 * Special attention needs to be paid to the alignment of inputs:
 * - AES-ECB and AES-CBC require all inputs to be aligned to 16 byte blocks
 * - AES-GCM can deal with non-aligned blocks, but only in the last call to
 *   this function.
 *
 * @param hCipher (required) handle of SEOS Crypto Cipher object
 * @param input (required) input data for cipher
 * @param inputSize (required) length of input data
 * @param output (required) buffer for resulting output data
 * @param outputSize (required) size of output buffer, will be set to actual
 * amount of bytes written if function succeeds (or to the minimum size if it
 * fails)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes passing \p inputSize that is not aligned with the underlying
 * blocksize
 * @retval SEOS_ERROR_ABORTED if the cryptographic operation failed or if process
 * was called without calling start (e.g., for GCM mode) or if process is called
 * after the cipher was already finalized
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p outputSize is too small to hold
 * the full result in the \p output buffer
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p inputSize or \p outputSize is
 * greater than `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Cipher_process(
    SeosCryptoApi_CipherH hCipher,
    const void*           input,
    const size_t          inputSize,
    void*                 output,
    size_t*               outputSize);

/**
 * @brief Start encryption of data (only relevant for some algorithms)
 *
 * This functions starts a computation for certain algorithms. One example is
 * AES GCM, where besides encrypting data, additional data can be added for
 * authentication.
 *
 * Will not work with algorithms that don't require it (e.g., AES-ECB).
 *
 * @param hCipher (required) handle of SEOS Crypto Cipher object
 * @param input (optional) input data for cipher
 * @param inputSize (optional) length of input data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if cipher object does not require start, or if it
 * was already started or if the internal cryptographic operation failed
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p inputSize is greater than
 * `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Cipher_start(
    SeosCryptoApi_CipherH hCipher,
    const void*           input,
    const size_t          inputSize);

/**
 * @brief Finish encryption of data (only relevant for some algorithms)
 *
 * This function finishes a computation for certain algorithms. One example is
 * AES GCM: in encryption mode, finalize writes the authentication tag to \p tag,
 * in decryption mode it recomputes the tag internally and compares it with a
 * tag that must be provided in \p tag.
 *
 * For GCM in encryption mode, the \p tagSize must be >= 4, as the resulting tag
 * can be shortened if desired.
 *
 * Will not work with algorithms that don't require it.
 *
 * @param hCipher (required) handle of SEOS Crypto Cipher object
 * @param tag (required) input/output buffer for final cipher operation
 * @param tagSize (required) lenght of input/size of output buffer, will be set
 * to actual amount of bytes written if function succeeds (or the minimum size
 * fails)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if cipher does not require finalize, or if cipher
 * was already finalized or if it was not started and did not process any data yet
 * or if underlying operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p tagSize is either too small for data
 * written to the \p tag buffer
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p tagSize is greater than
 * `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Cipher_finalize(
    SeosCryptoApi_CipherH hCipher,
    void*                 tag,
    size_t*               tagSize);

/** @} */
