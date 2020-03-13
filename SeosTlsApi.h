/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
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

#include "SeosCryptoApi.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Maxmimum size of PEM-encoded CA cert we accept.
 */
#define SeosTlsLib_SIZE_CA_CERT_MAX    3072
#define SeosTlsLib_MAX_CIPHERSUITES    8
#define SeosTlsLib_MAX_DIGESTS         SeosTlsLib_MAX_CIPHERSUITES

/**
 * Functions for sending/receiving data on an open socket.
 */
typedef int (SeosTlsLib_RecvFunc)(
    void*          ctx,
    unsigned char* buf,
    size_t         len);
typedef int (SeosTlsLib_SendFunc)(
    void*                ctx,
    const unsigned char* buf,
    size_t               len);

/**
 * Digest algorithms available
 */
typedef enum
{
    SeosTlsLib_Digest_NONE     = 0x00,
    SeosTlsLib_Digest_SHA256   = 0x06,
} SeosTlsLib_Digest;

/**
 * Cipher suites available
 */
typedef enum
{
    SeosTlsLib_CipherSuite_NONE                              = 0x0000,
    SeosTlsLib_CipherSuite_DHE_RSA_WITH_AES_128_GCM_SHA256   = 0x009e,
    SeosTlsLib_CipherSuite_ECDHE_RSA_WITH_AES_128_GCM_SHA256 = 0xc02f
} SeosTlsLib_CipherSuite;

typedef enum
{
    SeosTlsLib_Flag_NONE          = 0x0000,
    /**
     *  Produce debug output from underlying protocol provider
     */
    SeosTlsLib_Flag_DEBUG         = 0x0001,
    /**
     * Do not attempt to validate server certificate. This is dangerous,
     * so you better know what you are doing
     */
    SeosTlsLib_Flag_NO_VERIFY     = 0x0002
} SeosTlsLib_Flag;

/**
 * For legacy reasons it may be important to override the param/algorithm choices
 * automatically derived from the chosen ciphersuites (where possible).
 */
typedef struct
{
    /**
     * Possibly allow other digests as well (maybe the suite says to use SHA256
     * but for legacy reasons we may want to add SHA1 or so..)
     *
     * Note: We add +1 so the last element can be set to 0 internally.
     */
    SeosTlsLib_Digest sessionDigests[SeosTlsLib_MAX_DIGESTS + 1];
    size_t sessionDigestsLen;
    SeosTlsLib_Digest signatureDigests[SeosTlsLib_MAX_DIGESTS + 1];
    size_t signatureDigestsLen;
    /**
     * For all asymmetric operations (DH key exchange, RSA signatures, etc.)
     * we can enforce a mimimum level of security by making sure the respective
     * bit lenghts adhere to this minimum. The curve is currently set and can't
     * be changed, so there is no parameter for ECC.
     */
    size_t rsaMinBits;
    size_t dhMinBits;
} SeosTlsLib_Policy;

typedef struct
{
    struct
    {
        SeosTlsLib_RecvFunc* recv;
        SeosTlsLib_SendFunc* send;
        /**
         * This is a parameter which is passed into every call to send/recv.
         * Typically it would be a socket handle or similar.
         */
        void* context;
    } socket;
    struct
    {
        /**
         * Policy can be NULL, then it is set automatically.
         */
        SeosTlsLib_Policy* policy;
        /**
         * Need an initialized crypto context for SEOS Crypto API
         */
        SeosCryptoApi* context;
        /**
         * Here a certificate in PEM encoding (including headers) is passed to
         * the TLS API so it can be used to verify the root of the server's
         * certificate chain.
         */
        char caCert[SeosTlsLib_SIZE_CA_CERT_MAX];
        /**
         * For simplicity, a user can just set some ciphersuites and be fine. The hash
         * given in the cipersuites will be ENFORCED for everything (incl. session hash,
         * signature hashes etc.). Similary, the key size of the AES key will be used to
         * determine the minimum asymmetric key lengths automatically, so all parameters
         * and algorithms will be internally consistent (as far as the suite allows it).
         *
         * Note: We add +1 so the last element can be set to 0 internally.
         */
        SeosTlsLib_CipherSuite cipherSuites[SeosTlsLib_MAX_CIPHERSUITES + 1];
        size_t cipherSuitesLen;
    } crypto;
    SeosTlsLib_Flag flags;
} SeosTlsLib_Config;

/**
 * The main API context
 */
typedef struct SeosTlsApi SeosTlsApi;
typedef SeosTlsApi* SeosTlsApiH;

/**
 * API configuration
 */
typedef struct
{
    SeosTlsLib_Config library;
    void* dataport;
} SeosTlsRpc_Server_Config;

typedef SeosTlsApi* SeosTlsRpcServer_Handle;
typedef struct
{
    SeosTlsRpcServer_Handle handle;
    void* dataport;
} SeosTlsRpc_Client_Config;

typedef enum
{
    SeosTlsApi_Mode_NONE = 0,
    /**
     * Use TLS module as library, all calls to the API will internally mapped to
     * be executed locally.
     */
    SeosTlsApi_Mode_LIBRARY,
    /**
     * Use TLS module as component. This requires a TLS component with a matching
     * RPC interface. Calls to the API will internally be mapped to RPC calls and
     * thus will be executed in the component for better isolation.
     */
    SeosTlsApi_Mode_RPC_SERVER,
    SeosTlsApi_Mode_RPC_CLIENT
} SeosTlsApi_Mode;

typedef struct
{
    SeosTlsApi_Mode mode;
    union
    {
        SeosTlsLib_Config library;
        SeosTlsRpc_Client_Config client;
        SeosTlsRpc_Server_Config server;
    } config;
} SeosTlsApi_Config;

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
 * @param hTls (required) pointer to handle of the SEOS TLS API context
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
SeosTlsApi_init(
    SeosTlsApiH*             hTls,
    const SeosTlsApi_Config* cfg);

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
 * @param hTls (required) handle of the SEOS TLS API context
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval SEOS_ERROR_OPERATION_DENIED if the TLS session is not already established
 */
seos_err_t
SeosTlsApi_handshake(
    SeosTlsApiH hTls);

/**
 * @brief Write to TLS connection
 *
 * After handshake has been completed, this function can be used to write data
 * to the server via an established TLS tunnel.
 *
 * @param hTls (required) handle of the SEOS TLS API context
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
 * @retval SEOS_ERROR_OPERATION_DENIED if the TLS session is not yet established
 */
seos_err_t
SeosTlsApi_write(
    SeosTlsApiH  hTls,
    const void*  data,
    const size_t dataSize);

/**
 * @brief Read from a TLS connection
 *
 * After handshake has been completed, this function can be used to read from an
 * established TLS connection. The server may close the session, this will be
 * signaled by returning no error, but setting \p dataSize to zero.
 *
 * @param hTls (required) handle of the SEOS TLS API context
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
SeosTlsApi_read(
    SeosTlsApiH hTls,
    void*       data,
    size_t*     dataSize);

/**
 * @brief Reset a TLS connection
 *
 * Reset an initialized and used TLS API context for re-use. After this, given
 * that the associated socket is still connected, the TLS connection needs to be
 * re-established with via the handshake() function.
 *
 * @param hTls (required) handle of the SEOS TLS API context
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval SEOS_ERROR_ABORTED if the re-set failed
 */
seos_err_t
SeosTlsApi_reset(
    SeosTlsApiH hTls);

/**
 * @brief Free a TLS object
 *
 * Frees the memory allocted with a TLS object.
 *
 * @param hTls (required) handle of the SEOS TLS API context
 *
 * @return an error code
 * @retval SEOS_SUCCESS if operation succeeded
 * @retval SEOS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 */
seos_err_t
SeosTlsApi_free(
    SeosTlsApiH hTls);

/**
 * @brief Get mode of TLS API
 *
 * @param hTls (required) handle of the SEOS TLS API context
 *
 * @return mode the TLS API is used in
 */
SeosTlsApi_Mode
SeosTlsApi_getMode(
    SeosTlsApiH hTls);

/** @} */