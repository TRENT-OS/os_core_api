/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @addtogroup API
 * @{
 *
 * @file SeosKeyStoreApi.h
 *
 * @brief SEOS KeyStore API library
 *
 */
#pragma once

/***************************** KeyStore functions *******************************/
/**
 * @brief Imports a SeosCrypto_Key object into the keystore
 *
 * @param keyStoreCtx       pointer to keyStoreCtx
 * @param name              name of the key to import
 * @param keyData           buffer containing the key data
 * @param keySize           size of the key data in bytes
 *
 * @return seos_err_t
 *
 */
seos_err_t
SeosKeyStoreApi_importKey(SeosKeyStoreCtx*          keyStoreCtx,
                          const char*               name,
                          void const*               keyData,
                          size_t                    keySize);
/**
 * @brief Retreives the key with a given name from the keystore
 *
 * @param       keyStoreCtx     pointer to keyStoreCtx
 * @param       name            name of the key to get
 * @param[out]  keyData         address of the buffer which will be filled 
 *                              with key data
 * @param[out]  keySize         address of the variable which will be filled 
 *                              with key data size
 *
 * @return seos_err_t
 *
 */
seos_err_t
SeosKeyStoreApi_getKey(SeosKeyStoreCtx*         keyStoreCtx,
                       const char*              name,
                       void*                    keyData,
                       size_t*                  keySize);
/**
 * @brief Deletes a key with a given name from the keystore
 *
 * @param keyStoreCtx   pointer to keyStoreCtx
 * @param name          name of the key to delete
 *
 * @return seos_err_t
 *
 */
seos_err_t
SeosKeyStoreApi_deleteKey(SeosKeyStoreCtx*         keyStoreCtx,
                          const char*              name);
/**
 * @brief Copies the key from the current key store to the destination key store
 *
 * @param keyStoreCtx   pointer to keyStoreCtx
 * @param name          name of the key to copy
 * @param destKeyStore  pointer to the destination key store
 *
 * @return seos_err_t
 *
 */
seos_err_t
SeosKeyStoreApi_copyKey(SeosKeyStoreCtx*         keyStoreCtx,
                        const char*              name,
                        SeosKeyStoreCtx*         destKeyStore);
/**
 * @brief Moves the key from the current key store to
 * the destination key store (after the operation the key is no longer in the
 * current key store)
 *
 * @param keyStoreCtx   pointer to keyStoreCtx
 * @param name          name of the key to move
 * @param destKeyStore  pointer to the destination key store
 *
 * @return seos_err_t
 *
 */
seos_err_t
SeosKeyStoreApi_moveKey(SeosKeyStoreCtx*         keyStoreCtx,
                        const char*              name,
                        SeosKeyStoreCtx*         destKeyStore);
/**
 * @brief Deletes all the keys from the keystore
 *
 * @param keyStoreCtx   pointer to keyStoreCtx
 *
 * @return seos_err_t
 *
 */
seos_err_t
SeosKeyStoreApi_wipeKeyStore(SeosKeyStoreCtx* keyStoreCtx);

/** @} */
