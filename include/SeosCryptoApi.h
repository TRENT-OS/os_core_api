/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
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

#include <limits.h>

/**
 * Fixed size of dataport, will be replaced with a more generic and self-contained
 * solution in the future.
 */
#define SeosCryptoApi_SIZE_DATAPORT PAGE_SIZE

typedef enum
{
    SeosCryptoApi_Mode_NONE = 0,
    SeosCryptoApi_Mode_LIBRARY,
    SeosCryptoApi_Mode_RPC_CLIENT,
    SeosCryptoApi_Mode_RPC_SERVER_WITH_LIBRARY,
    SeosCryptoApi_Mode_ROUTER,
} SeosCryptoApi_Mode;

typedef struct SeosCryptoApi SeosCryptoApi;
/**
 * Handle to an API context.
 */
typedef SeosCryptoApi* SeosCryptoApiH;
/**
 * All underlying library objects are encapsulated via a proxy object, to which
 * ultimately all API object handles point.
 */
typedef struct SeosCryptoApi_Proxy SeosCryptoApi_Proxy;
/**
 *  Pointer to (MAC, Digest, etc.) objects managed by library.
 */
typedef void* SeosCryptoLib_Object;

// Include all after definining the API handle above; also make sure that key and
// digest are included first so they are defined for the other functions.
#include "SeosCryptoApi_Key.h"
#include "SeosCryptoApi_Digest.h"
#include "SeosCryptoApi_Agreement.h"
#include "SeosCryptoApi_Cipher.h"
#include "SeosCryptoApi_Mac.h"
#include "SeosCryptoApi_Signature.h"
#include "SeosCryptoApi_Rng.h"

typedef void* (SeosCryptoApi_MallocFunc)(
    size_t size);
typedef void (SeosCryptoApi_FreeFunc)(
    void* ptr);

/**
 * User of API can provide custom allocator/free functionality.
 */
typedef struct
{
    SeosCryptoApi_MallocFunc* malloc;
    SeosCryptoApi_FreeFunc* free;
}
SeosCryptoApi_MemIf;

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
        SeosCryptoApi_Rng_EntropyFunc* entropy;
        /**
         * Context to pass to entropy callback.
         */
        void* context;
    } rng;
} SeosCryptoLib_Config;

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
} SeosCryptoRpcClient_Config;

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
} SeosCryptoRpcServer_Config;

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
    SeosCryptoRpcClient_Config client;
    /**
     * Configuration of remote RPC Server instance of the Crypto API.
     */
    SeosCryptoLib_Config lib;
} SeosCryptoRouter_Config;

/**
 * The Crypto API main configuration struct; first the mode needs to be set to
 * the desired value, then the respective sub-configuration must be filled in:
 * -  SeosCryptoApi_Mode_LIBRARY:                   impl.lib
 * -  SeosCryptoApi_Mode_RPC_CLIENT:                impl.client
 * -  SeosCryptoApi_Mode_ROUTER:                    impl.router
 * -  SeosCryptoApi_Mode_RPC_SERVER_WITH_LIBRARY:   impl.lib AND server
 */
typedef struct
{
    SeosCryptoApi_Mode mode;
    SeosCryptoApi_MemIf mem;
    union
    {
        SeosCryptoLib_Config lib;
        SeosCryptoRpcClient_Config client;
        SeosCryptoRouter_Config router;
    } impl;
    SeosCryptoRpcServer_Config server;
} SeosCryptoApi_Config;

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
 *  SeosCryptoApi_Config cfgLocal =  {
 *      .mode = SeosCryptoApi_Mode_LIBRARY,
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
 *  SeosCryptoApi_Config cfgRemote = {
 *      .mode = SeosCryptoApi_Mode_RPC_CLIENT,
 *      .mem = {
 *          .malloc = malloc,
 *          .free = free,
 *      },
 *      .impl.client.dataPort = clientDataport
 *  };
 *  \endcode
 * 3. Run API instance as RPC server backend:
 *  \code{.c}
 *  SeosCryptoApi_Config cfgServer = {
 *      .mode = SeosCryptoApi_Mode_RPC_SERVER_WITH_LIBRARY,
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
 *  SeosCryptoApi_Config cfgRouter = {
 *      .mode = SeosCryptoApi_Mode_ROUTER,
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
 * @param hCrypto (required) pointer to handle of SEOS Crypto API
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
SeosCryptoApi_init(
    SeosCryptoApiH*             hCrypto,
    const SeosCryptoApi_Config* cfg);

/**
 * @brief Free a context associated with the Crypto API
 *
 * @param hCrypto (required) handle of SEOS Crypto API
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_free(
    SeosCryptoApiH hCrypto);

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
 * @param proxy (required) handle of SEOS Crypto Key object, e.g.,
 * SeosCryptoApi_MacH, SeosCryptoApi_DigestH, etc.
 *
 * @return pointer to object or NULL of \p proxy was NULL
 */
SeosCryptoLib_Object*
SeosCryptoApi_getObject(
    const SeosCryptoApi_Proxy* proxy);

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
 *       appropriate function (e.g., if it is a SeosCryptoApi_KeyH, the corresponding
 *       SeosCryptoApi_Key_free() needs to be called).
 *
 * NOTE: This function should only be used by an expert; underlying library objects
 *       should never be used directly, as this may create situations where multiple
 *       proxy objects point to the same library objects, which, in turn may lead
 *       to use-after-free and other issues!
 *
 * @param proxy (required) pointer to handle of SEOS Crypto object (e.g., can
 *  be a SeosCryptoApi_KeyH*)
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param ptr (required) pointer to the library Key object from the some API instance
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_migrateObject(
    SeosCryptoApi_Proxy**      proxy,
    const SeosCryptoApiH       hCrypto,
    const SeosCryptoLib_Object ptr);

/**
 * @brief Get mode of Crypto API instance.
 *
 * @param hCrypto (required) handle of SEOS Crypto API
 *
 * @return Mode of API or SeosCryptoApi_Mode_NONE if \p hCrypto is NULL
 */
SeosCryptoApi_Mode
SeosCryptoApi_getMode(
    const SeosCryptoApiH hCrypto);

/** @} */