/* Copyright (C) 2019, Hensoldt Cyber GmbH */

/**
 * @file
 * @addtogroup OS_Keystore OS Keystore API
 * @{
 * @brief OS Keystore API library
 */

#pragma once

#include "OS_Error.h"
#include "OS_Crypto.h"
#include "OS_FileSystem.h"

#include <stddef.h>

/// @cond INTERNAL
//------------------------------------------------------------------------------
typedef struct OS_Keystore OS_Keystore_t, *OS_Keystore_Handle_t;
//------------------------------------------------------------------------------
/// @endcond

/**
 * @brief Initialize an instance of the Keystore
 *
 * @return OS_Error_t
 */
OS_Error_t
OS_Keystore_init(
    OS_Keystore_Handle_t*  hKeystore, /**< [in,out] Pointer to the keystore
                                                    handle. */
    OS_FileSystem_Handle_t hFs,       /**< [in] Keystore's file system handle.*/
    OS_Crypto_Handle_t     hCrypto,   /**< [in] Handle to initialized the
                                                OS_Crypto instance. */
    const char*            name       /**< [in] Name of the given keystore. */);

/**
 * @brief Free up an instance of the Keystore
 *
 * @param hKeystore  keystore handle
 *
 * @return OS_Error_t
 *
 */
OS_Error_t
OS_Keystore_free(
    OS_Keystore_Handle_t hKeystore);

/**
 * @brief Imports a key blob into the Keystore
 *
 * @param hKeystore  keystore handle
 * @param name       name of the key to import
 * @param keyData    buffer containing the key blob
 * @param keySize    size of the key blob in bytes
 *
 * @return OS_Error_t
 *
 */
OS_Error_t
OS_Keystore_storeKey(
    OS_Keystore_Handle_t hKeystore,
    const char*          name,
    void const*          keyData,
    size_t               keySize);

/**
 * @brief Retrieves the key with a given name from the Keystore
 *
 * @param       hKeystore  keystore handle
 * @param       name       name of the key
 * @param[out]  keyData    buffer which will be filled with a key blob
 * @param[out]  keySize    variable which will be set to the key blob size
 *
 * @return OS_Error_t
 *
 */
OS_Error_t
OS_Keystore_loadKey(
    OS_Keystore_Handle_t hKeystore,
    const char*          name,
    void*                keyData,
    size_t*              keySize);

/**
 * @brief Deletes a key with a given name from the Keystore
 *
 * @param hKeystore  pointer to hKeystore
 * @param name       name of the key to delete
 *
 * @return OS_Error_t
 *
 */
OS_Error_t
OS_Keystore_deleteKey(
    OS_Keystore_Handle_t hKeystore,
    const char*          name);

/**
 * @brief Copies the key from the current key store to the destination key store
 *
 * @param hKeystore      keystore handle
 * @param name           name of the key to copy
 * @param hDestKeystore  destination keystore handle
 *
 * @return OS_Error_t
 *
 */
OS_Error_t
OS_Keystore_copyKey(
    OS_Keystore_Handle_t hKeystore,
    const char*          name,
    OS_Keystore_Handle_t hDestKeystore);

/**
 * @brief Moves the key from the current key store to
 * the destination key store (after the operation the key is no longer in the
 * current key store)
 *
 * @param hKeystore      keystore handle
 * @param name           name of the key to move
 * @param hDestKeystore  destination keystore handle
 *
 * @return OS_Error_t
 *
 */
OS_Error_t
OS_Keystore_moveKey(
    OS_Keystore_Handle_t hKeystore,
    const char*          name,
    OS_Keystore_Handle_t hDestKeystore);

/**
 * @brief Deletes all the keys from the Keystore
 *
 * @param hKeystore  keystore handle
 *
 * @return OS_Error_t
 *
 */
OS_Error_t
OS_Keystore_wipeKeystore(
    OS_Keystore_Handle_t hKeystore);

/** @} */
