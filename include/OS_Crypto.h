/* Copyright (C) 2019-2020, Hensoldt Cyber GmbH */

/**
 * @file
 * @defgroup OS_Crypto OS Crypto API
 * @{
 * @brief OS Crypto API library
 */

#pragma once

#include "OS_Error.h"
#include "OS_Dataport.h"

#include "interfaces/if_OS_Entropy.h"

#include <stddef.h>

/**
 * The Crypto API can be instantiated in different modes, which basically determine
 * if a local or a remote instance of the underlying crypto library is used.
 */
typedef enum
{
    OS_Crypto_MODE_NONE = 0,

    /**
     * In this mode, the Crypto API uses only a library instance so all functions
     * called through the API are executed in the context of the calling component.
     */
    OS_Crypto_MODE_LIBRARY,

    /**
     * In this mode, the Crypto API uses only a RPC client instance so all functions
     * called through the API are forwarded to the RPC server and executed in
     * the context of this component.
     */
    OS_Crypto_MODE_CLIENT,

    /**
     * In this mode, the Crypto API has a library and a RPC client instance and
     * transparently switches between the local and remote instance:
     * - Crypto objects that use no keys are always executed in the local library.
     * - Key objects are allocated and handled in the local or remote library,
     *   depending on their "keepLocal" attribute.
     * - Crypto objects that use a key are allocated and handled wherever the
     *   associated key resides.
     */
    OS_Crypto_MODE_KEY_SWITCH,
} OS_Crypto_Mode_t;

/// @cond INTERNAL
//------------------------------------------------------------------------------
typedef struct OS_Crypto OS_Crypto_t;
typedef OS_Crypto_t* OS_Crypto_Handle_t;
typedef struct OS_Crypto_Object OS_Crypto_Object_t;
//------------------------------------------------------------------------------
/// @endcond

// Include all after definining the API handle above; also make sure that key and
// digest are included first so they are defined for the other functions.
//
// NOTE: These should never be included directly.
#include "crypto/OS_CryptoKey.h"
#include "crypto/OS_CryptoDigest.h"
#include "crypto/OS_CryptoAgreement.h"
#include "crypto/OS_CryptoCipher.h"
#include "crypto/OS_CryptoMac.h"
#include "crypto/OS_CryptoSignature.h"
#include "crypto/OS_CryptoRng.h"

// Include this after the respective OS_CryptoXXX_Handle_t are defined, which
// requires:
// 1. the definition of OS_Crypto_Object_t
// 2. the inclusion of crypto/OS_CryptoXXX.h
#include "interfaces/if_OS_Crypto.h"

/**
 * User of API can provide custom allocator functionality.
 */
typedef struct
{
    void* (*calloc)(size_t n, size_t size);
    void  (*free)(void* ptr);
} OS_Crypto_Memory_t;

/**
 * The Crypto API main configuration struct; first the mode needs to be set to
 * the desired value, then the respective sub-configuration must be filled in:
 *
 *                                     | cfg.memory | cfg.entropy |  cfg.rpc
 *   ----------------------------------+------------+-------------+-----------
 *              OS_Crypto_MODE_LIBRARY |      X     |      X      |
 *               OS_Crypto_MODE_CLIENT |      X     |             |    X
 *           OS_Crypto_MODE_KEY_SWITCH |      X     |      X      |    X
 *
 */
typedef struct
{
    /**
     * Mode the API should be operated in
     */
    OS_Crypto_Mode_t mode;

    /**
     * Optional calloc/free functions to use instead of standard ones
     */
    OS_Crypto_Memory_t memory;

    /**
     * All users of the Crypto API have to provide a platform-dependent
     * entropy function. This function will be called by the API's internal
     * DRBG to enrich its internal state with entropy.
     */
    if_OS_Entropy_t entropy;

    /**
     * If the API is in OS_Crypto_MODE_CLIENT/OS_Crypto_MODE_KEY_SWITCH mode,
     * use this interface to set to the remote instance's CAmkES RPC
     * functionality.
     */
    if_OS_Crypto_t rpc;
} OS_Crypto_Config_t;

/**
 * @brief Initialize the Crypto API
 *
 * This function allocates a Crypto API object and sets up Crypto API functionality
 * according to the configuration.
 *
 * @param hCrypto (required) pointer to handle of OS Crypto API
 * @param cfg (required) pointer to configuration
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if the internal state could not be initialized
 * @retval OS_ERROR_NOT_SUPPORTED if \p cfg is not supported
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of the API object failed
 */
OS_Error_t
OS_Crypto_init(
    OS_Crypto_Handle_t*       hCrypto,
    const OS_Crypto_Config_t* cfg);

/**
 * @brief Free a context associated with the Crypto API
 *
 * @param hCrypto (required) handle of OS Crypto API
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_Crypto_free(
    OS_Crypto_Handle_t hCrypto);

/// @cond INTERNAL
//------------------------------------------------------------------------------

/**
 * @brief Get pointer managed by proxy object
 *
 * If we have a Crypto API proxy object and we need to migrate it from one API
 * instance to another, we need to get the pointer to the underlying library object.
 *
 * NOTE: This function should only be used by an expert; underlying library objects
 *       should never be used directly, as this may create situations where multiple
 *       proxy objects point to the same library objects, which, in turn may lead
 *       to use-after-free and other issues!
 *
 * @param proxy (required) proxy object
 *
 * @return pointer to object or NULL of \p proxy was NULL
 */
void*
OS_Crypto_getProxyPtr(
    const OS_Crypto_Object_t* proxy);

/**
 * @brief Create a proxy object for an existing Crypto API instance
 *
 * Create a proxy object to wrap an Crypto API object and associate it with a
 * Crypto API instance. This would be used to be able to handle objects
 * belonging to a remote Crypto API instance through the local API instance
 * (when in client mode).
 *
 * NOTE: This function allocates a new object, which must be free'd with the
 *       appropriate function.
 *
 * NOTE: This function should only be used by an expert; underlying objects
 *       should never be used directly, as this may create situations where
 *       multiple proxy objects point to the same library objects, which, in
 *       turn may lead to use-after-free and other issues!
 *
 * @param proxy (required) pointer to proxy object
 * @param hCrypto (required) handle of OS Crypto API
 * @param ptr (required) pointer to the underlying object from some API instance
 * @param local (required) indicate if the object belongs to a local or
 *  remote instance of the crypto library
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_Crypto_createProxy(
    OS_Crypto_Object_t**       proxy,
    const OS_Crypto_Handle_t   self,
    const void*                ptr,
    const bool                 local);

/**
 * @brief Get mode of Crypto API instance.
 *
 * @param hCrypto (required) handle of OS Crypto API
 *
 * @return Mode of API or OS_Crypto_MODE_NONE if \p hCrypto is NULL
 */
OS_Crypto_Mode_t
OS_Crypto_getMode(
    const OS_Crypto_Handle_t hCrypto);

//------------------------------------------------------------------------------
/// @endcond

/** @} */
