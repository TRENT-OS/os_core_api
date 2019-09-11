/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @addtogroup API
 * @{
 *
 * @file SeosCryptoAPI.h
 *
 * @brief SEOS Crypto API library
 *
 */
#pragma once

/***************************** Crypto functions *******************************/
/**
 * @brief generate random number
 *
 * @param cryptoCtx (required) pointer to the seos_crypto context
 * @param buffer random data buffer container
 * @param len capacity of buffer
 *
 * @return an error code
 * @retval SEOS_SUCCESS if all right
 * @retval SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is
 *  missing or wrong
 * @retval SEOS_ERROR_UNSUPPORTED requested random source is not supported or
 *  requested length of random data is not supported for this source
 * @retval SEOS_ERROR_ABORTED operation has been aborted, can happen if random
 *  source had an internal error or became unavailable during the operation. It
 *  may also happen if the operation is running for too long
 *
 */
seos_err_t
SeosCryptoApi_rngGetBytes(SeosCryptoCtx*    cryptoCtx,
                          void**            buffer,
                          size_t            dataLen);

seos_err_t
SeosCryptoApi_rngReSeed(SeosCryptoCtx*      cryptoCtx,
                        const void*         seed,
                        size_t              seedLen);

/**
 * @brief initializes a digest context (local or remote)
 *
 * @param self (required) pointer to SeosCryptoCtx context
 * @param pDigestHandle (required) output parameter to receive the handle of the
 *  created local or remote digest context
 * @param algorithm the digest algorithm
 * @param iv (optional) the initialization vector
 * @param ivLen the initialization vector length
 *
 * @return an error code
 * @retval SEOS_SUCCESS if all right
 * @retval SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is
 *  missing
 * @retval SEOS_ERROR_NOT_SUPPORTED if there is no implementation for the given
 *  algorithm
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if not enough resources
 */
seos_err_t
SeosCryptoApi_digestInit(SeosCryptoCtx*                 cryptoCtx,
                         SeosCrypto_DigestHandle*       pDigestHandle,
                         unsigned int                   algorithm,
                         void*                          iv,
                         size_t                         ivLen);
/**
 * @brief closes the digest context referred by \p digestHandle
 *
 * @retval SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosCryptoApi_digestClose(SeosCryptoCtx*                cryptoCtx,
                          SeosCrypto_DigestHandle       digestHandle);
/**
 * @brief updates the computation of the digest providing a new block of data
 *
 * @param self (required) pointer to the SeosCryptoCtx context
 * @param digestHandle (required) handle that refers to a local or remote digest
 *  context
 * @param data (required) the data block
 * @param dataLen the length of the data block
 *
 * @return an error code.
 *
 * @retval SEOS_SUCCESS if all right
 * @retval SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is
 *  missing or wrong
 * @retval SEOS_ERROR_NOT_SUPPORTED if there is no implementation for the given
 *  algorithm
 * @retval SEOS_ERROR_ABORTED if the underlying implementation of the algorithm
 *  fails for any reason
 * @retval SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosCryptoApi_digestUpdate(SeosCryptoCtx*               cryptoCtx,
                           SeosCrypto_DigestHandle      digestHandle,
                           const void*                  data,
                           size_t                       dataLen);
/**
 * @brief finalizes the computation of the digest providing a new block of data
 *  or padding (when data == NULL).
 *
 * @param self (required) pointer to the SeosCryptoCtx context
 * @param digestHandle (required) handle that refers to a local or remote digest
 *  context
 * @param data (optional) the data block. When not provided (== NULL) then
 *  padding is done
 * @param dataLen the length of the data block
 * @param digest (required) a pointer to the buffer containing the digest.
 *  When *digest == NULL then a buffer is provided as output parameter otherwise
 *  if provided by the caller then it is just used. In this last case
 *  *digestSize is taken first as input to check the boundaries of the buffer
 *  and then in any case is set to the size of the digest before to return
 * @param digestSize (required) size of digest. Can work both as input or
 *  output parameter as described for \p digest
 *
 * @return an error code.
 *
 * @retval SEOS_SUCCESS if all right
 * @retval SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is
 *  missing or wrong
 * @retval SEOS_ERROR_NOT_SUPPORTED if there is no implementation for the given
 *  algorithm
 * @retval SEOS_ERROR_ABORTED if the underlying implementation of the algorithm
 *  fails for any reason or the output buffer is not big enough
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if the size of the digest buffer provided
 *  by the caller is not enough to hold the data generated
 * @retval SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosCryptoApi_digestFinalize(SeosCryptoCtx*             cryptoCtx,
                             SeosCrypto_DigestHandle    digestHandle,
                             const void*                data,
                             size_t                     dataLen,
                             void**                     digest,
                             size_t*                    digestSize);

/**
 * @brief Key API
 *
 */
seos_err_t
SeosCryptoApi_keyInit(SeosCryptoCtx*                   ctx,
                      SeosCrypto_KeyHandle*            keyHandle,
                      unsigned int                     type,
                      unsigned int                     flags,
                      size_t                           bits);

seos_err_t
SeosCryptoApi_keyGenerate(SeosCryptoCtx*               ctx,
                          SeosCrypto_KeyHandle         keyHandle);

seos_err_t
SeosCryptoApi_keyGeneratePair(SeosCryptoCtx*           ctx,
                              SeosCrypto_KeyHandle     prvKeyHandle,
                              SeosCrypto_KeyHandle     pubKeyHandle);

seos_err_t
SeosCryptoApi_keyImport(SeosCryptoCtx*                 ctx,
                        SeosCrypto_KeyHandle           keyHandle,
                        SeosCrypto_KeyHandle           wrapKeyHandle,
                        const void*                    keyBytes,
                        size_t                         keySize);

seos_err_t
SeosCryptoApi_keyExport(SeosCryptoCtx*                 ctx,
                        SeosCrypto_KeyHandle           keyHandle,
                        SeosCrypto_KeyHandle           wrapKeyHandle,
                        void**                         buf,
                        size_t*                        bufSize);

seos_err_t
SeosCryptoApi_keyDeInit(SeosCryptoCtx*                 ctx,
                        SeosCrypto_KeyHandle           keyHandle);

/**
 * @brief initializes a cipher context
 *
 * @param self (required) pointer to the SeosCryptoCtx context
 * @param pCipherHandle (required) output parameter to receive the handle of the
 *  created local or remote cipher context
 * @param algorithm the cipher algorithm
 * @param key (required) the cipher key
 * @param iv (optional) the initialization vector
 * @param ivLen the initialization vector length
 *
 * @return an error code
 * @retval SEOS_SUCCESS if all right
 * @retval SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is
 *  missing or wrong
 * @retval SEOS_ERROR_NOT_SUPPORTED if there is no implementation for the given
 *  algorithm
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if not enough resources
 *
 */
seos_err_t
SeosCryptoApi_cipherInit(SeosCryptoCtx*                 cryptoCtx,
                         SeosCrypto_CipherHandle*       pCipherHandle,
                         unsigned int                   algorithm,
                         SeosCrypto_KeyHandle           keyHandle,
                         const void*                    iv,
                         size_t                         ivLen);
/**
 * @brief closes the cipher context referred by \p digestHandle
 *
 * @retval SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosCryptoApi_cipherClose(SeosCryptoCtx*                cryptoCtx,
                          SeosCrypto_CipherHandle       cipherHandle);
/**
 * @brief performs cipher operation on a block
 *
 * @param self (required) pointer to the SeosCryptoCtx context
 * @param cipherHandle (required) handle that refers to a local or remote cipher
 *  context
 * @param data (required) input buffer
 * @param dataLen input data legth
 * @param output (optional) input/output parameter cointaining the pointer to
 *  the output buffer. If content is == NULL, then it is set to a local (to the
 *  context) buffer and the content of \p outputSize is set to the correct value
 *  of the amount of written data. Otherwise (!= NULL) the given buffer is used
 *  as output and the value in \p outputSize is used (in the meaning of capacity
 *  of the buffer) for boundary check before writing. If write is possible then
 *  the value of \p outputSize is set to the correct value of the amount of
 *  written data.
 * @param outputSize (required) input/output parameter holding the capacity/size
 *  of \p output
 *
 * @return an error code
 * @retval SEOS_SUCCESS if all right
 * @retval SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is
 *  missing or wrong
 * @retval SEOS_ERROR_NOT_SUPPORTED if there is no implementation for the given
 *  algorithm
 * @retval SEOS_ERROR_ABORTED if the underlying implementation of the algorithm
 *  fails for any reason or the output buffer is not big enough
 * @retval SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosCryptoApi_cipherUpdate(SeosCryptoCtx*               cryptoCtx,
                           SeosCrypto_CipherHandle      cipherHandle,
                           const void*                  data,
                           size_t                       dataLen,
                           void**                       output,
                           size_t*                      outputSize);
/**
 * @brief given the reference to the cipher context \p cipherHandle, it performs
 * the semantic of SeosCryptoCipher_updateAd()
 *
 * @retval SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosCryptoApi_cipherUpdateAd(SeosCryptoCtx*               cryptoCtx,
                             SeosCrypto_CipherHandle      cipherHandle,
                             const void*                  data,
                             size_t                       dataLen);

/**
 * @brief given the reference to the cipher context \p cipherHandle, it performs
 * the semantic of SeosCryptoCipher_finalize()
 *
 * @retval SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosCryptoApi_cipherFinalize(SeosCryptoCtx*               cryptoCtx,
                             SeosCrypto_CipherHandle      cipherHandle,
                             void**                       output,
                             size_t*                      outputSize);

/**
 * @brief given the reference to the cipher context \p cipherHandle, it performs
 * the semantic of SeosCryptoCipher_verifyTag()
 *
 * @retval SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosCryptoApi_cipherVerifyTag(SeosCryptoCtx*               cryptoCtx,
                              SeosCrypto_CipherHandle      cipherHandle,
                              const void*                  tag,
                              size_t                       tagSize);

/** @} */
