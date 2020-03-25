/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup SeosCryptoApi SEOS Crypto API
 * @{
 *
 * @file SeosCryptoApi_Key.h
 *
 * @brief SEOS Crypto API library types, constants and enums for KEY object
 *
 */

#pragma once

#include "SeosError.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "compiler.h"

/**
 * Maximum sizes supported for the respective key types.
 */
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
    SeosCryptoApi_Key_PARAM_NONE = 0,
    SeosCryptoApi_Key_PARAM_ECC_SECP192R1,
    SeosCryptoApi_Key_PARAM_ECC_SECP224R1,
    SeosCryptoApi_Key_PARAM_ECC_SECP256R1
} SeosCryptoApi_Key_Param;

/**
 * Type of key generation spec.
 */
typedef enum
{
    SeosCryptoApi_Key_SPECTYPE_NONE = 0,
    /**
     * KeySpec defines the amount of desired bits for a newly generated key directly.
     */
    SeosCryptoApi_Key_SPECTYPE_BITS,
    /**
     * KeySpec defines target params of newly generated keys, thus its size is
     * defined by those parameters (e.g., in case of DH keys, the size of the
     * underlying prime P).
     */
    SeosCryptoApi_Key_SPECTYPE_PARAMS,
} SeosCryptoApi_Key_SpecType;

/**
 * Special flags to use for KEY object.
 */
typedef enum
{
    SeosCryptoApi_Key_FLAG_NONE = 0,
}
SeosCryptoApi_Key_Flag;

/**
 * Type of KEY object.
 */
typedef enum
{
    SeosCryptoApi_Key_TYPE_NONE = 0,
    /**
     * Key for use with AES encryption/decryption; can be 128, 192, 256 bits.
     */
    SeosCryptoApi_Key_TYPE_AES,
    /**
     * Key for use with RSA private operations (signature/decryption); can be
     * 128-4096 bits.
     */
    SeosCryptoApi_Key_TYPE_RSA_PRV,
    /**
     * Key for use with RSA public operations (verification/encryption); can be
     * 128-4096 bits.
     */
    SeosCryptoApi_Key_TYPE_RSA_PUB,
    /**
     * Key for use with DH private operations; can be 64-4096 bits.
     */
    SeosCryptoApi_Key_TYPE_DH_PRV,
    /**
     * Key for use with DH public operations; can be 64-4096 bits.
     */
    SeosCryptoApi_Key_TYPE_DH_PUB,
    /**
     * Key on SECP256r1 Elliptic Curve for private operations; can only be 256 bits.
     */
    SeosCryptoApi_Key_TYPE_SECP256R1_PRV,
    /**
     * Key on SECP256r1 Elliptic Curve for public operations; can only be 256 bits.
     */
    SeosCryptoApi_Key_TYPE_SECP256R1_PUB,
    /**
     * Key on generic Elliptic Curve for private operations; currently not used.
     */
    SeosCryptoApi_Key_TYPE_ECC_PRV,
    /**
     * Key on generic Elliptic Curve for public operations; currently not used.
     */
    SeosCryptoApi_Key_TYPE_ECC_PUB
}
SeosCryptoApi_Key_Type;

/**
 * Handle for SEOS Crypto API KEY objects.
 */
typedef SeosCryptoApi_Proxy* SeosCryptoApi_KeyH;

/**
 * Struct for an AES Key.
 */
typedef struct
{
    uint8_t bytes[SeosCryptoApi_Key_SIZE_AES_MAX];
    uint32_t len;
}
SeosCryptoApi_Key_Aes;

/**
 * Struct for RSA public key data.
 */
typedef struct
{
    uint8_t nBytes[SeosCryptoApi_Key_SIZE_RSA_MAX]; ///< public modulus n=p*q
    uint32_t nLen;
    uint8_t eBytes[SeosCryptoApi_Key_SIZE_RSA_MAX]; ///< public exponent
    uint32_t eLen;
}
SeosCryptoApi_Key_RsaRub;

/**
 * Struct for RSA private key data.
 */
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

/**
 * Struct for shared, generic ECC curve params in Weirstrass form.
 */
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

/**
 * Struct for generic ECC public key data.
 */
typedef struct
{
    SeosCryptoApi_Key_EccParams params; ///< params of curve: A, B, G, P, n=ord(G)
    uint8_t qxBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< x of point Q=P*d
    uint32_t qxLen;
    uint8_t qyBytes[SeosCryptoApi_Key_SIZE_ECC]; ///< y of point Q=P*d
    uint32_t qyLen;
}
SeosCryptoApi_Key_EccPub;

/**
 * Struct for generic ECC private key data.
 */
typedef struct
{
    SeosCryptoApi_Key_EccParams params; ///< params of curve: A, B, G, P, n=ord(G)
    uint8_t dBytes[SeosCryptoApi_Key_SIZE_ECC];       ///<  private scalar
    uint32_t dLen;
}
SeosCryptoApi_Key_EccPrv;

/**
 * Struct for SECP256r1 ECC public key data; does not need to carry the params
 * of the curve, as the key type already defines everything.
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
 * Struct for SECP256r1 ECC private key data; does not need to carry the params
 * of the curve, as the key type already defines everything.
 */
typedef struct
{
    uint8_t dBytes[SeosCryptoApi_Key_SIZE_ECC]; ///<  private scalar
    uint32_t dLen;
}
SeosCryptoApi_Key_Secp256r1Prv;

/**
 * Struct for shared DH parameters.
 */
typedef struct
{
    uint8_t pBytes[SeosCryptoApi_Key_SIZE_DH_MAX]; ///< shared prime
    uint32_t pLen;
    uint8_t gBytes[SeosCryptoApi_Key_SIZE_DH_MAX]; ///< shared generator
    uint32_t gLen;
}
SeosCryptoApi_Key_DhParams;

/**
 * Struct for DH public key data.
 */
typedef struct
{
    SeosCryptoApi_Key_DhParams params; ///< shared params: generator G and prime P
    uint8_t gxBytes[SeosCryptoApi_Key_SIZE_DH_MAX]; ///< public key g^x mod p
    uint32_t gxLen;
}
SeosCryptoApi_Key_DhPub;

/**
 * Struct for DH private key data.
 */
typedef struct
{
    SeosCryptoApi_Key_DhParams params; ///< shared params: generator G and prime P
    uint8_t xBytes[SeosCryptoApi_Key_SIZE_DH_MAX]; ///< private exponent
    uint32_t xLen;
}
SeosCryptoApi_Key_DhPrv;

/**
 * Struct for attributes associated with every key.
 */
typedef struct
{
    /**
     * Flags set for this key.
     */
    SeosCryptoApi_Key_Flag flags;
    /**
     * Keys can be EXPORTABLE or NOT_EXPORTABLE. This flag is evaluated in two
     * places:
     * - The RPC Server interface checks if a key can be exported, before passing
     *   key material to the RPC client.
     * - When a Crypto API instance is configured in ROUTER mode, this flag is
     *   evaluated to determine whether a Key can be used locally or needs to be
     *   handled by a RPC server instance (e.g., the CryptoServer).
     */
    bool exportable;
} SeosCryptoApi_Key_Attribs;

/**
 * Specification for the generation of new secret keys (e.g., AES keys or private
 * keys of keypairs). The type determines, which of the fields of the union need
 * to be set:
 * - SPECTYPE_BITS:     params.bits
 * - SPECTYPE_PARAMS:   params.ecc or params.dh (depends on key.type)
 */
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

/**
 * Specification of a key's raw data. Which fields of the union need to be set
 * depends on the type:
 * - TYPE_AES:      key.aes
 * - TYPE_RSA_PUB:  key.rsa.pub
 * - TYPE_RSA_PRV:  key.rsa.prv
 * etc.
 */
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
 * @brief Generate symmetric/private KEY with internal RNG.
 *
 * This function allocates a KEY object and fills it with KEY material generated
 * by the internal RNG of the Crypto API.
 *
 * This function takes a \p spec parameter, which defines what type of key is
 * generated and wich attributes the resulting key has.
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
 * @param hKey (required) pointer to handle of SEOS Crypto KEY object
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param spec (required) specification of key to create
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid, this
 *  includes giving invalid keysizes where these are fixed (e.g., 100 bits for AES)
 * @retval SEOS_ERROR_NOT_SUPPORTED if the spec type, the key type or the amount of
 *  key bits given by the spec is in an unsupported range (e.g. DH key with 40 bits)
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the key failed
 * @retval SEOS_ERROR_ABORTED if an internal error occured during cryptographic
 *  operations
 */
seos_err_t
SeosCryptoApi_Key_generate(
    SeosCryptoApi_KeyH*           hKey,
    const SeosCryptoApiH          hCrypto,
    const SeosCryptoApi_Key_Spec* spec);

/**
 * @brief Import data into KEY object from buffer.
 *
 * This function allocates a KEY object and imports KEY material from a buffer.
 *
 * During import, the sizes of the keys will be checked (e.g., based on the modulus
 * provided by RSA or based on the amounts of bytes passed for AES).
 *
 * Here are some example key data configurations for typical types of keys:
 * 1. Define a 128-bit AES key that is exportable:
 *  \code{.c}
 *  static const SeosCryptoApi_Key_Data aes128Data =
 *  {
 *      .type = SeosCryptoApi_Key_TYPE_AES,
 *      .attribs.exportable = true,
 *      .data.aes.bytes  = "0123456789abcdef",
 *      .data.aes.len    = 16
  *  };
 *  \endcode
 * 2. Define a SECP256r1 private key that is NOT exportable (abbreviated):
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
 * 3. Define 1024-bit RSA private key that is exportable (abbreviated):
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
 * @param hKey (required) pointer to handle of SEOS Crypto KEY object
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param keyData (required) buffer for key material to import
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid;
 *  this includes supplying \p keyData that has internal inconsistencies (e.g.
 *  too long buffer lengths) or key size that do not match what is expected
 *  for algorithms where it is discretely defined (e.g., 120 bit key for AES)
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the key failed
 */
seos_err_t
SeosCryptoApi_Key_import(
    SeosCryptoApi_KeyH*           hKey,
    const SeosCryptoApiH          hCrypto,
    const SeosCryptoApi_Key_Data* keyData);

/**
 * @brief Make a public KEY from a private KEY object.
 *
 * This function allocates a new KEY object and computes a public key based on
 * an existing private KEY object. In order to make a keypair, generate() and
 * makePublic() have to be called in sequence.
 *
 * @param hPubKey (required) pointer to handle of SEOS Crypto KEY object
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param hPrvKey (required) handle of SEOS Crypto KEY object to make public key for
 * @param attribs (required) attributes to assign to public key
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if the type of \p hPrvKey is not supported
 * \p bits is in an invalid range for those algorithms which accept a range (e.g. DH)
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of any of the keys failed
 */
seos_err_t
SeosCryptoApi_Key_makePublic(
    SeosCryptoApi_KeyH*              hPubKey,
    const SeosCryptoApiH             hCrypto,
    const SeosCryptoApi_KeyH         hPrvKey,
    const SeosCryptoApi_Key_Attribs* attribs);

/**
 * @brief Finish a KEY object.
 *
 * This function frees the memory associated with the KEY object and zeroizes
 * any sensitive material that was stored internally.
 *
 * @param hKey (required) handle of SEOS Crypto KEY object
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Key_free(
    SeosCryptoApi_KeyH hKey);

/**
 * @brief Export data from KEY handle into buffer.
 *
 * If a KEY object is in the local address space (Crypto API used in LIB mode)
 * or if it has the exportable attribute set, this function will export the
 * associated key material into a buffer.
 *
 * If a KEY is not exportable AND held in a remote Crypto API instance (e.g., the
 * CryptoServer) this function will fail.
 *
 * NOTE: It is intentional that a KEY obejct which is not exportable but used only
 *       in a local library instance will be exported, IGNORING the exportable
 *       attribute. The rationale behind this is that it resides in the callers
 *       address space anyways so he might equally well read the data himself.
 *
 * @param hKey (required) handle of SEOS Crypto KEY object
 * @param keyData (required) buffer for key data
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_OPERATION_DENIED if the key cannot be exported due attribs
 *  set during creation of the key object
 */
seos_err_t
SeosCryptoApi_Key_export(
    const SeosCryptoApi_KeyH hKey,
    SeosCryptoApi_Key_Data*  keyData);

/**
 * @brief Get shared parameters from KEY.
 *
 * Some key types have shared parameters, e.g., DHPrv and DHPub typically share
 * the prime P and base G. This function allows to read out these parameters. The
 * exportable flag is ignored here, as these are public parameters which may be
 * needed to generate more keys (e.g., in case of key exchange).
 *
 * @param hKey (required) handle of SEOS Crypto KEY object
 * @param param (required) buffer for key params
 * @param paramSize (required) buffer for key data, will be set to effectively
 *  written bytes if function succeeds (or the minimum size if it fails due to too
 *  small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if key has no exportable parameters
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p paramSize is too small to hold the
 *  whole \p param
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p paramSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Key_getParams(
    const SeosCryptoApi_KeyH hKey,
    void*                    param,
    size_t*                  paramSize);

/**
 * @brief Get attributes from KEY object.
 *
 * All KEYs have a set of attributes which can be extracted with this function, e.g,
 * to check if a KEY object can be exported.
 *
 * @param hKey (required) handle of SEOS Crypto KEY object
 * @param attribs (required) buffer for attributes
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_Key_getAttribs(
    const SeosCryptoApi_KeyH   hKey,
    SeosCryptoApi_Key_Attribs* attribs);

/**
 * @brief Load pre-defined parameters.
 *
 * For some protocols, it is possible to agree on a set of pre-defined parameters,
 * e.g. use SECP256r1 curve or a fixed DH group. This function allows to read those
 * parameters, so they can be used with a PARAM spec to generate keys.
 *
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param name (required) name of the parameter set
 * @param param (required) buffer for key params
 * @param paramSize (required) buffer for key data, will be set to effectively
 *  written bytes if function succeeds (or the minimum size if it fails due to too
 *  small buffer)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p name indicates an unknown parameter set
 * @retval SEOS_ERROR_BUFFER_TOO_SMALL if \p paramSize is too small to hold the
 *  whole \p param
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p paramSize is greater than
 *  `SeosCryptoApi_SIZE_DATAPORT`
 */
seos_err_t
SeosCryptoApi_Key_loadParams(
    SeosCryptoApiH                hCrypto,
    const SeosCryptoApi_Key_Param name,
    void*                         param,
    size_t*                       paramSize);

/** @} */