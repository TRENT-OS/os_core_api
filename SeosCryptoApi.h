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

#include "SeosCryptoAgreement_Impl.h"
#include "SeosCryptoCipher_Impl.h"
#include "SeosCryptoDigest_Impl.h"
#include "SeosCryptoKey_Impl.h"
#include "SeosCryptoRng_Impl.h"
#include "SeosCryptoSignature_Impl.h"

// -------------------------------- RNG API ------------------------------------

/**
 * @brief Generate random numbers
 *
 * The internal RNG is based on CTR_DRBG using AES. It is fed from an entropy
 * source that has to be provided at the initialization of the crypto API.
 *
 * Whenever this function is called, it will internally try to add more entropy
 * from the entropy source into the RNG state to enhance prediction resistance.
 * This behavior can be modified by passing respective \p flags.
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p bufSize.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param flags (optional) flags for RNG operation
 * @param buf (required) buffer for random bytes
 * @param bufSize (required) amount of random bytes requested
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter is missing or invalid
 * @retval SEOS_ERROR_ABORTED if the internal RNG had a failure
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p flags are not supported by RNG
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p bufSize is too large
 */
seos_err_t
SeosCryptoApi_rngGetBytes(SeosCryptoCtx*            ctx,
                          const SeosCryptoRng_Flags flags,
                          void*                     buf,
                          const size_t              bufSize);

/**
 * @brief Reseed the internal RNG
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p seedLen.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param seed (required) additional seed to feed into RNG state
 * @param seedLen (required) length of seed data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter is missing or invalid*
 * @retval SEOS_ERROR_ABORTED if the RNG had a failure
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p seedLen is too large
 */
seos_err_t
SeosCryptoApi_rngReSeed(SeosCryptoCtx*      ctx,
                        const void*         seed,
                        const size_t        seedLen);

// ------------------------------ Digest API -----------------------------------

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
 * @param algorithm (required) digest algorithm to use, see #SeosCryptoDigest_Algorithm
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the internal state could not be initialized
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the digest failed
 */
seos_err_t
SeosCryptoApi_digestInit(SeosCryptoCtx*                     ctx,
                         SeosCrypto_DigestHandle*           pDigestHandle,
                         const SeosCryptoDigest_Algorithm   algorithm);

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
SeosCryptoApi_digestFree(SeosCryptoCtx*                ctx,
                         const SeosCrypto_DigestHandle digestHandle);

/**
 * @brief Process block of data with digest algorithm
 *
 * Update internal state of digest algorithm with another block of data.
 * 
 * The amount of data that can be passed here is limited to approx. 4000 bytes 
 * (PAGE_SIZE - sizeof(size_t)),  this applies to \p dataLen. As a result
 * larger blocks of data need to be broken into smaller chunks by the caller.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param digestHandle (required) initialized digest handle
 * @param data (required) data to process with digest
 * @param dataLen (required) length of data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if processing of the block failed or if digest was
 * was already finalized
 */
seos_err_t
SeosCryptoApi_digestProcess(SeosCryptoCtx*                  ctx,
                            const SeosCrypto_DigestHandle    digestHandle,
                            const void*                      data,
                            const size_t                     dataLen);

/**
 * @brief Finish digest computation to produce digest
 *
 * Write the digest resulting from any preceding calls to process into a buffer.
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p digestSize.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param digestHandle (required) initialized digest handle
 * @param digest (required) buffer to write digest to
 * @param digestSize (required) size of digest buffer
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the digest could not be produced or if no
 * blocks were processed before finalizing or if finalize was already called
 */
seos_err_t
SeosCryptoApi_digestFinalize(SeosCryptoCtx*                 ctx,
                             const SeosCrypto_DigestHandle  digestHandle,
                             void*                          digest,
                             size_t*                        digestSize);

// -------------------------------- Key API ------------------------------------

/**
 * @brief Initialize key object
 *
 * This function allocates a key object of a certain type. Initially, this key
 * is empty and has to be filled either by generating key material or importing
 * a known key. Keys can be created with certain flags, which indicate if a key
 * can be freely exported, if it has to be wrapped or if it cannot be exported
 * at all. Keys have a type and when used with any of the algorithms must match
 * the given algorithm.
 *
 * The following key types are supported:
 * - RSA private/public key
 * - DH private/public key
 * - EC private/public key for SECP256r1 curve
 * - AES symmetric key
 *
 * The \p bits parameter here is important because it is enforced during key
 * generation and import. For example: If the key is an RSA private key with 1024
 * bits, importing a private key where the modulus is smaller or bigger will fail.
 *
 * The following key sizes are supported:
 * - RSA: 128 bits - 4096 bits
 * -  DH:  64 bits - 4096 bits
 * -  EC: 256 bits (fixed due to SECP256r1 curve)
 * - AES: 128, 192, 256 bits
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pKeyHandle (required) pointer to key handle
 * @param type (required) type of key to create, see #SeosCryptoKey_Type
 * @param flags (optional) flags indicating key criticality, see #SeosCryptoKey_Flags
 * @param bits (required) keysize in bits, e.g., 128 bits for AES or 1024 bits for
 * RSA, etc.
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid, this
 * includes giving invalid keysizes in \p bits
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the key failed
 */
seos_err_t
SeosCryptoApi_keyInit(SeosCryptoCtx*                ctx,
                      SeosCrypto_KeyHandle*         keyHandle,
                      const SeosCryptoKey_Type      type,
                      const SeosCryptoKey_Flags     flags,
                      const size_t                  bits);

/**
 * @brief Generate key with internal RNG
 *
 * This function uses the internal RNG of the crypto API to generate a symmetric
 * key.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param keyHandle (required) initialized key handle
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid*
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if \p keyHandle has already key material attached or
 * if there was an error generating the key
 */
seos_err_t
SeosCryptoApi_keyGenerate(SeosCryptoCtx*               ctx,
                          const SeosCrypto_KeyHandle   keyHandle);

/**
 * @brief Generate a private/public keypair with internal RNG
 *
 * This function uses the internal RNG of the crypto API to generate a keypair
 * of a certain type. The key objects given in \p prvKeyHandle and \p pubKeyHandle
 * need to be of the same algorithm type (e.g. RSA_PRV and RSA_PUB).
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param prvKeyHandle (required) initialized key handle for private key
 * @param pubKeyHandle (required) initialized key handle for public key
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if \p prvKeyHandle or \p pubKeyHandle has already
 * key material attached or if there was an error generating the keypair
 */
seos_err_t
SeosCryptoApi_keyGeneratePair(SeosCryptoCtx*                ctx,
                              const SeosCrypto_KeyHandle    prvKeyHandle,
                              const SeosCrypto_KeyHandle    pubKeyHandle);

/**
 * @brief Import key data into key object from buffer
 *
 * This function imports key material into an allocated key object. If the key
 * material is wrapped, a wrapping key must be given so that the key material
 * can be unwrapped before importing it.
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p keySize.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param keyHandle (required) initialized key handle
 * @param wrapKeyHandle (optional) handle of key used for unwrapping the key
 * material before importing it into the crypto API
 * @param keyBytes (required) buffer for key material to import
 * @param keySize (required) size of key material in buffer
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid;
 * this includes supplying an unexpected amount of bytes in \p keySize or key data
 * that does not provide the correct level of security (indicated by \p bits in keyInit)
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p wrapKeyHandle is not NULL (wrapping is
 * not yet supported)
 * @retval SEOS_ERROR_ABORTED if \p keyHandle had already key material attached
 */
seos_err_t
SeosCryptoApi_keyImport(SeosCryptoCtx*                 ctx,
                        const SeosCrypto_KeyHandle     keyHandle,
                        const SeosCrypto_KeyHandle     wrapKeyHandle,
                        const void*                    keyBytes,
                        const size_t                   keySize);

/**
 * @brief Export key data from key object into buffer
 *
 * If the key is exportable, this function will write the key material stored
 * in the key object to a buffer. If the key is to be wrapped, a wrapping key
 * must be given which then will be used to encrypt \p keyHandle before writing
 * it to the buffer. If a key is not exportable, this function will fail.
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p bufSize.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param keyHandle (required) initialized key handle
 * @param wrapKeyHandle (optional) handle of key used for wrapping the key
 * material before exporting it into buffer
 * @param keyBytes (required) buffer for key material to export
 * @param keySize (required) size of buffer, will be set to amount of bytes
 * written if functions succeeds
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the \p keyHandle had no key material
 * @retval SEOS_ERROR_ACCESS_DENIED if the key cannot be exported due to flags
 * set during creation of the key object
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p wrapKeyHandle is not NULL (wrapping is
 * currently not supported)
 */
seos_err_t
SeosCryptoApi_keyExport(SeosCryptoCtx*                 ctx,
                        const SeosCrypto_KeyHandle     keyHandle,
                        const SeosCrypto_KeyHandle     wrapKeyHandle,
                        void*                          buf,
                        size_t*                        bufSize);

/**
 * @brief Finish a key object
 *
 * This function frees the memory associated with the key object and zeroizes
 * any key material that was stored internally.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param keyHandle (required) initialized key handle
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_keyFree(SeosCryptoCtx*                ctx,
                      const SeosCrypto_KeyHandle    keyHandle);

// ----------------------------- Signature API ---------------------------------

/**
 * @brief Initialize a signature object
 *
 * This function allocates a signature object and initializes it. It can hold a
 * private and public key (or both) and based on what is provided during
 * initialization the respective private operation (signing) or public operation
 * (verification) are possible.
 *
 * Currently supported algorithms are:
 * - RSA according to PKCS#1
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pSigHandle (required) pointer to signature handle
 * @param algorithm (required) signature algorithm to use, see #SeosCryptoSignature_Algorithm
 * @param prvHandle (optional) key to use for private operations
 * @param pubHandle (optional) key to use for public operations
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid, this
 * includes passing the wrong types of key
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the digest failed
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 */
seos_err_t
SeosCryptoApi_signatureInit(SeosCryptoCtx*                      ctx,
                            SeosCrypto_SignatureHandle*         pSigHandle,
                            const SeosCryptoSignature_Algorithm algorithm,
                            const SeosCrypto_KeyHandle          prvHandle,
                            const SeosCrypto_KeyHandle          pubHandle);

/**
 * @brief Finish a signature object
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param sigHandle (required) signature handle
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_signatureFree(SeosCryptoCtx*                      ctx,
                            const SeosCrypto_SignatureHandle    sigHandle);

/**
 * @brief Sign a hash value
 *
 * Sign a digest value (typically 16-32 bytes) with the private key of the
 * signature object; for this the \p prvHandle param must be set during signature
 * initialization.
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p hashSize and \p signatureSize.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param sigHandle (required) signature handle
 * @param hash (required) hash value to sign
 * @param hashSize (required) size of hash
 * @param signature (required) buffer for resulting signature
 * @param signatureSize (required) size of signature buffer, will be set to
 * actually written amount of bytes if function succeeds
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if no private key was set during initialization or
 * if the cryptographic operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if the \p signatureSize is too small or \p
 * hashSize is too large
 */
seos_err_t
SeosCryptoApi_signatureSign(SeosCryptoCtx*                      ctx,
                            const SeosCrypto_SignatureHandle    sigHandle,
                            const void*                         hash,
                            const size_t                        hashSize,
                            void*                               signature,
                            size_t*                             signatureSize);

/**
 * @brief Verify signature over a hash
 *
 * Verify a signature for a given value, which is usually a message digest of
 * fixed size (16-32 bytes). For this operation to work, the  \p pubHandle
 * param must be set during signature initialization.
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p hashSize and \p signatureSize.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param sigHandle (required) signature handle
 * @param hash (required) hash value to recompute signature for
 * @param hashSize (required) size of hash
 * @param signature (required) buffer for signature to verify
 * @param signatureSize (required) size of signature in buffer
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes passing a \p signatureSize that is unexpected (due to the key)
 * @retval SEOS_ERROR_ABORTED if no private key was set during initialization or
 * if the cryptographic operation failed (i.e., the signature was invalid)
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p signatureSize or \p hashSize
 * is too large
 */
seos_err_t
SeosCryptoApi_signatureVerify(SeosCryptoCtx*                    ctx,
                              const SeosCrypto_SignatureHandle  sigHandle,
                              const void*                       hash,
                              const size_t                      hashSize,
                              const void*                       signature,
                              const size_t                      signatureSize);

// ----------------------------- Agreement API ---------------------------------

/**
 * @brief Initialize an agreement object
 *
 * This function allocates a key agreement object and initializes it. A private
 * key needs to be given, which will be used during the agreement phase to derive
 * a shared secret.
 *
 * Currently supported algorithms are:
 * - DH
 * - ECDH
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pAgrHandle (required) pointer to agreement handle
 * @param algorithm (required) key agreement algorithm to use, see #SeosCryptoAgreement_Algorithm
 * @param prvHandle (required) handle of private key to use for key agreement
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes provding the wrong type of key (e.g. AES key for DH algorithm)
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the agreement
 * object failed
 */
seos_err_t
SeosCryptoApi_agreementInit(SeosCryptoCtx*                      ctx,
                            SeosCrypto_AgreementHandle*         pAgrHandle,
                            const SeosCryptoAgreement_Algorithm algorithm,
                            const SeosCrypto_KeyHandle          prvHandle);

/**
 * @brief Finish a agreement object
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param agrHandle (required) initialized agreement handle
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_agreementFree(SeosCryptoCtx*                      ctx,
                            const SeosCrypto_AgreementHandle    agrHandle);

/**
 * @brief Agree on a shared value
 *
 * Computes the underlying key agreement based on the private key assosciated with
 * the object and a public key given here.
 *
 * The resulting shared secret is a bignum specific to the underlying algorithm,
 * e.g. in the case of DH \p shared represents an integer smaller than the prime
 * chosen for DH. A final processing step should be applied to the agreed key in
 * order to produce a symmetric key of suitable size.
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p sharedSize.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param agrHandle (required) initialized agreement handle
 * @param pubHandle (required) public key to use for key agreement
 * @param shared (required) buffer to hold shared secret
 * @param sharedSize (required) size of buffer, will be set to actual amount of
 * bytes written if function succeeds
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid*
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes passing the wrong type of key
 * @retval SEOS_ERROR_ABORTED if the underlying agreement operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p sharedSize is too small
 */
seos_err_t
SeosCryptoApi_agreementAgree(SeosCryptoCtx*                     ctx,
                             const SeosCrypto_AgreementHandle   agrHandle,
                             const SeosCrypto_KeyHandle         pubHandle,
                             void*                              shared,
                             size_t*                            sharedSize);

// ------------------------------- Cipher API ----------------------------------

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
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p ivLen.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pCipherHandle (required) pointer to cipher handle
 * @param algorithm (required) cipher algorithm to use, see #SeosCryptoCipher_Algorithm
 * @param keyHandle (required) handle of key to use for cipher operation
 * @param iv (optional) initiialization vector required for some ciphers
 * @param ivLen (optional) length of initialization vector
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes if an iv is given for an algorithm that does not require an IV
 * or if \p iv is NOT set for an algorithm that does require an IV; also includes
 * mismatching IV sizes or passing a key that is not matching the algorithm
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p algorithm is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the cipher failed
 * @retval SEOS_ERROR_ABORTED if setting the key internally failed
 */
seos_err_t
SeosCryptoApi_cipherInit(SeosCryptoCtx*                     ctx,
                         SeosCrypto_CipherHandle*           pCipherHandle,
                         const SeosCryptoCipher_Algorithm   algorithm,
                         const SeosCrypto_KeyHandle         keyHandle,
                         const void*                        iv,
                         const size_t                       ivLen);

/**
 * @brief Finish cipher object
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param cipherHandle (required) initialized cipher handle
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_cipherFree(SeosCryptoCtx*                 ctx,
                         const SeosCrypto_CipherHandle  cipherHandle);

/**
 * @brief Process data blocks with the cipher
 *
 * Process blocks of data with the selected algorithm. If the cipher is operated
 * in ENC mode, \p input will hold the plaintext and \p output will be the cipertext.
 * If operated in DEC mode, it will be vice versa.
 *
 * The amount of data that can be passed here is limited to approx. 4000 bytes 
 * (PAGE_SIZE - sizeof(size_t)), this applies to \p inputLen and \p outputLen. 
 * As a result larger blocks of data need to be broken into smaller chunks by 
 * the caller.
 *
 * Special attention needs to be paid to the alignment of inputs:
 * - AES-ECB and AES-CBC require all inputs to be aligned to 16 byte blocks
 * - AES-GCM can deal with non-aligned blocks, but only in the last call to 
 *   this function.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param cipherHandle (required) initialized cipher handle
 * @param input (required) input data for cipher
 * @param inputLen (required) length of input data
 * @param output (required) buffer for resulting output data
 * @param outputSize (required) size of output buffer, will be set to actual
 * amount of bytes written if function succeeds
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes passing \p inputSize that is not aligned with the underlying
 * blocksize
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p outputSize is too small or \p inputLen
 * is too large
 * @retval SEOS_ERROR_ABORTED if the cryptographic operation failed or if process
 * was called without calling start (e.g., for GCM mode) or if process is called
 * after the cipher was already finalized
 */
seos_err_t
SeosCryptoApi_cipherProcess(SeosCryptoCtx*                  ctx,
                            const SeosCrypto_CipherHandle    cipherHandle,
                            const void*                      input,
                            const size_t                     inputLen,
                            void*                            output,
                            size_t*                          outputSize);

/**
 * @brief Start encryption of data (only relevant for some algorithms)
 *
 * This functions starts a computation for certain algorithms. One example is
 * AES GCM, where besides encrypting data, additional data can be added for
 * authentication.
 *
 * Will not work with algorithms that don't require it (e.g., AES-ECB).
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p inputLen.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param cipherHandle (required) initialized cipher handle
 * @param input (optional) input data for cipher
 * @param inputLen (optional) length of input data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if cipher object does not require start, or if it
 * was already started or if the internal cryptographic operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p inputLen is too large
 */
seos_err_t
SeosCryptoApi_cipherStart(SeosCryptoCtx*                ctx,
                          const SeosCrypto_CipherHandle cipherHandle,
                          const void*                   input,
                          const size_t                  inputLen);

/**
 * @brief Finish encryption of data (only relevant for some algorithms)
 *
 * This function finishes a computation for certain algorithms. One example is
 * AES GCM: in encryption mode, finalize writes the authentication tag to \p buf,
 * in decryption mode it recomputes the tag internally and compares it with a
 * tag that must be provided in \p buf.
 *
 * Will not work with algorithms that don't require it.
 *
 * Buffer sizes are limited to approx. 4000 bytes (PAGE_SIZE - sizeof(size_t)),
 * this applies to \p bufSize.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param cipherHandle (required) initialized cipher handle
 * @param buf (required) input/output buffer for final cipher operation
 * @param bufSize (required) lenght of input/size of output buffer, will be set
 * to actual amount of bytes written if function succeeds
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if cipher does not require finalize, or if cipher
 * was already finalized or if it was not started and did not process any data yet
 * or if underlying operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p bufSize is either too small for data
 * written by finalize or too large
 */
seos_err_t
SeosCryptoApi_cipherFinalize(SeosCryptoCtx*                 ctx,
                             const SeosCrypto_CipherHandle  cipherHandle,
                             void*                          buf,
                             size_t*                        bufSize);

/** @} */
