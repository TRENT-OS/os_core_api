/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @defgroup OS_Tls OS TLS API
 * @{
 *
 * @file OS_Tls.h
 *
 * @brief OS TLS API library
 *
 */

#pragma once

#include "OS_Crypto.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Mode the TLS API instance should be operated in.
 */
typedef enum
{
    OS_Tls_MODE_NONE = 0,

    /**
     * Use TLS module as library, all calls to the API will internally mapped to
     * be executed locally.
     */
    OS_Tls_MODE_LIBRARY,

    /**
     * Forward all TLS API calls to a remote instance of the TLS API where the
     * TLS library is running in RPC Server mode. This allows for isolation of
     * the actual TLS protocol stack.
     */
    OS_Tls_MODE_CLIENT,

    /**
     * Use as RPC Server; can only be accessed through an appropriately configured
     * RPC client instance of the TLS API.
     */
    OS_Tls_MODE_SERVER
} OS_Tls_Mode_t;

/**
 * Digest algorithms available; these need to match the values of the underlying
 * TLS provider library
 */
typedef enum
{
    OS_Tls_DIGEST_NONE     = 0x00,

    /**
     * Use SHA256 as hash algorithm.
     */
    OS_Tls_DIGEST_SHA256   = 0x06,
} OS_Tls_Digest_t;

/**
 * Cipher suites available; these need to match the values of the underlying
 * TLS provider library
 */
typedef enum
{
    OS_Tls_CIPHERSUITE_NONE                              = 0x0000,

    /**
     * Use DHE_RSA_WITH_AES_128_GCM_SHA256 ciphersuite.
     */
    OS_Tls_CIPHERSUITE_DHE_RSA_WITH_AES_128_GCM_SHA256   = 0x009e,

    /**
     * Use ECDHE_RSA_WITH_AES_128_GCM_SHA256 ciphersuite.
     */
    OS_Tls_CIPHERSUITE_ECDHE_RSA_WITH_AES_128_GCM_SHA256 = 0xc02f
} OS_Tls_CipherSuite_t;

/**
 * Flags for setting options in the TLS provider lib.
 */
typedef enum
{
    OS_Tls_FLAG_NONE          = (1u << 0),

    /**
     *  Produce debug output from underlying protocol provider
     */
    OS_Tls_FLAG_DEBUG         = (1u << 1),

    /**
     * Do not attempt to validate server certificate. This is dangerous,
     * so you better know what you are doing!
     */
    OS_Tls_FLAG_NO_VERIFY     = (1u << 2)
} OS_Tls_Flag_t;

/**
 * Maxmimum size of PEM-encoded CA cert we accept. This is used to allocate a
 * static buffer in the config struct and for now set such it may hold ONE large
 * PEM-encoded certificate.
 */
#define OS_Tls_SIZE_CA_CERT_MAX    3072
/**
 * Max values to enable static array allocation; we do not actually provide as
 * many ciphersuites, yet.
 */
#define OS_Tls_MAX_CIPHERSUITES    8
#define OS_Tls_MAX_DIGESTS         OS_Tls_MAX_CIPHERSUITES

/**
 * For legacy reasons it may be important to override the param/algorithm choices
 * automatically derived from the chosen ciphersuites (where possible).
 */
typedef struct
{
    /**
     * Also allow these digest algorithms to be used for the hashing of all
     * session data.
     *
     * NOTE: We add +1 so the last element can be set to 0 internally.
     */
    OS_Tls_Digest_t sessionDigests[OS_Tls_MAX_DIGESTS + 1];
    /**
     * Amount of digests set in \p sessionDigests.
     */
    size_t sessionDigestsLen;
    /**
     * Also allow these digest algorithms to be used for the generation of hashes
     * which are then used for signatures (e.g., in certificates).
     *
     * NOTE: We add +1 so the last element can be set to 0 internally.
     */
    OS_Tls_Digest_t signatureDigests[OS_Tls_MAX_DIGESTS + 1];
    /**
     * Amount of digests set in \p signatureDigests.
     */
    size_t signatureDigestsLen;
    /**
     * Minimum bit length for RSA-based operations.
     */
    size_t rsaMinBits;
    /**
     * Minimum bit length for DH-based operations.
     */
    size_t dhMinBits;
} OS_Tls_Policy_t;

/**
 * Functions for sending/receiving data on an open socket.
 */
typedef int (OS_Tls_Recv_func)(
    void*          ctx,
    unsigned char* buf,
    size_t         len);
typedef int (OS_Tls_Send_func)(
    void*                ctx,
    const unsigned char* buf,
    size_t               len);

/**
 * Configuration for the TLS provider library used by the TLS API layer.
 */
typedef struct
{
    struct
    {
        /**
         * Callbacks to use by the TLS library to send/recieve data for a socket
         * that is already CONNECTED.
         */
        OS_Tls_Recv_func* recv;
        OS_Tls_Send_func* send;
        /**
         * This is a parameter which is passed into every call to send/recv.
         * Typically it would be a socket handle or similar.
         */
        void* context;
    } socket;
    struct
    {
        /**
         * Policy can be NULL, then it is set automatically based on the ciphersuites
         * chosen by the user.
         */
        OS_Tls_Policy_t* policy;
        /**
         * Need an initialized for OS Crypto API handle for cryptographic
         * operations.
         */
        OS_Crypto_Handle_t handle;
        /**
         * Here a certificate in PEM encoding (including headers) is passed to
         * the TLS API so it can be used to verify the root of the server's
         * certificate chain.
         */
        char caCert[OS_Tls_SIZE_CA_CERT_MAX];
        /**
         * For simplicity, a user can just set some ciphersuites and be fine. The hash
         * given in the cipersuites will be ENFORCED for everything (incl. session hash,
         * signature hashes etc.). Similary, the key size of the AES key will be used to
         * determine the minimum asymmetric key lengths automatically, so all parameters
         * and algorithms will be internally consistent (as far as the suite allows it).
         *
         * NOTE: We add +1 so the last element can be set to 0 internally.
         */
        OS_Tls_CipherSuite_t cipherSuites[OS_Tls_MAX_CIPHERSUITES + 1];
        /**
         * Amount of ciphersuites in \p cipherSuites.
         */
        size_t cipherSuitesLen;
    } crypto;
    OS_Tls_Flag_t flags;
} TlsLib_Config_t;

typedef struct OS_Tls OS_Tls_t;
/**
 * Handle of the main TLS API context.
 */
typedef OS_Tls_t* OS_Tls_Handle_t;

/**
 * Configuration for TLS API in RPC Server mode.
 */
typedef struct
{
    /**
     * Configuration of the local TLS library instance.
     */
    TlsLib_Config_t library;
    /**
     * Dataport to use for communication with the RPC Client.
     */
    void* dataport;
} TlsLibServer_Config_t;

/**
 * Configuration for TLS API in RPC Client mode.
 */
typedef struct
{
    /**
     * Dataport to use for communication with the RPC Server.
     */
    void* dataport;
} TlsLibClient_Config_t;

/**
 * Configuration of the TLS API. The mode value defines which of the union fields
 * needs to be filled in:
 *   OS_Tls_MODE_LIBRARY:       config.library
 *   OS_Tls_MODE_CLIENT:        config.client
 *   OS_Tls_MODE_SERVER:        config.server
 */
typedef struct
{
    OS_Tls_Mode_t mode;
    union
    {
        TlsLib_Config_t library;
        TlsLibClient_Config_t client;
        TlsLibServer_Config_t server;
    } config;
} OS_Tls_Config_t;

/**
 * @brief Initialize TLS API.
 *
 * Set up the TLS API by providing a \p cfg parameter which holds the APIs
 * configuration.
 *
 * NOTE: The TLS API expects a pre-initialized OS Crypto API instance.
 *
 * @param hTls (required) pointer to handle of the OS TLS API context
 * @param cfg (required) configuration of TLS API
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_NOT_SUPPORTED if any of the choices in \p policy are at this
 *  point not supported by the TLS API (e.g., DH min lengths, ...)
 */
OS_Error_t
OS_Tls_init(
    OS_Tls_Handle_t*       hTls,
    const OS_Tls_Config_t* cfg);

/**
 * @brief Perform the TLS handshake.
 *
 * The critical part of a TLS protocol run is performing the TLS handshake. After
 * a TLS object has been initialized, the handshake has to performed.
 *
 * Only after the handshake has been executed successfully, the read()/write()
 * functions can be called successfully.
 *
 * NOTE: Before this function is called, the socket handle passed via the config
 *       struct during init() must be ALREADY connected. The TLS API will never
 *       change the state of the socket explicitly, it will only call read()/write()
 *       for the transfer of protcol data.
 *
 * @param hTls (required) handle of the OS TLS API context
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_OPERATION_DENIED if the TLS session is not already established
 */
OS_Error_t
OS_Tls_handshake(
    OS_Tls_Handle_t hTls);

/**
 * @brief Write to TLS connection.
 *
 * After handshake has been completed, this function can be used to write data
 * to the server via an established TLS tunnel.
 *
 * NOTE: Internally, it will use the callback provided via the config struct to
 *       write() data to the socket.
 *
 * @param hTls (required) handle of the OS TLS API context
 * @param data (required) pointer data buffer
 * @param dataSize (required) amount of bytes to write
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_ABORTED if the write failed
 * @retval OS_ERROR_INSUFFICIENT_SPACE if \p data is too large for the
 *  size of the dataport, i.e., \p dataSize > `PAGE_SIZE`
 * @retval OS_ERROR_OPERATION_DENIED if the TLS session is not yet established
 */
OS_Error_t
OS_Tls_write(
    OS_Tls_Handle_t hTls,
    const void*     data,
    const size_t    dataSize);

/**
 * @brief Read from a TLS connection.
 *
 * After handshake has been completed, this function can be used to read from an
 * established TLS connection. The server may close the session, this will be
 * signaled by returning no error, but setting \p dataSize to zero.
 *
 * NOTE: Internally, it will use the callback provided via the config struct to
 *       read() data from the socket.
 *
 * @param hTls (required) handle of the OS TLS API context
 * @param data (required) pointer data buffer
 * @param dataSize (required) amount of bytes requested, in case of success it
 * will be set to the amount of bytes effectively read (or zero if peer has closed
 * the connection)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_ABORTED if the read failed
 * @retval OS_ERROR_INSUFFICIENT_SPACE if \p data is too large for the
 *  size of the dataport, i.e., \p dataSize > `PAGE_SIZE`
 * @retval OS_ERROR_OPERATION_DENIED if the TLS session is not yet established
 */
OS_Error_t
OS_Tls_read(
    OS_Tls_Handle_t hTls,
    void*           data,
    size_t*         dataSize);

/**
 * @brief Reset a TLS connection.
 *
 * Reset a TLS API context that has been already through a successful handshake()
 * and possibly multiple read()/write() calls.
 *
 * After a reset, provided that that the associated socket is still connected,
 * the TLS connection can be re-established with via the handshake() function.
 *
 * @param hTls (required) handle of the OS TLS API context
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_ABORTED if the re-set failed
 */
OS_Error_t
OS_Tls_reset(
    OS_Tls_Handle_t hTls);

/**
 * @brief Free a TLS object.
 *
 * Frees the memory allocted with a TLS object.
 *
 * @param hTls (required) handle of the OS TLS API context
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid (e.g.,
 *  NULL pointer, invalid sizes, etc.)
 */
OS_Error_t
OS_Tls_free(
    OS_Tls_Handle_t hTls);

/**
 * @brief Get mode of TLS API.
 *
 * @param hTls (required) handle of the OS TLS API context
 *
 * @return mode the TLS API is used in
 */
OS_Tls_Mode_t
OS_Tls_getMode(
    OS_Tls_Handle_t hTls);

/** @} */