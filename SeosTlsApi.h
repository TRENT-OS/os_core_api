/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * @defgroup SeosTlsApi SEOS TLS API
 * @{
 *
 * @file SeosTlsApi.h
 *
 * @brief SEOS TLS API library
 *
 */

#pragma once

#include "SeosTlsApi_Impl.h"

/**
 * @brief Initialize TLS API
 *
 * Set up the TLS API by providing a \p cfg parameter which holds the APIs
 * configuration.
 *
 * The TLS API can be configured to operate in three modes:
 * 1. As library: Here all calls to handshake/read/write will be executed within the
 *    component of the caller.
 * 2. As RPC client: Here, an RPC client is configured which passes all calls to
 *    handshake/read/write to an RPC server that resides in a different component.
 * 3. As RPC server: This is the "other half" of the RPC system, which takes calls
 *    from the RPC client. Calling handshake/read/write on an API instance in this
 *    configuration will not work.
 *
 * Currently, the TLS API supports these ciphersuites:
 * - `SeosTls_CipherSuite_DHE_RSA_WITH_AES_128_GCM_SHA256`
 *      Key exchange based on DH with RSA-based certificates and AES-GCM with
 *      128-bit keys for encryption and SHA256 for key derivation and session
 *      hashing.
 * - `SeosTls_CipherSuite_ECDHE_RSA_WITH_AES_128_GCM_SHA256`
 *      Key exchange based on ECDH with RSA-based certificates and AES-GCM with
 *      128-bit keys for encryption and SHA256 for key derivation and session
 *      hashing.
 *
 * The TLS API supports these digests for certificate hashing and session digests:
 * - `SeosTls_Digest_SHA256`:     SHA256
 *
 * @param ctx (required) pointer to the TLS context
 * @param cfg (required) configuration of TLS API
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval SEOS_ERROR_NOT_SUPPORTED if any of the choices in \p policy are at this
 *  point not supported by the TLS API (e.g., DH min lengths, ...)
 */
seos_err_t
SeosTlsApi_init(SeosTlsApi_Context*         ctx,
                const SeosTlsApi_Config*    cfg);

/**
 * @brief Perform the TLS handshake
 *
 * The critical part of a TLS protocol run is performing the TLS handshake. After
 * a TLS object has been initialized, the handshake has to performed. For this, the
 * socket passed via the configuration needs to be connected to the target server.
 *
 * Only after the handshake has been executed successfully, the read/write functions
 * can be called.
 *
 * @param ctx (required) pointer to the TLS context
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval SEOS_ERROR_OPERATION_DENIED if the TLS session is not already established
 */
seos_err_t
SeosTlsApi_handshake(SeosTlsApi_Context*    ctx);

/**
 * @brief Write to TLS connection
 *
 * After handshake has been completed, this function can be used to write data
 * to the server via an established TLS tunnel.
 *
 * @param ctx (required) pointer to the TLS context
 * @param data (required) pointer data buffer
 * @param dataSize (required) amount of bytes to write
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval SEOS_ERROR_ABORTED if the write failed
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p data is too large for the
 *  size of the dataport, i.e., \p dataSize > `PAGE_SIZE`
 * @retval SEOS_ERROR_OPERATION_DENIED if the TLS session is not yet established*
 */
seos_err_t
SeosTlsApi_write(SeosTlsApi_Context*    ctx,
                 const void*            data,
                 const size_t           dataSize);

/**
 * @brief Read from a TLS connection
 *
 * After handshake has been completed, this function can be used to read from an
 * established TLS connection. The server may close the session, this will be
 * signaled by returning no error, but setting \p dataSize to zero.
 *
 * @param ctx (required) pointer to the TLS context
 * @param data (required) pointer data buffer
 * @param dataSize (required) amount of bytes requested, in case of success it
 * will be set to the amount of bytes effectively read (or zero if peer has closed
 * the connection)
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval SEOS_ERROR_ABORTED if the read failed
 * @retval SEOS_ERROR_INSUFFICIENT_SPACE if \p data is too large for the
 *  size of the dataport, i.e., \p dataSize > `PAGE_SIZE`
 * @retval SEOS_ERROR_OPERATION_DENIED if the TLS session is not yet established
 */
seos_err_t
SeosTlsApi_read(SeosTlsApi_Context*     ctx,
                void*                   data,
                size_t*                 dataSize);

/**
 * @brief Free a TLS object
 *
 * Frees the memory allocted with a TLS object.
 *
 * @param ctx (required) pointer to the TLS context
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 */
seos_err_t
SeosTlsApi_free(SeosTlsApi_Context*     ctx);

/** @} */