/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @addtogroup API
 * @{
 *
 * @file OS_Keystore.h
 *
 * @brief OS Keystore library API
 *
 */

#pragma once

#include "SeosError.h"
#include "OS_Crypto.h"

#include "LibIO/FileStreamFactory.h"

#include <stddef.h>

/**
 * API context is an opaque struct, the handle of an API context is a pointer
 */
typedef struct OS_Keystore OS_Keystore_t, *OS_Keystore_Handle_t;

/**
 * @brief Initialize an instance of the Keystore
 *
 * @param hKeystore[out]    pointer to keystore handle
 * @param fileStreamFactory file stream factory to use for writing keys
 * @param hCrypto           handle to initialized OS_Crypto instance
 * @param name              name keystore
 *
 * @return seos_err_t
 *
 */
seos_err_t
OS_Keystore_init(
    OS_Keystore_Handle_t* hKeystore,
    FileStreamFactory*    fileStreamFactory,
    OS_Crypto_Handle_t    hCrypto,
    const char*           name);

/**
 * @brief Free up an instance of the Keystore
 *
 * @param hKeystore  keystore handle
 *
 * @return seos_err_t
 *
 */
seos_err_t
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
 * @return seos_err_t
 *
 */
seos_err_t
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
 * @return seos_err_t
 *
 */
seos_err_t
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
 * @return seos_err_t
 *
 */
seos_err_t
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
 * @return seos_err_t
 *
 */
seos_err_t
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
 * @return seos_err_t
 *
 */
seos_err_t
OS_Keystore_moveKey(
    OS_Keystore_Handle_t hKeystore,
    const char*          name,
    OS_Keystore_Handle_t hDestKeystore);

/**
 * @brief Deletes all the keys from the Keystore
 *
 * @param hKeystore  keystore handle
 *
 * @return seos_err_t
 *
 */
seos_err_t
OS_Keystore_wipeKeystore(
    OS_Keystore_Handle_t hKeystore);

/** @} */