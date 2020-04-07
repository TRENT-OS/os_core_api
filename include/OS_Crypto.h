/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup OS_Crypto OS Crypto API
 * @{
 *
 * @file OS_Crypto.h
 *
 * @brief OS Crypto API library
 *
 */

#pragma once

#include "SeosError.h"

#include <limits.h>
#include <stddef.h>

/**
 * Fixed size of dataport, will be replaced with a more generic and self-contained
 * solution in the future.
 */
#define OS_Crypto_SIZE_DATAPORT PAGE_SIZE

typedef enum
{
    OS_Crypto_MODE_NONE = 0,
    OS_Crypto_MODE_LIBRARY,
    OS_Crypto_MODE_RPC_CLIENT,
    OS_Crypto_MODE_RPC_SERVER_WITH_LIBRARY,
    OS_Crypto_MODE_ROUTER,
} OS_Crypto_Mode_t;

typedef struct OS_Crypto OS_Crypto_t;
/**
 * Handle to an API context.
 */
typedef OS_Crypto_t* OS_Crypto_Handle_t;
/**
 * All underlying library objects are encapsulated via a proxy object, to which
 * ultimately all API object handles point.
 */
typedef struct OS_Crypto_Object OS_Crypto_Object_t;
/**
 *  Pointer to (MAC, Digest, etc.) objects managed by library.
 */
typedef void* CryptoLib_Object_ptr;

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

typedef void* (OS_Crypto_Malloc_func)(
    size_t size);
typedef void (OS_Crypto_Free_func)(
    void* ptr);

/**
 * User of API can provide custom allocator/free functionality.
 */
typedef struct
{
    OS_Crypto_Malloc_func* malloc;
    OS_Crypto_Free_func* free;
}
OS_Crypto_Memory_t;

/**
 * Configuration for underlying Crypto Library.
 */
typedef struct
{
    struct
    {
        /**
         * All users of the Crypto API have to provide a platform-dependent
         * entropy function. This function will be called by the API's internal
         * DRBG to enrich its internal state with entropy.
         *
         * NOTE: Currently, the entropy callback will be called for every call
         *       (external and internal) to the RNG.
         */
        OS_CryptoRng_Entropy_func* entropy;
        /**
         * Context to pass to entropy callback.
         */
        void* context;
    } rng;
} CryptoLib_Config_t;

/**
 * Configuration for Crypto API in RPC Client mode.
 */
typedef struct
{
    /**
     * Dataport to use for the Crypto API in RPC Client mode to communicate with
     * an API instance in RPC Server mode.
     */
    void* dataPort;
} CryptoLibClient_Config_t;

/**
 * Configuration for Crypto API in RPC Server mode (RPC Server + Library).
 */
typedef struct
{
    /**
     * Dataport to use for the Crypto API in RPC Server mode to communicate with
     * an API instance in RPC Client mode.
     */
    void* dataPort;
} CryptoLibServer_Config_t;

/**
 * Configuration for Crypto API in Router mode; since the router switches between
 * the local librara and a remote instance seamlessly, it needs a configuration
 * for both.
 */
typedef struct
{
    /**
     * Configuration of locally used Library instance (used for keys that are
     * flagged as exportable).
     */
    CryptoLibClient_Config_t client;
    /**
     * Configuration of remote RPC Server instance of the Crypto API.
     */
    CryptoLib_Config_t lib;
} CryptoLibRouter_Config_t;

/**
 * The Crypto API main configuration struct; first the mode needs to be set to
 * the desired value, then the respective sub-configuration must be filled in:
 * -  OS_Crypto_MODE_LIBRARY:                   impl.lib
 * -  OS_Crypto_MODE_RPC_CLIENT:                impl.client
 * -  OS_Crypto_MODE_ROUTER:                    impl.router
 * -  OS_Crypto_MODE_RPC_SERVER_WITH_LIBRARY:   impl.lib AND server
 */
typedef struct
{
    OS_Crypto_Mode_t mode;
    OS_Crypto_Memory_t mem;
    union
    {
        CryptoLib_Config_t lib;
        CryptoLibClient_Config_t client;
        CryptoLibRouter_Config_t router;
    } impl;
    CryptoLibServer_Config_t server;
} OS_Crypto_Config_t;

/**
 * @brief Initialize the Crypto API
 *
 * This function allocates a Crypto API object and sets up Crypto API functionality
 * according to the configuration.
 *
 * The API can be operated in four modes:
 * - Library:     Cryptographic library functionality can be used directly through
 *                calls to the API.
 * - RPC Client:  Calls to API functions are relayed to the RPC server, where they
 *                are executed by an implementation in the context of that component.
 * - RPC Server:  The API instance acts as RPC server for processing Crypto API
 *                calls. Such an API instance can be used in the component instantiating
 *                it as if it where a library; the remote side has to go through
 *                a Crypto API instance configured as RPC Client.
 * - Router:      In this mode, the Crypto API instance will "route" calls either
 *                to a local instance of the library, or to a remote instance. The
 *                decision is made base on the "exportable" flag of cryptographic
 *                keys -- either when the key is created or when it is used.
 *
 * Here are some example configurations for each of these modes:
 * 1. Run the API instance as local instance of the Crypto library:
 *  \code{.c}
 *  OS_Crypto_Config_t cfgLocal =  {
 *      .mode = OS_Crypto_MODE_LIBRARY,
 *      .mem = {
 *          .malloc = malloc,
 *          .free = free,
 *      },
 *      .impl.lib.rng.entropy = entropy_func,
 *  };
 *  \endcode
 * 2. Run the API instance as RPC client, which executes all Crypto functionality
 *    inside of an RPC server component:
 *  \code{.c}
 *  OS_Crypto_Config_t cfgRemote = {
 *      .mode = OS_Crypto_MODE_RPC_CLIENT,
 *      .mem = {
 *          .malloc = malloc,
 *          .free = free,
 *      },
 *      .impl.client.dataPort = clientDataport
 *  };
 *  \endcode
 * 3. Run API instance as RPC server backend:
 *  \code{.c}
 *  OS_Crypto_Config_t cfgServer = {
 *      .mode = OS_Crypto_MODE_RPC_SERVER_WITH_LIBRARY,
 *      .mem = {
 *          .malloc = malloc,
 *          .free   = free,
 *      },
 *      .impl.lib.rng.entropy = entropy_func,
 *      .server.dataPort = serverDataport
 *  };
 *  \endcode
 * 4. Run API instance in ROUTER mode:
 *  \code{.c}
 *  OS_Crypto_Config_t cfgRouter = {
 *      .mode = OS_Crypto_MODE_ROUTER,
 *      .mem = {
 *          .malloc = malloc,
 *          .free   = free,
 *      },
 *      .impl.lib.rng.entropy = entropy_func,
 *      .impl.client.dataPort = clientDataport
 *  };
 *  \endcode
 *
 * All API configurations must pass function pointers to malloc/free, those involved
 * with RPC functionality must set the dataport configuration according to those
 * addresses assigned by CAmKES.
 *
 * @param hCrypto (required) pointer to handle of OS Crypto API
 * @param cfg (required) pointer to configuration
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval SEOS_ERROR_ABORTED if the internal state could not be initialized
 * @retval SEOS_ERROR_NOT_SUPPORTED if \p cfg is not supported
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if allocation of the API object failed
 */
seos_err_t
OS_Crypto_init(
    OS_Crypto_Handle_t*       hCrypto,
    const OS_Crypto_Config_t* cfg);

/**
 * @brief Free a context associated with the Crypto API
 *
 * @param hCrypto (required) handle of OS Crypto API
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
OS_Crypto_free(
    OS_Crypto_Handle_t hCrypto);

/**
 * @brief Get pointer to library object.
 *
 * If we have a Crypto API proxy object and we need to migrate it from one API
 * instance to another, we need to get the pointer to the underlying library object.
 *
 * NOTE: This function should only be used by an expert; underlying library objects
 *       should never be used directly, as this may create situations where multiple
 *       proxy objects point to the same library objects, which, in turn may lead
 *       to use-after-free and other issues!
 *
 * @param proxy (required) handle of OS Crypto Key object, e.g.,
 * OS_CryptoMac_Handle_t, OS_CryptoDigest_Handle_t, etc.
 *
 * @return pointer to object or NULL of \p proxy was NULL
 */
CryptoLib_Object_ptr*
OS_Crypto_getObject(
    const OS_Crypto_Object_t* proxy);

/**
 * @brief Migrate a library object from a one API instance to another instance.
 *
 * With the RPC functionality of the Crypto API it is possible to have an RPC
 * server with an instance of the library in one component and an RPC client in
 * another component that uses the same library.
 *
 * Now, if we are on the RPC client side and get a pointer to a Key object of the
 * library (which lives in the RPC server), we need to use this function to create a
 * Key object which can be used through the RPC client.
 *
 * The reason for this is that the API key object "knows" its API context and
 * contains a pointer to the library Key object (which holds the actual data). This
 * function sets the correct API context of a API key object (for the RPC client)
 * together with the pointer to the actual Key (on the RPC server).
 *
 * NOTE: This function allocates a new object, which must be free'd with the
 *       appropriate function (e.g., if it is a OS_CryptoKey_Handle_t, the corresponding
 *       OS_CryptoKey_free() needs to be called).
 *
 * NOTE: This function should only be used by an expert; underlying library objects
 *       should never be used directly, as this may create situations where multiple
 *       proxy objects point to the same library objects, which, in turn may lead
 *       to use-after-free and other issues!
 *
 * @param proxy (required) pointer to handle of OS Crypto object (e.g., can
 *  be a OS_CryptoKey_Handle_t*)
 * @param hCrypto (required) handle of OS Crypto API
 * @param ptr (required) pointer to the library Key object from the some API instance
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
OS_Crypto_migrateObject(
    OS_Crypto_Object_t**       proxy,
    const OS_Crypto_Handle_t   hCrypto,
    const CryptoLib_Object_ptr ptr);

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

/** @} */