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

#include <limits.h>

#define SeosCryptoApi_SIZE_DATAPORT PAGE_SIZE

typedef enum
{
    SeosCryptoApi_Mode_NONE = 0,
    SeosCryptoApi_Mode_LIBRARY,
    SeosCryptoApi_Mode_RPC_CLIENT,
    SeosCryptoApi_Mode_RPC_SERVER_WITH_LIBRARY,
    SeosCryptoApi_Mode_ROUTER,
} SeosCryptoApi_Mode;

/**
 * These types are all internally defined; here we just need their names
 */

// The main API context and its handle
typedef struct SeosCryptoApi SeosCryptoApi;
typedef SeosCryptoApi* SeosCryptoApiH;
// All underlying library objects are encapsulated via a proxy object, to which
// ultimately all API handles point
typedef struct SeosCryptoApi_Proxy SeosCryptoApi_Proxy;
// Pointer to (MAC, Digest, etc.) objects managed by library
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

typedef struct
{
    SeosCryptoApi_MallocFunc* malloc;
    SeosCryptoApi_FreeFunc* free;
}
SeosCryptoApi_MemIf;

typedef struct
{
    struct
    {
        SeosCryptoApi_Rng_EntropyFunc* entropy;
        void* context;
    } rng;
} SeosCryptoLib_Config;

typedef struct
{
    void* dataPort;
} SeosCryptoRpcClient_Config;

typedef struct
{
    void* dataPort;
} SeosCryptoRpcServer_Config;

typedef struct
{
    SeosCryptoRpcClient_Config client;
    SeosCryptoLib_Config lib;
} SeosCryptoRouter_Config;

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
 * This function allocates a Crypto API object and sets up crypto functionality
 * according to the configuration.
 *
 * The API can be operated in two modes:
 * - DIRECT mode: API functionality can be used directly through calls to the API.
 * - SERVER mode: The API instance acts as RPC server for processing Crypto API
 *                calls. Such an API instance cannot be used directly, but has to
 *                be addressed through the associated RPC client's API instance.
 *
 * For DIRECT mode, the following modules are currently supported:
 * - Library:    Calls to API functions are executed directly to perform crypto-
 *               graphic operations.
 * - RPC Client: Calls to API functions are relayed to the RPC server, where they
 *               are executed by an implementation in the context of that component.
 *
 * Here are some example configurations for each of the three modes:
 * 1. Run the API instance as local instance of the Crypto library:
 *  \code{.c}
 *  SeosCryptoApi_Config cfgLocal =  {
 *      .mode = SeosCryptoApi_Mode_LIBRARY,
 *      .mem = {
 *          .malloc = malloc,
 *          .free = free,
 *      },
 *      .impl.lib.rng = {
 *          .entropy = entropy,
 *          .context = rngContext
 *      }
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
 *      .impl.client.dataPort = clientDataport,
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
 *      .impl.lib.rng = {
 *          .entropy = entropy,
 *          .context = rngContext
 *      },
 *      .server.dataPort = serverDataport
 *  };
 *  \endcode
 *
 * All API configurations must pass function pointers to malloc/free, those involved
 * with RPC functionality must set the dataport configuration according to those
 * addresses assigned by Camkes.
 *
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param cfg (required) pointer to the API configuration
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
 * @brief Get pointer to library object
 *
 * If we have a crypto API proxy object and we need to migrate it from one API
 * instance to another, we need to get the pointer to the underlying LIB object.
 *
 * @param proxy (required) handle of SEOS Crypto Key object
 *
 * @return pointer to object or NULL of \p proxy was NULL
 */
SeosCryptoLib_Object*
SeosCryptoApi_getObject(
    const SeosCryptoApi_Proxy* proxy);

/**
 * @brief Migrate a library object from a one API instance to another instance
 *
 * With the RPC functionality of the Crypto API it is possible to have an RPC
 * server with an instance of the LIB in one component and an RPC client in
 * another component that uses the same LIB.
 *
 * Now, if we are on the RPC client side and get a pointer to a Key object of the
 * LIB (which lives in the RPC server), we need to use this function to create a
 * Key object which can be used through the RPC client.
 *
 * The reason for this is that the API key object "knows" its API context and
 * contains a pointer to the LIB Key object (which holds the actual data). This
 * function sets the correct API context of a API key object (for the RPC client)
 * together with the pointer to the actual Key (on the RPC server).
 *
 * @param proxy (required) pointer to handle of SEOS Crypto object (e.g., can
 * be a SeosCryptoApi_KeyH*)
 * @param hCrypto (required) handle of SEOS Crypto API
 * @param ptr (required) pointer to the LIB Key object from the some API instance
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
 * @brief Get API mode
 *
 * @param hCrypto (required) handle of SEOS Crypto API
 *
 * @return Mode of API or SeosCryptoApi_Mode_NONE if \p hCrypto is NULL
 */
SeosCryptoApi_Mode
SeosCryptoApi_getMode(
    const SeosCryptoApiH hCrypto);

/** @} */
