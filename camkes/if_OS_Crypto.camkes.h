/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#pragma once

#define IF_OS_CRYPTO_CAMKES                             \
    OS_Error_t Rng_getBytes(                            \
        in unsigned int flags,                          \
        in size_t bufSize                               \
    );                                                  \
    OS_Error_t Rng_reseed(                              \
        in size_t seedLen                               \
    );                                                  \
    \
    OS_Error_t Mac_init(                                \
        inout OS_CryptoMac_Handle_t pMacHandle,         \
        in OS_CryptoKey_Handle_t keyHandle,             \
        in unsigned int algorithm                       \
    );                                                  \
    OS_Error_t Mac_free(                                \
        in OS_CryptoMac_Handle_t macHandle              \
    );                                                  \
    OS_Error_t Mac_process(                             \
        in OS_CryptoMac_Handle_t macHandle,             \
        in size_t dataSize                              \
    );                                                  \
    OS_Error_t Mac_finalize(                            \
        in OS_CryptoMac_Handle_t macHandle,             \
        inout size_t macSize                            \
    );                                                  \
    \
    OS_Error_t Digest_init(                             \
        inout OS_CryptoDigest_Handle_t pDigestHandle,   \
        in unsigned int algorithm                       \
    );                                                  \
    OS_Error_t Digest_clone(                            \
        inout OS_CryptoDigest_Handle_t pDigestHandle,   \
        in OS_CryptoDigest_Handle_t srcDigestHandle     \
    );                                                  \
    OS_Error_t Digest_free(                             \
        in OS_CryptoDigest_Handle_t digestHandle        \
    );                                                  \
    OS_Error_t Digest_process(                          \
        in OS_CryptoDigest_Handle_t digestHandle,       \
        in size_t inLen                                 \
    );                                                  \
    OS_Error_t Digest_finalize(                         \
        in OS_CryptoDigest_Handle_t digestHandle,       \
        inout size_t digestSize                         \
    );                                                  \
    \
    OS_Error_t Key_generate(                            \
        inout OS_CryptoKey_Handle_t pKeyHandle          \
    );                                                  \
    OS_Error_t Key_makePublic(                          \
        inout OS_CryptoKey_Handle_t pPubKeyHandle,      \
        in OS_CryptoKey_Handle_t prvKeyHandle           \
    );                                                  \
    OS_Error_t Key_import(                              \
        inout OS_CryptoKey_Handle_t pKeyHandle          \
    );                                                  \
    OS_Error_t Key_export(                              \
        in OS_CryptoKey_Handle_t keyHandle              \
    );                                                  \
    OS_Error_t Key_getParams(                           \
        in OS_CryptoKey_Handle_t keyHandle,             \
        inout size_t paramSize                          \
    );                                                  \
    OS_Error_t Key_getAttribs(                          \
        in OS_CryptoKey_Handle_t keyHandle              \
    );                                                  \
    OS_Error_t Key_loadParams(                          \
        in unsigned int param,                          \
        inout size_t paramSize                          \
    );                                                  \
    OS_Error_t Key_free(                                \
        in OS_CryptoKey_Handle_t keyHandle              \
    );                                                  \
    \
    OS_Error_t Signature_init(                          \
        inout OS_CryptoSignature_Handle_t pSigHandle,   \
        in OS_CryptoKey_Handle_t prvHandle,             \
        in OS_CryptoKey_Handle_t pubHandle,             \
        in unsigned int algorithm,                      \
        in unsigned int digest                          \
    );                                                  \
    OS_Error_t Signature_verify(                        \
        in OS_CryptoSignature_Handle_t sigHandle,       \
        in size_t hashSize,                             \
        in size_t signatureSize                         \
    );                                                  \
    OS_Error_t Signature_sign(                          \
        in OS_CryptoSignature_Handle_t sigHandle,       \
        in size_t hashSize,                             \
        inout size_t signatureSize                      \
    );                                                  \
    OS_Error_t Signature_free(                          \
        in OS_CryptoSignature_Handle_t sigHandle        \
    );                                                  \
    \
    OS_Error_t Agreement_init(                          \
        inout OS_CryptoAgreement_Handle_t pAgrHandle,   \
        in OS_CryptoKey_Handle_t prvHandle,             \
        in unsigned int algorithm,                      \
    );                                                  \
    OS_Error_t Agreement_agree(                         \
        in OS_CryptoAgreement_Handle_t agrHandle,       \
        in OS_CryptoKey_Handle_t pubHandle,             \
        inout size_t sharedSize                         \
    );                                                  \
    OS_Error_t Agreement_free(                          \
        in OS_CryptoAgreement_Handle_t agrHandle        \
    );                                                  \
    \
    OS_Error_t Cipher_init(                             \
        inout OS_CryptoCipher_Handle_t pCipherHandle,   \
        in OS_CryptoKey_Handle_t keyHandle,             \
        in unsigned int algorithm,                      \
        in size_t ivLen                                 \
    );                                                  \
    OS_Error_t Cipher_free(                             \
        in OS_CryptoCipher_Handle_t cipherHandle        \
    );                                                  \
    OS_Error_t Cipher_process(                          \
        in OS_CryptoCipher_Handle_t cipherHandle,       \
        in size_t inLen,                                \
        inout size_t outSize                            \
    );                                                  \
    OS_Error_t Cipher_start(                            \
        in OS_CryptoCipher_Handle_t cipherHandle,       \
        in size_t len                                   \
    );                                                  \
    OS_Error_t Cipher_finalize(                         \
        in OS_CryptoCipher_Handle_t cipherHandle,       \
        inout size_t len                                \
    );