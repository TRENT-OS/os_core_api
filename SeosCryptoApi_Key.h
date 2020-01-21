/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Key.h
 *
 * @brief SEOS Crypto API library types, constants and enums for Key object
 *
 */
#pragma once

#include "SeosError.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SeosCryptoApi_Key_SIZE_AES_MAX      32      ///< max 256 bit
#define SeosCryptoApi_Key_SIZE_AES_MIN      16      ///< min 128 bit
#define SeosCryptoApi_Key_SIZE_RSA_MAX      512     ///< max 4096 bit
#define SeosCryptoApi_Key_SIZE_RSA_MIN      16      ///< min 128 bit
#define SeosCryptoApi_Key_SIZE_DH_MAX       512     ///< max 4096 bit
#define SeosCryptoApi_Key_SIZE_DH_MIN       8       ///< min 64 bit
#define SeosCryptoApi_Key_SIZE_ECC          32      ///< always 256 bit

/**
 * These need to be set to these exact values to match values expected by the
 * implementation of the Crypto API.
 */
typedef enum
{
    SeosCryptoApi_Key_PARAM_NONE          = 0,
    SeosCryptoApi_Key_PARAM_ECC_SECP192R1 = 1,
    SeosCryptoApi_Key_PARAM_ECC_SECP224R1 = 2,
    SeosCryptoApi_Key_PARAM_ECC_SECP256R1 = 3,
} SeosCryptoApi_Key_Param;

typedef enum
{
    SeosCryptoApi_Key_SPECTYPE_NONE = 0,
    SeosCryptoApi_Key_SPECTYPE_BITS,
    SeosCryptoApi_Key_SPECTYPE_PARAMS,
} SeosCryptoApi_Key_SpecType;

typedef enum
{
    SeosCryptoApi_Key_FLAG_NONE                = 0,
}
SeosCryptoApi_Key_Flag;

typedef enum
{
    SeosCryptoApi_Key_TYPE_NONE = 0,
    SeosCryptoApi_Key_TYPE_AES,
    SeosCryptoApi_Key_TYPE_RSA_PRV,
    SeosCryptoApi_Key_TYPE_RSA_PUB,
    SeosCryptoApi_Key_TYPE_DH_PRV,
    SeosCryptoApi_Key_TYPE_DH_PUB,
    SeosCryptoApi_Key_TYPE_SECP256R1_PRV,
    SeosCryptoApi_Key_TYPE_SECP256R1_PUB,
    SeosCryptoApi_Key_TYPE_ECC_PRV,
    SeosCryptoApi_Key_TYPE_ECC_PUB
}
SeosCryptoApi_Key_Type;

typedef struct SeosCryptoLib_Key SeosCryptoLib_Key;
typedef struct
{
    SeosCryptoLib_Key* key;
    SeosCryptoApi_Impl impl;
} SeosCryptoApi_Key;

typedef struct
{
    uint8_t bytes[SeosCryptoApi_Key_SIZE_AES_MAX];
    uint32_t len;
}
SeosCryptoApi_Key_Aes;

typedef struct
{
    uint8_t nBytes[SeosCryptoApi_Key_SIZE_RSA_MAX]; ///< public modulus n=p*q
    uint32_t nLen;
    uint8_t eBytes[SeosCryptoApi_Key_SIZE_RSA_MAX]; ///< public exponent
    uint32_t eLen;
}
SeosCryptoApi_Key_RsaRub;

typedef struct
{
    uint8_t dBytes[SeosCryptoApi_Key_SIZE_RSA_MAX]; ///< secret exp.
    uint32_t dLen;
    uint8_t eBytes[SeosCryptoApi_Key_SIZE_RSA_MAX]; ///< public exp.
    uint32_t eLen;
    uint8_t pBytes[SeosCryptoApi_Key_SIZE_RSA_MAX / 2]; ///< prime factor of n
    uint32_t pLen;
    uint8_t qBytes[SeosCryptoApi_Key_SIZE_RSA_MAX / 2]; ///< prime factor of n
    uint32_t qLen;
}
SeosCryptoApi_Key_RsaRrv;

typedef struct
{
    uint8_t aBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< A of Weierstrass curve
    uint32_t aLen;
    uint8_t bBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< B of Weierstrass curve
    uint32_t bLen;
    uint8_t gxBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< coord x of basepoint G
    uint32_t gxLen;
    uint8_t gyBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< coord y of basepoint G
    uint32_t gyLen;
    uint8_t pBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< prime P of base field
    uint32_t pLen;
    uint8_t nBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< order of G
    uint32_t nLen;
}
SeosCryptoApi_Key_EccParams;

typedef struct
{
    SeosCryptoApi_Key_EccParams params; ///< params of curve: A, B, G, P, n=ord(G)
    uint8_t qxBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< x of point Q=P*d
    uint32_t qxLen;
    uint8_t qyBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< y of point Q=P*d
    uint32_t qyLen;
}
SeosCryptoApi_Key_EccPub;

typedef struct
{
    SeosCryptoApi_Key_EccParams params; ///< params of curve: A, B, G, P, n=ord(G)
    uint8_t dBytes[SeosCryptoApi_Key_SIZE_ECC];       ///<  private scalar
    uint32_t dLen;
}
SeosCryptoApi_Key_EccPrv;

/**
 * Public key for NIST Secp256r1 curve; does not need to carry the params as the
 * key type already defines everything.
 */
typedef struct
{
    uint8_t qxBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< x of point Q=P*d
    uint32_t qxLen;
    uint8_t qyBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< y of point Q=P*d
    uint32_t qyLen;
}
SeosCryptoApi_Key_Secp256r1Pub;

/**
 * Private key for NIST Secp256r1 curve; does not need to carry the params as the
 * key type already defines everything.
 */
typedef struct
{
    uint8_t dBytes[SeosCryptoApi_Key_SIZE_ECC]; ///<  private scalar
    uint32_t dLen;
}
SeosCryptoApi_Key_Secp256r1Prv;

typedef struct
{
    uint8_t pBytes[SeosCryptoApi_Key_SIZE_DH_MAX]; ///< shared prime
    uint32_t pLen;
    uint8_t gBytes[SeosCryptoApi_Key_SIZE_DH_MAX]; ///< shared generator
    uint32_t gLen;
}
SeosCryptoApi_Key_DhParams;

typedef struct
{
    SeosCryptoApi_Key_DhParams params; ///< shared params: generator G and prime P
    uint8_t gxBytes[SeosCryptoApi_Key_SIZE_DH_MAX]; ///< public key g^x mod p
    uint32_t gxLen;
}
SeosCryptoApi_Key_DhPub;

typedef struct
{
    SeosCryptoApi_Key_DhParams params; ///< shared params: generator G and prime P
    uint8_t xBytes[SeosCryptoApi_Key_SIZE_DH_MAX]; ///< private exponent
    uint32_t xLen;
}
SeosCryptoApi_Key_DhPrv;

typedef struct
{
    SeosCryptoApi_Key_Flag flags;
    bool exportable;
} SeosCryptoApi_Key_Attribs;

typedef struct
{
    SeosCryptoApi_Key_SpecType type;
    struct key
    {
        SeosCryptoApi_Key_Type type;
        SeosCryptoApi_Key_Attribs attribs;
        union
        {
            uint32_t bits;
            SeosCryptoApi_Key_EccParams ecc;
            SeosCryptoApi_Key_DhParams dh;
        } params;
    } key;
}
SeosCryptoApi_Key_Spec;

typedef struct
{
    SeosCryptoApi_Key_Type type;
    SeosCryptoApi_Key_Attribs attribs;
    union
    {
        union
        {
            SeosCryptoApi_Key_EccPrv prv;
            SeosCryptoApi_Key_EccPub pub;
        } ecc;
        union
        {
            SeosCryptoApi_Key_Secp256r1Prv prv;
            SeosCryptoApi_Key_Secp256r1Pub pub;
        } secp256r1;
        union
        {
            SeosCryptoApi_Key_DhPrv prv;
            SeosCryptoApi_Key_DhPub pub;
        } dh;
        union
        {
            SeosCryptoApi_Key_RsaRrv prv;
            SeosCryptoApi_Key_RsaRub pub;
        } rsa;
        SeosCryptoApi_Key_Aes aes;
    } data;
}
SeosCryptoApi_Key_Data;

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
 * - `SeosCryptoApi_Key_TYPE_RSA_PRV`:        RSA private key (128-4096 bits)
 * - `SeosCryptoApi_Key_TYPE_DH_PRV`:         DH private key (64-4096 bits)
 * - `SeosCryptoApi_Key_TYPE_SECP256R1_PRV`:  ECC private key for SECP256r1 curve
 * - `SeosCryptoApi_Key_TYPE_AES`:            AES key (128, 192, 256 bits)
 *
 * Here are some example specs for typical keys:
 * 1. Create a DH priv key with 101 bits, which is NOT exportable (this is the default
 *    value). Use a parameter spec which already provides the prime P and base G.
 *  \code{.c}
 *  static const SeosCryptoApi_Key_Spec dh101pSpec = {
 *      .type = SeosCryptoApi_Key_SPECTYPE_PARAMS,
 *      .key = {
 *          .type = SeosCryptoApi_Key_TYPE_DH_PRV,
 *          .params.dh = {
 *              .pBytes = {0x12, 0xdf, 0x4d, 0x76, ... 0x07},
 *              .pLen   = 13,
 *              .gBytes = {0x00, 0x1e, 0x32, 0x15, ... 0xd6},
 *              .gLen   = 13,
 *          }
 *      }
 *  };
 *  \endcode
 * 2. Create a 128 bit AES key and make it exportable, use a bit spec for that:
 *  \code{.c}
 *  static const SeosCryptoApi_Key_Spec aes128Spec = {
 *      .type = SeosCryptoApi_Key_SPECTYPE_BITS,
 *      .key = {
 *          .type = SeosCryptoApi_Key_TYPE_AES,
 *          .attribs.exportable = true,
 *          .params.bits = 128
 *      }
 *  };
 *  \endcode
 * 3. Create 1024-bit RSA privkey, again using a bit spec:
 *  \code{.c}
 *  static const SeosCryptoApi_Key_Spec rsa128Spec = {
 *     .type = SeosCryptoApi_Key_SPECTYPE_BITS,
 *     .key = {
 *        .type = SeosCryptoApi_Key_TYPE_RSA_PRV,
 *        .params.bits = 1024
 *      }
 *  };
 *  \endcode
 *
 * @param api (required) pointer to the seos crypto context
 * @param obj (required) pointer to Key object
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
SeosCryptoApi_Key_generate(
    SeosCryptoApi*                api,
    SeosCryptoApi_Key*            obj,
    const SeosCryptoApi_Key_Spec* spec);

/**
 * @brief Import key data into key object from buffer
 *
 * This function allocates a key object and imports key material from a buffer.
 *
 * During import, the sizes of the keys will be checked (e.g., based on the modulus
 * provided by RSA or based on the amounts of bytes passed for AES). The following
 * types and keysizes of the imported \p keyData are supported:
 * - `SeosCryptoApi_Key_TYPE_AES`:            AES key (128, 192, 256 bits)
 * - `SeosCryptoApi_Key_TYPE_RSA_PRV`:        RSA private key (128 - 4096 bits)
 * - `SeosCryptoApi_Key_TYPE_RSA_PUB`:        RSA public key (128 - 4096 bits)
 * - `SeosCryptoApi_Key_TYPE_DH_PRV`:         DH private key (64 - 4096 bits)
 * - `SeosCryptoApi_Key_TYPE_DH_PUB`:         DH public key (64 - 4096 bits)
 * - `SeosCryptoApi_Key_TYPE_SECP256R1_PRV`:  ECC private key for SECP256r1 curve
 * - `SeosCryptoApi_Key_TYPE_SECP256r1_PUB`:  ECC public key for SECP256r1 curve
 *
 * Here are some example key data configurations for typical types of keys:
 * 1. Define a 128-bit AES key that is exportable:
 *  \code{.c}
 *  static const SeosCryptoApi_Key_Data aes128Data =
 *  {
 *      .type = SeosCryptoApi_Key_TYPE_AES,
 *      .attribs.exportable = true,
 *      .data.aes = {
 *          .bytes  = "0123456789abcdef",
 *          .len    = 16
 *      }
 *  };
 *  \endcode
 * 2. Define a 256-bit AES key that is NOT exportable:
 *  \code{.c}
 *  static const SeosCryptoApi_Key_Data aes256Data =
 *  {
 *      .type = SeosCryptoApi_Key_TYPE_AES,
 *      .attribs.exportable = false,
 *      .data.aes = {
 *          .bytes  = "0123456789abcdef0123456789abcdef",
 *          .len    = 32
 *      }
 *  };
 *  \endcode
 * 3. Define a SECP256r1 private key that is NOT exportable (abbreviated):
 *  \code{.c}
 *  static const SeosCryptoApi_Key_Data secp256r1PrvData =
 *  {
 *      .type = SeosCryptoApi_Key_TYPE_SECP256R1_PRV,
 *      .attribs.exportable = false,
 *      .data.secp256r1.prv = {
 *          .dBytes = {0xc6, 0xef, 0x9c, 0x5d, ... 0x20},
 *          .dLen   = 32,
 *      }
 *  };
 *  \endcode
 * 4. Define 1024-bit RSA private key that is exportable (abbreviated):
 *  \code{.c}
 *  static const SeosCryptoApi_Key_Data rsa1024PrvData =
 *  {
 *      .type = SeosCryptoApi_Key_TYPE_RSA_PRV,
 *      .attribs.exportable = true,
 *      .data.rsa.prv = {
 *          .dBytes = {0x35, 0xe7, 0x4c, 0x80, ... 0x99},
 *          .dLen   = 128,
 *          .eBytes = {0x01, 0x00, 0x01},
 *          .eLen   = 3,
 *          .pBytes = {0xdd, 0x35, 0x19, 0x94, ... 0xa3},
 *          .pLen   = 64,
 *          .qBytes = {0xa9, 0x1e, 0xc2, 0x6b, ... 0xeb},
 *          .qLen   = 64,
 *      }
 *  };
 *  \endcode
 *
 * @param api (required) pointer to the seos crypto context
 * @param obj (required) pointer to the Key object
 * @param keyData (required) buffer for key material to import
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid;
 * this includes supplying \p keyData that has internal inconsistencies (e.g.
 * too long buffer lengths) or key size that do not match what is expected
 * for algorithms where it is discretely defined (e.g., 120 bit key for AES)
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the key failed
 */
seos_err_t
SeosCryptoApi_Key_import(
    SeosCryptoApi*                api,
    SeosCryptoApi_Key*            obj,
    const SeosCryptoApi_Key_Data* keyData);

/**
 * @brief Make a public key from a private key
 *
 * This function allocates a new key object and computes a public key based on
 * an existing private key. In order to make a keypair, generate() and makePublic()
 * have to be called in sequence.
 *
 * A public key can be computed based on a private key \p prvKeyHandle of this type:
 * - `SeosCryptoApi_Key_TYPE_RSA_PRV`:        RSA private key
 * - `SeosCryptoApi_Key_TYPE_DH_PRV`:         DH private key
 * - `SeosCryptoApi_Key_TYPE_SECP256R1_PRV`:  ECC private key for SECP256r1 curve
 *
 * @param obj (required) pointer to the Key object
 * @param prvObj (required) private Key object
 * @param attribs (required) attributes to assign to public key
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if the type of \p prvKeyHandle is not supported
 * \p bits is in an invalid range for those algorithms which accept a range (e.g. DH)
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of any of the keys failed
 */
seos_err_t
SeosCryptoApi_Key_makePublic(
    SeosCryptoApi_Key*               obj,
    const SeosCryptoApi_Key*         prvObj,
    const SeosCryptoApi_Key_Attribs* attribs);

/**
 * @brief Export key data from key handle into buffer
 *
 * If the key is exportable, this function will write the key material stored
 * in the object indicated by \p keyHandle to a buffer. If a key is not exportable,
 * this function will fail.
 *
 * @param obj (required) pointer to the Key object
 * @param keyData (required) buffer for key data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_OPERATION_DENIED if the key cannot be exported due attribs
 * set during creation of the key object
 */
seos_err_t
SeosCryptoApi_Key_export(
    const SeosCryptoApi_Key* obj,
    SeosCryptoApi_Key_Data*  keyData);

/**
 * @brief Get shared parameters from key
 *
 * Some key types have shared parameters, e.g., DHPrv and DHPub typically share
 * the prime P and base G. This function allows to read out these parameters. The
 * exportable flag is ignored here, as these are public parameters which may be
 * needed to generate more keys (e.g., in case of key exchange).
 *
 * @param obj (required) pointer to the Key object
 * @param param (required) buffer for key params
 * @param paramSize (required) buffer for key data, will be set to effectively
 * written bytes if function succeeds (or the minimum size if it fails due to too
 * small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if key has no exportable parameters
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p paramSize is too small to hold the
 * whole \p param
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p paramSize is greater than
 * `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Key_getParams(
    const SeosCryptoApi_Key* obj,
    void*                    param,
    size_t*                  paramSize);

/**
 * @brief Get attributes from key
 *
 * All keys have a set of attributes which can be extracted with this function.
 *
 * @param obj (required) pointer to the Key object
 * @param attribs (required) buffer for attributes
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Key_getAttribs(
    const SeosCryptoApi_Key*   obj,
    SeosCryptoApi_Key_Attribs* attribs);

/**
 * @brief Load pre-defined parameters
 *
 * For some protocols, it is possible to agree on a set of pre-defined parameters,
 * e.g. use SECP256r1 curve or a fixed DH group. This function allows to read those
 * parameters, so they can be used with a PARAM spec to generate keys.
 *
 * @param api (required) pointer to the seos crypto context
 * @param name (required) name of the parameter set
 * @param param (required) buffer for key params
 * @param paramSize (required) buffer for key data, will be set to effectively
 * written bytes if function succeeds (or the minimum size if it fails due to too
 * small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p name indicates an unknown parameter
 * set
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p paramSize is too small to hold the
 * whole \p param
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p paramSize is greater than
 * `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Key_loadParams(
    SeosCryptoApi*                api,
    const SeosCryptoApi_Key_Param name,
    void*                         param,
    size_t*                       paramSize);

/**
 * @brief Finish a key object
 *
 * This function frees the memory associated with the key object and zeroizes
 * any key material that was stored internally.
 *
 * @param obj (required) pointer to the Key object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Key_free(
    SeosCryptoApi_Key* obj);

/** @} */
