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
 * @param keyHandle         returns the key handle for the imported key
 * @param name              name of the key to import
 * @param keyBytesBuffer    buffer containing the key bytes
 * @param algorithm         algorithm that uses the key
 * @param flags             \see SeosCryptoKey_Flags
 * @param lenBits           length of the key in bits
 *
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE
 * @retval SEOS_ERROR_INVALID_PARAMETER
 * @retval SEOS_ERROR_OPERATION_DENIED
 *
 */
seos_err_t
SeosKeyStoreApi_importKey(SeosKeyStoreCtx*          keyStoreCtx,
                          SeosCrypto_KeyHandle*     keyHandle,
                          const char*               name,
                          void const*               keyBytesBuffer,
                          unsigned int              algorithm,
                          unsigned int              flags,
                          size_t                    lenBits);
/**
 * @brief Retreives the key with a given name from the keystore
 *
 * @param keyStoreCtx   pointer to keyStoreCtx
 * @param keyHandle     receives the key handle
 * @param name          name of the key to get
 *
 * @return SEOS_ERROR_GENERIC
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE
 * @retval SEOS_ERROR_INVALID_PARAMETER
 * @retval SEOS_ERROR_OPERATION_DENIED
 *
 */
seos_err_t
SeosKeyStoreApi_getKey(SeosKeyStoreCtx*         keyStoreCtx,
                       SeosCrypto_KeyHandle*    keyHandle,
                       const char*              name);
/**
 * @brief Deletes a key with a given name from the keystore
 *
 * @param keyHandle     receives the key handle
 *
 * @return SEOS_ERROR_NOT_FOUND
 *
 */
seos_err_t
SeosKeyStoreApi_deleteKey(SeosCrypto_KeyHandle keyHandle);

/**
 * @brief Closes a key with a given handle
 *
 * @param keyHandle     receives the key handle
 *
 * @return SEOS_ERROR_INVALID_HANDLE
 *
 */
seos_err_t
SeosKeyStoreApi_closeKey(SeosCrypto_KeyHandle keyHandle);
/**
 * @brief Copies the key from the current key store to the destination key store
 *
 * @param keyHandle     receives the key handle
 * @param destKeyStore  pointer to the destination key store
 *
 * @return SEOS_ERROR_GENERIC
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE
 * @retval SEOS_ERROR_INVALID_PARAMETER
 * @retval SEOS_ERROR_OPERATION_DENIED
 *
 */
seos_err_t
SeosKeyStoreApi_copyKey(SeosCrypto_KeyHandle    keyHandle,
                        SeosKeyStoreCtx*        destKeyStore);
/**
 * @brief Moves the key from the current key store to
 * the destination key store (after the operation the key is no longer in the
 * current key store)
 *
 * @param keyHandle     receives the key handle
 * @param destKeyStore  pointer to the destination key store
 *
 * @return SEOS_ERROR_GENERIC
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE
 * @retval SEOS_ERROR_INVALID_PARAMETER
 * @retval SEOS_ERROR_OPERATION_DENIED
 *
 */
seos_err_t
SeosKeyStoreApi_moveKey(SeosCrypto_KeyHandle    keyHandle,
                        SeosKeyStoreCtx*        destKeyStore);
/**
 * @brief Generates a key with a given name using an RNG, stores the key into the key store
 * and returns the key data in the key object.
 *
 * @param keyStoreCtx   pointer to keyStoreCtx
 * @param keyHandle     returns the key handle for the generated key
 * @param name          name of the key to get
 * @param algorithm     algorithm that uses the key
 * @param flags         \see SeosCryptoKey_Flags
 * @param lenBits       length of the key in bits
 *
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE
 * @retval SEOS_ERROR_INVALID_PARAMETER
 * @retval SEOS_ERROR_OPERATION_DENIED
 *
 */
seos_err_t
SeosKeyStoreApi_generateKey(SeosKeyStoreCtx*            keyStoreCtx,
                            SeosCrypto_KeyHandle*       keyHandle,
                            const char*                 name,
                            unsigned int                algorithm,
                            unsigned int                flags,
                            size_t                      lenBits);
/**
 * @brief Deletes all the keys from the keystore
 *
 * @param keyStoreCtx   pointer to keyStoreCtx
 *
 * @retval SEOS_ERROR_NOT_FOUND
 *
 */
seos_err_t
SeosKeyStoreApi_wipeKeyStore(SeosKeyStoreCtx* keyStoreCtx);

/** @} */
