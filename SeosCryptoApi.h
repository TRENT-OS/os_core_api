/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi.h
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
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p bufSize is greater than
 * `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_rngGetBytes(SeosCryptoCtx*            ctx,
                          const SeosCryptoRng_Flags flags,
                          void*                     buf,
                          const size_t              bufSize);

/**
 * @brief Reseed the internal RNG
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param seed (required) additional seed to feed into RNG state
 * @param seedSize (required) length of seed data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter is missing or invalid
 * @retval SEOS_ERROR_ABORTED if the RNG had a failure
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p seedSize is greater than
 *  `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_rngReSeed(SeosCryptoCtx*      ctx,
                        const void*         seed,
                        const size_t        seedSize);

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
 * @param ctx (required) pointer to the seos crypto context
 * @param digestHandle (required) initialized digest handle
 * @param data (required) data to process with digest
 * @param dataSize (required) length of data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if processing of the block failed or if digest was
 * was already finalized
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p dataSize is greater than
 *  `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_digestProcess(SeosCryptoCtx*                  ctx,
                            const SeosCrypto_DigestHandle   digestHandle,
                            const void*                     data,
                            const size_t                    dataSize);

/**
 * @brief Finish digest computation to produce digest
 *
 * Write the digest resulting from any preceding calls to process into a buffer.
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
 * `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_digestFinalize(SeosCryptoCtx*                 ctx,
                             const SeosCrypto_DigestHandle  digestHandle,
                             void*                          digest,
                             size_t*                        digestSize);

// -------------------------------- Key API ------------------------------------

/**
 * @brief Generate symmetric/private key with internal RNG
 *
 * This function allocates a key and fills it with key material generated by the
 * internal RNG.
 *
 * This function takes a \p spec parameter, which defines what type of key is
 * generated and wich attributes the resulting key has.
 *
 * The following types of keys with the following key sizes can be generated
 * with this function:
 * - `SeosCryptoKey_Type_RSA_PRV`:        RSA private key (128-4096 bits)
 * - `SeosCryptoKey_Type_DH_PRV`:         DH private key (64-4096 bits)
 * - `SeosCryptoKey_Type_SECP256R1_PRV`:  ECC private key for SECP256r1 curve
 * - `SeosCryptoKey_Type_AES`:            AES key (128, 192, 256 bits)
 *
 * Keys can have attributes, which have a flag parameter. The following flags are
 * supported at key generation:
 * - `SeosCryptoKey_Flags_NONE`:                  Key cannot be exported
 * - `SeosCryptoKey_Flags_EXPORTABLE_RAW`:        Key is exportable in 'raw' form
 * - `SeosCryptoKey_Flags_EXPORTABLE_WRAPPED`:    Key has to be wrapped before export
 *
 * Here are some example specs for typical keys:
 * 1. Create a DH priv key with 101 bits, which is NOT exportable (this is the default
 *    value). Use a parameter spec which already provides the prime P and base G.
 *    \code{.c}
 *      static const SeosCryptoKey_Spec dh101pSpec = {
 *          .type = SeosCryptoKey_SpecType_PARAMS,
 *          .key = {
 *              .type = SeosCryptoKey_Type_DH_PRV,
 *              .params.dh = {
 *                  .pBytes = {0x12, 0xdf, 0x4d, 0x76, ... 0x07},
 *                  .pLen   = 13,
 *                  .gBytes = {0x00, 0x1e, 0x32, 0x15, ... 0xd6},
 *                  .gLen   = 13,
 *              }
 *          }
 *      };
 *    \endcode
 * 2. Create a 128 bit AES key and make it exportable, use a bit spec for that:
 *    \code{.c}
 *      static const SeosCryptoKey_Spec aes128Spec = {
 *          .type = SeosCryptoKey_SpecType_BITS,
 *          .key = {
 *              .type = SeosCryptoKey_Type_AES,
 *              .attribs.flags = SeosCryptoKey_Flags_EXPORTABLE_RAW,
 *              .params.bits = 128
 *          }
 *      };
 *    \endcode
 * 3. Create 1024-bit RSA privkey, again using a bit spec:
 *    \code{.c}
 *      static const SeosCryptoKey_Spec rsa128Spec = {
 *          .type = SeosCryptoKey_SpecType_BITS,
 *          .key = {
 *              .type = SeosCryptoKey_Type_RSA_PRV,
 *              .params.bits = 1024
 *          }
 *      };
 *    \endcode
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pKeyHandle (required) pointer to key handle
 * @param spec (required) specification of key to create
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid, this
 * includes giving invalid keysizes where these are fixed (e.g., 100 bits for AES)
 * @retval SEOS_ERROR_NOT_SUPPORTED if the spec type, the key type or the amount of
 * key bits given by the spec is in an unsupported range (e.g. DH key with 40 bits)
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the key failed
 * @retval SEOS_ERROR_ABORTED if an internal error occured during cryptographic
 * operations
 */
seos_err_t
SeosCryptoApi_keyGenerate(SeosCryptoCtx*             ctx,
                          SeosCrypto_KeyHandle*      pKeyHandle,
                          const SeosCryptoKey_Spec*  spec);

/**
 * @brief Make a public key from a private key
 *
 * This function allocates a new key object and computes a public key based on
 * an exsiting private key. In order to make a keypair, generate() and makePublic()
 * have to be called in sequence.
 *
 * A public key can be computed based on a private key \p prvKeyHandle of this type:
 * - `SeosCryptoKey_Type_RSA_PRV`:        RSA private key
 * - `SeosCryptoKey_Type_DH_PRV`:         DH private key
 * - `SeosCryptoKey_Type_SECP256R1_PRV`:  ECC private key for SECP256r1 curve
 *
 * The following values are supported for as flags for \p attribs:
 * - `SeosCryptoKey_Flags_NONE`:                Key cannot be exported
 * - `SeosCryptoKey_Flags_EXPORTABLE_RAW`:      Key is exportable in 'raw' form
 * - `SeosCryptoKey_Flags_EXPORTABLE_WRAPPED`:  Key has to be wrapped before export
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pPrvKeyHandle (required) pointer to handle for private key
 * @param pPubKeyHandle (required) pointer to handle for public key
 * @param attribs (required) attributes to assign to public key
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if the type of \p prvKeyHandle is not supported
 * \p bits is in an invalid range for those algorithms which accept a range (e.g. DH)
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of any of the keys failed
 */
seos_err_t
SeosCryptoApi_keyMakePublic(SeosCryptoCtx*                  ctx,
                            SeosCrypto_KeyHandle*           pPubHandle,
                            const SeosCrypto_KeyHandle      prvKeyHandle,
                            const SeosCryptoKey_Attribs*    attribs);

/**
 * @brief Import key data into key object from buffer
 *
 * This function allocates a key object and imports key material from a buffer.
 * If the key material is wrapped, a wrapping key must be given so that the key
 * material can be unwrapped before importing it.
 *
 * During import, the sizes of the keys will be checked (e.g., based on the modulus
 * provided by RSA or based on the amounts of bytes passed for AES). The following
 * types and keysizes of the imported \p keyData are supported:
 * - `SeosCryptoKey_Type_AES`:            AES key (128, 192, 256 bits)
 * - `SeosCryptoKey_Type_RSA_PRV`:        RSA private key (128 - 4096 bits)
 * - `SeosCryptoKey_Type_RSA_PUB`:        RSA public key (128 - 4096 bits)
 * - `SeosCryptoKey_Type_DH_PRV`:         DH private key (64 - 4096 bits)
 * - `SeosCryptoKey_Type_DH_PUB`:         DH public key (64 - 4096 bits)
 * - `SeosCryptoKey_Type_SECP256R1_PRV`:  ECC private key for SECP256r1 curve
 * - `SeosCryptoKey_Type_SECP256r1_PUB`:  ECC public key for SECP256r1 curve
 *
 * The following values are supported as flags of the \p keyData attribs:
 * - `SeosCryptoKey_Flags_NONE`:                  Key cannot be exported
 * - `SeosCryptoKey_Flags_EXPORTABLE_RAW`:        Key is exportable in 'raw' form
 * - `SeosCryptoKey_Flags_EXPORTABLE_WRAPPED`:    Key has to be wrapped before export
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param pKeyHandle (required) pointer to key handle
 * @param wrapKeyHandle (optional) handle to existing key for unwrapping
 * @param keyData (required) buffer for key material to import
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid;
 * this includes supplying \p keyData that has internal inconsistencies (e.g.
 * too long buffer lengths) or key size that do not match what is expected
 * for algorithms where it is discretely defined (e.g., 120 bit key for AES)
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p wrapKeyHandle is not `NULL` (wrapping is
 * not yet supported) or if the data (e.g. sizes, key type, etc.) in \p keyData
 * is not supported, e.g., size of DH prime is not within the expected range
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the key failed
 */
seos_err_t
SeosCryptoApi_keyImport(SeosCryptoCtx*              ctx,
                        SeosCrypto_KeyHandle*       pKeyHandle,
                        const SeosCrypto_KeyHandle  wrapKeyHandle,
                        const SeosCryptoKey_Data*   keyData);

/**
 * @brief Export key data from key handle into buffer
 *
 * If the key is exportable, this function will write the key material stored
 * in the object indicated by \p keyHandle to a buffer. If the key is to be wrapped,
 * a wrapping key must be given which then will be used to encrypt \p keyHandle
 * before writing it to the buffer. If a key is not exportable, this function
 * will fail.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param keyHandle (required) handle of key to export
 * @param wrapKeyHandle (optional) handle of key used for wrapping the key
 * @param keyData (required) buffer for key data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_OPERATION_DENIED if the key cannot be exported due to flags
 * set during creation of the key object
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p wrapKeyHandle is not `NULL` (wrapping is
 * currently not supported)
 */
seos_err_t
SeosCryptoApi_keyExport(SeosCryptoCtx*              ctx,
                        const SeosCrypto_KeyHandle  keyHandle,
                        const SeosCrypto_KeyHandle  wrapKeyHandle,
                        SeosCryptoKey_Data*         keyData);

/**
 * @brief Get shared parameters from key
 *
 * Some key types have shared parameters, e.g., DHPrv and DHPub typically share
 * the prime P and base G. This function allows to read out these parameters. The
 * exportable flag is ignored here, as these are public parameters which may be
 * needed to generate more keys (e.g., in case of key exchange).
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param keyHandle (required) handle of key to export
 * @param param (required) buffer for key params
 * @param paramSize (required) buffer for key data, will be set to effectively
 * written bytes if function succeeds (or the minimum size if it fails due to too
 * small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if key has no exportable parameters
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p paramSize is too small to hold the
 * whole \p param
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p paramSize is greater than
 * `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_keyGetParams(SeosCryptoCtx*               ctx,
                           const SeosCrypto_KeyHandle   keyHandle,
                           void*                        param,
                           size_t*                      paramSize);

/**
 * @brief Load pre-defined parameters
 *
 * For some protocols, it is possible to agree on a set of pre-defined parameters,
 * e.g. use SECP256r1 curve or a fixed DH group. This function allows to read those
 * parameters, so they can be used with a PARAM spec to generate keys.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param name (required) name of the parameter set
 * @param param (required) buffer for key params
 * @param paramSize (required) buffer for key data, will be set to effectively
 * written bytes if function succeeds (or the minimum size if it fails due to too
 * small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p name indicates an unknown parameter
 * set
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p paramSize is too small to hold the
 * whole \p param
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p paramSize is greater than
 * `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_keyLoadParams(SeosCryptoCtx*               ctx,
                            const SeosCryptoKey_Param    name,
                            void*                        param,
                            size_t*                      paramSize);

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
 * @param algorithm (required) signature algorithm to use
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
 * @param ctx (required) pointer to the seos crypto context
 * @param sigHandle (required) signature handle
 * @param hash (required) hash value to sign
 * @param hashSize (required) size of hash
 * @param signature (required) buffer for resulting signature
 * @param signatureSize (required) size of signature buffer, will be set to
 * actually written amount of bytes if function succeeds (or the minimum size
 * if it fails due to too small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if no private key was set during initialization or
 * if the cryptographic operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p signatureSize is too small to hold
 * the resulting \p signature
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p hashSize or \p signatureSize is
 * greater than `SeosCrypto_BUFFER_SIZE`
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
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p signatureSize + \p hashSize is
 * greater than `SeosCrypto_BUFFER_SIZE`
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
 * @param algorithm (required) key agreement algorithm to use
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
 * @param ctx (required) pointer to the seos crypto context
 * @param agrHandle (required) initialized agreement handle
 * @param pubHandle (required) public key to use for key agreement
 * @param shared (required) buffer to hold shared secret
 * @param sharedSize (required) size of buffer, will be set to actual amount of
 * bytes written if function succeeds (or the minimum size if it fails due to too
 * small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid*
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes passing the wrong type of key
 * @retval SEOS_ERROR_ABORTED if the underlying agreement operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p sharedSize is too small to hold
 * the full result in the \p shared buffer
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p sharedSize is greater than
 * `SeosCrypto_BUFFER_SIZE`
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
 * @param ctx (required) pointer to the seos crypto context
 * @param pCipherHandle (required) pointer to cipher handle
 * @param algorithm (required) cipher algorithm to use
 * @param keyHandle (required) handle of key to use for cipher operation
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
 * \p ivSize is greater than `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_cipherInit(SeosCryptoCtx*                     ctx,
                         SeosCrypto_CipherHandle*           pCipherHandle,
                         const SeosCryptoCipher_Algorithm   algorithm,
                         const SeosCrypto_KeyHandle         keyHandle,
                         const void*                        iv,
                         const size_t                       ivSize);

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
 * Special attention needs to be paid to the alignment of inputs:
 * - AES-ECB and AES-CBC require all inputs to be aligned to 16 byte blocks
 * - AES-GCM can deal with non-aligned blocks, but only in the last call to
 *   this function.
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param cipherHandle (required) initialized cipher handle
 * @param input (required) input data for cipher
 * @param inputSize (required) length of input data
 * @param output (required) buffer for resulting output data
 * @param outputSize (required) size of output buffer, will be set to actual
 * amount of bytes written if function succeeds (or to the minimum size if it
 * fails)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid,
 * this includes passing \p inputSize that is not aligned with the underlying
 * blocksize
 * @retval SEOS_ERROR_ABORTED if the cryptographic operation failed or if process
 * was called without calling start (e.g., for GCM mode) or if process is called
 * after the cipher was already finalized
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p outputSize is too small to hold
 * the full result in the \p output buffer
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p inputSize or \p outputSize is
 * greater than `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_cipherProcess(SeosCryptoCtx*                  ctx,
                            const SeosCrypto_CipherHandle   cipherHandle,
                            const void*                     input,
                            const size_t                    inputSize,
                            void*                           output,
                            size_t*                         outputSize);

/**
 * @brief Start encryption of data (only relevant for some algorithms)
 *
 * This functions starts a computation for certain algorithms. One example is
 * AES GCM, where besides encrypting data, additional data can be added for
 * authentication.
 *
 * Will not work with algorithms that don't require it (e.g., AES-ECB).
 *
 * @param ctx (required) pointer to the seos crypto context
 * @param cipherHandle (required) initialized cipher handle
 * @param input (optional) input data for cipher
 * @param inputSize (optional) length of input data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if cipher object does not require start, or if it
 * was already started or if the internal cryptographic operation failed
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p inputSize is greater than
 * `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_cipherStart(SeosCryptoCtx*                ctx,
                          const SeosCrypto_CipherHandle cipherHandle,
                          const void*                   input,
                          const size_t                  inputSize);

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
 * @param ctx (required) pointer to the seos crypto context
 * @param cipherHandle (required) initialized cipher handle
 * @param tag (required) input/output buffer for final cipher operation
 * @param tagSize (required) lenght of input/size of output buffer, will be set
 * to actual amount of bytes written if function succeeds (or the minimum size
 * fails)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_HANDLE if the object handle is invalid
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if cipher does not require finalize, or if cipher
 * was already finalized or if it was not started and did not process any data yet
 * or if underlying operation failed
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p tagSize is either too small for data
 * written to the \p tag buffer
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p tagSize is greater than
 * `SeosCrypto_BUFFER_SIZE`
 */
seos_err_t
SeosCryptoApi_cipherFinalize(SeosCryptoCtx*                 ctx,
                             const SeosCrypto_CipherHandle  cipherHandle,
                             void*                          tag,
                             size_t*                        tagSize);

/** @} */
