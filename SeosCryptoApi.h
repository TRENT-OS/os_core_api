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
    SeosCryptoApi_Mode_LIBRARY = 0,
    SeosCryptoApi_Mode_RPC_CLIENT,
    SeosCryptoApi_Mode_ROUTER,
    SeosCryptoApi_Mode_RPC_SERVER_WITH_LIBRARY,
} SeosCryptoApi_Mode;

typedef struct SeosCryptoVtable SeosCryptoVtable;
typedef struct
{
    const SeosCryptoVtable* vtable;
    void* context;
} SeosCryptoApi_Impl;

typedef struct
{
    SeosCryptoApi_Mode mode;
    SeosCryptoApi_Impl impl;
    struct
    {
        void* context;
    } server;
} SeosCryptoApi;
typedef SeosCryptoApi* SeosCryptoApi_Ptr;

// Include all after definining the API+Impl above; also make sure that key and
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
} SeosCryptoApi_Lib_Config;

typedef struct
{
    void* dataPort;
    SeosCryptoApi* api;
} SeosCryptoApi_RpcClient_Config;

typedef struct
{
    void* dataPort;
} SeosCryptoApi_RpcServer_Config;

typedef struct
{
    SeosCryptoApi_RpcClient_Config client;
    SeosCryptoApi_Lib_Config lib;
} SeosCryptoApi_Router_Config;

typedef struct
{
    SeosCryptoApi_Mode mode;
    SeosCryptoApi_MemIf mem;
    union
    {
        SeosCryptoApi_Lib_Config lib;
        SeosCryptoApi_RpcClient_Config client;
        SeosCryptoApi_Router_Config router;
    } impl;
    SeosCryptoApi_RpcServer_Config server;
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
 *      .impl.client.api = serverApi
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
 * @param api (required) pointer to the seos crypto context
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
    SeosCryptoApi*              api,
    const SeosCryptoApi_Config* cfg);

/**
 * @brief Free a context associated with the Crypto API
 *
 * @param api (required) pointer to the seos crypto context
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
seos_err_t
SeosCryptoApi_free(
    SeosCryptoApi* api);

/** @} */
