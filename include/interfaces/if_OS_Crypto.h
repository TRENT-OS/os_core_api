/*
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 */

#pragma once

#include "OS_Crypto.h"

#include <stdint.h>

typedef struct
{
    OS_Error_t (*Rng_getBytes)(unsigned int flags, size_t bufSize);
    OS_Error_t (*Rng_reseed)(size_t seedLen);
    OS_Error_t (*Mac_init)(OS_CryptoMac_Handle_t* pMacObj,
                           OS_CryptoKey_Handle_t keyObj, unsigned int algorithm);
    OS_Error_t (*Mac_free)(OS_CryptoMac_Handle_t macObj);
    OS_Error_t (*Mac_process)(OS_CryptoMac_Handle_t macObj, size_t dataSize);
    OS_Error_t (*Mac_finalize)(OS_CryptoMac_Handle_t macObj, size_t* macSize);
    OS_Error_t (*Digest_init)(OS_CryptoDigest_Handle_t* pDigestObj,
                              unsigned int algorithm);
    OS_Error_t (*Digest_clone)(OS_CryptoDigest_Handle_t* pDigestObj,
                               OS_CryptoDigest_Handle_t srcDigestObj);
    OS_Error_t (*Digest_free)(OS_CryptoDigest_Handle_t digestObj);
    OS_Error_t (*Digest_process)(OS_CryptoDigest_Handle_t digestObj, size_t inLen);
    OS_Error_t (*Digest_finalize)(OS_CryptoDigest_Handle_t digestObj,
                                  size_t* digestSize);
    OS_Error_t (*Key_generate)(OS_CryptoKey_Handle_t* pKeyObj);
    OS_Error_t (*Key_makePublic)(OS_CryptoKey_Handle_t* pPubKeyObj,
                                 OS_CryptoKey_Handle_t prvKeyObj);
    OS_Error_t (*Key_import)(OS_CryptoKey_Handle_t* pKeyObj);
    OS_Error_t (*Key_export)(OS_CryptoKey_Handle_t keyObj);
    OS_Error_t (*Key_getParams)(OS_CryptoKey_Handle_t keyObj, size_t* paramSize);
    OS_Error_t (*Key_getAttribs)(OS_CryptoKey_Handle_t keyObj);
    OS_Error_t (*Key_loadParams)(unsigned int param, size_t* paramSize);
    OS_Error_t (*Key_free)(OS_CryptoKey_Handle_t keyObj);
    OS_Error_t (*Signature_init)(OS_CryptoSignature_Handle_t* pSigObj,
                                 OS_CryptoKey_Handle_t prvObj, OS_CryptoKey_Handle_t pubObj,
                                 unsigned int algorithm, unsigned int digest);
    OS_Error_t (*Signature_verify)(OS_CryptoSignature_Handle_t sigObj,
                                   size_t hashSize, size_t signatureSize);
    OS_Error_t (*Signature_sign)(OS_CryptoSignature_Handle_t sigObj,
                                 size_t hashSize, size_t* signatureSize);
    OS_Error_t (*Signature_free)(OS_CryptoSignature_Handle_t sigObj);
    OS_Error_t (*Agreement_init)(OS_CryptoAgreement_Handle_t* pAgrObj,
                                 OS_CryptoKey_Handle_t prvObj, unsigned int algorithm);
    OS_Error_t (*Agreement_agree)(OS_CryptoAgreement_Handle_t agrObj,
                                  OS_CryptoKey_Handle_t pubObj, size_t* sharedSize);
    OS_Error_t (*Agreement_free)(OS_CryptoAgreement_Handle_t agrObj);
    OS_Error_t (*Cipher_init)(OS_CryptoCipher_Handle_t* pCipherObj,
                              OS_CryptoKey_Handle_t keyObj, unsigned int algorithm, size_t ivLen);
    OS_Error_t (*Cipher_free)(OS_CryptoCipher_Handle_t cipherObj);
    OS_Error_t (*Cipher_process)(OS_CryptoCipher_Handle_t cipherObj, size_t inLen,
                                 size_t* outSize);
    OS_Error_t (*Cipher_start)(OS_CryptoCipher_Handle_t cipherObj, size_t len);
    OS_Error_t (*Cipher_finalize)(OS_CryptoCipher_Handle_t cipherObj, size_t* len);
    OS_Dataport_t dataport;
} if_OS_Crypto_t;

#define IF_OS_CRYPTO_ASSIGN(_rpc_, _port_)              \
{                                                       \
    .Rng_getBytes       = _rpc_ ## _Rng_getBytes,       \
    .Rng_reseed         = _rpc_ ## _Rng_reseed,         \
    .Mac_init           = _rpc_ ## _Mac_init,           \
    .Mac_free           = _rpc_ ## _Mac_free,           \
    .Mac_process        = _rpc_ ## _Mac_process,        \
    .Mac_finalize       = _rpc_ ## _Mac_finalize,       \
    .Digest_init        = _rpc_ ## _Digest_init,        \
    .Digest_clone       = _rpc_ ## _Digest_clone,       \
    .Digest_free        = _rpc_ ## _Digest_free,        \
    .Digest_process     = _rpc_ ## _Digest_process,     \
    .Digest_finalize    = _rpc_ ## _Digest_finalize,    \
    .Key_generate       = _rpc_ ## _Key_generate,       \
    .Key_makePublic     = _rpc_ ## _Key_makePublic,     \
    .Key_import         = _rpc_ ## _Key_import,         \
    .Key_export         = _rpc_ ## _Key_export,         \
    .Key_getParams      = _rpc_ ## _Key_getParams,      \
    .Key_getAttribs     = _rpc_ ## _Key_getAttribs,     \
    .Key_loadParams     = _rpc_ ## _Key_loadParams,     \
    .Key_free           = _rpc_ ## _Key_free,           \
    .Signature_init     = _rpc_ ## _Signature_init,     \
    .Signature_verify   = _rpc_ ## _Signature_verify,   \
    .Signature_sign     = _rpc_ ## _Signature_sign,     \
    .Signature_free     = _rpc_ ## _Signature_free,     \
    .Agreement_init     = _rpc_ ## _Agreement_init,     \
    .Agreement_agree    = _rpc_ ## _Agreement_agree,    \
    .Agreement_free     = _rpc_ ## _Agreement_free,     \
    .Cipher_init        = _rpc_ ## _Cipher_init,        \
    .Cipher_free        = _rpc_ ## _Cipher_free,        \
    .Cipher_process     = _rpc_ ## _Cipher_process,     \
    .Cipher_start       = _rpc_ ## _Cipher_start,       \
    .Cipher_finalize    = _rpc_ ## _Cipher_finalize,    \
    .dataport           = OS_DATAPORT_ASSIGN(_port_)    \
}

