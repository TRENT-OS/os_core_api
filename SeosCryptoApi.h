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
#include <stddef.h>

// This will be cleaned up soon..
#define SeosCryptoRpcClient_TO_SEOS_CRYPTO_CTX(self)    (&(self)->parent)
#define SeosCryptoLib_TO_SEOS_CRYPTO_CTX(self)          (&(self)->parent)

#define SeosCryptoApi_SIZE_DATAPORT PAGE_SIZE

typedef struct SeosCryptoApi_Context SeosCryptoApi_Context;
typedef struct SeosCryptoLib SeosCryptoLib;

// Include all after definining the Api_Context above; also make sure that key and
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
    SeosCryptoApi_Rng_EntropyFunc* entropy;
} SeosCryptoApi_Callbacks;

typedef struct
{
    SeosCryptoApi_MallocFunc* malloc;
    SeosCryptoApi_FreeFunc* free;
}
SeosCryptoApi_MemIf;

typedef struct SeosCryptoRpcServer SeosCryptoRpcServer;
typedef SeosCryptoRpcServer* SeosCryptoApi_RpcServer;

/** @} */
