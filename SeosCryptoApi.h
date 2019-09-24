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

// -------------------------------- RNG API ------------------------------------

/**
 * @brief Generate random numbers
 *
 */
seos_err_t
SeosCryptoApi_rngGetBytes(SeosCryptoCtx*        ctx,
                          const unsigned int    flags,
                          void*                 buf,
                          const size_t          bufSize);

/**
 * @brief Reseed the internal RNG
 *
 */
seos_err_t
SeosCryptoApi_rngReSeed(SeosCryptoCtx*      ctx,
                        const void*         seed,
                        const size_t        seedLen);

// ------------------------------ Digest API -----------------------------------

/**
 * @brief Initialize a digest object
 *
 */
seos_err_t
SeosCryptoApi_digestInit(SeosCryptoCtx*             ctx,
                         SeosCrypto_DigestHandle*   pDigestHandle,
                         const unsigned int         algorithm);

/**
 * @brief Finish a digest object
 *
 */
seos_err_t
SeosCryptoApi_digestFree(SeosCryptoCtx*                ctx,
                         const SeosCrypto_DigestHandle digestHandle);

/**
 * @brief Add data to a digest
 *
 */
seos_err_t
SeosCryptoApi_digestUpdate(SeosCryptoCtx*                   ctx,
                           const SeosCrypto_DigestHandle    digestHandle,
                           const void*                      data,
                           const size_t                     dataLen);

/**
 * @brief Write resulting digest to buffer
 *
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
 */
seos_err_t
SeosCryptoApi_keyInit(SeosCryptoCtx*                   ctx,
                      SeosCrypto_KeyHandle*            keyHandle,
                      const unsigned int               type,
                      const unsigned int               flags,
                      const size_t                     bits);

/**
 * @brief Generate key with internal RNG
 *
 */
seos_err_t
SeosCryptoApi_keyGenerate(SeosCryptoCtx*               ctx,
                          const SeosCrypto_KeyHandle   keyHandle);

/**
 * @brief Generate a private/public keypair with internal RNG
 *
 */
seos_err_t
SeosCryptoApi_keyGeneratePair(SeosCryptoCtx*                ctx,
                              const SeosCrypto_KeyHandle    prvKeyHandle,
                              const SeosCrypto_KeyHandle    pubKeyHandle);

/**
 * @brief Import key data into key object from buffer
 *
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
 */
seos_err_t
SeosCryptoApi_keyFree(SeosCryptoCtx*                ctx,
                      const SeosCrypto_KeyHandle    keyHandle);

// ----------------------------- Signature API ---------------------------------

/**
 * @brief Initialize a signature object
 *
 */
seos_err_t
SeosCryptoApi_signatureInit(SeosCryptoCtx*                ctx,
                            SeosCrypto_SignatureHandle*   pSigHandle,
                            const unsigned int            algorithm,
                            const SeosCrypto_KeyHandle    prvHandle,
                            const SeosCrypto_KeyHandle    pubHandle);

/**
 * @brief Finish a signature object
 *
 */
seos_err_t
SeosCryptoApi_signatureFree(SeosCryptoCtx*                      ctx,
                            const SeosCrypto_SignatureHandle    sigHandle);

/**
 * @brief Sign a hash value
 *
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
 * @brief Initialize an (key) agreement object
 *
 */
seos_err_t
SeosCryptoApi_agreementInit(SeosCryptoCtx*                ctx,
                            SeosCrypto_AgreementHandle*   pAgrHandle,
                            const unsigned int            algorithm,
                            const SeosCrypto_KeyHandle    prvHandle);

/**
 * @brief Finish a (key) agreement object
 *
 */
seos_err_t
SeosCryptoApi_agreementFree(SeosCryptoCtx*                      ctx,
                            const SeosCrypto_AgreementHandle    agrHandle);

/**
 * @brief Agree on a shared key
 *
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
 */
seos_err_t
SeosCryptoApi_cipherInit(SeosCryptoCtx*                 ctx,
                         SeosCrypto_CipherHandle*       pCipherHandle,
                         const unsigned int             algorithm,
                         const SeosCrypto_KeyHandle     keyHandle,
                         const void*                    iv,
                         const size_t                   ivLen);

/**
 * @brief Finish cipher object
 *
 */
seos_err_t
SeosCryptoApi_cipherFree(SeosCryptoCtx*                 ctx,
                         const SeosCrypto_CipherHandle  cipherHandle);

/**
 * @brief Encrypt data in chunks
 *
 */
seos_err_t
SeosCryptoApi_cipherUpdate(SeosCryptoCtx*                   ctx,
                           const SeosCrypto_CipherHandle    cipherHandle,
                           const void*                      data,
                           const size_t                     dataLen,
                           void*                            output,
                           size_t*                          outputSize);

/**
 * @brief Start encryption of data (only relevant for some algorithms)
 *
 */
seos_err_t
SeosCryptoApi_cipherStart(SeosCryptoCtx*                ctx,
                          const SeosCrypto_CipherHandle cipherHandle,
                          const void*                   data,
                          const size_t                  dataLen);

/**
 * @brief Finish encryption of data (only relevant for some algorithms)
 *
 */
seos_err_t
SeosCryptoApi_cipherFinalize(SeosCryptoCtx*                 ctx,
                             const SeosCrypto_CipherHandle  cipherHandle,
                             void*                          output,
                             size_t*                        outputSize);

/** @} */
