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
 * Maxmimum size of PEM-encoded CA cert we accept. This is used to allocate a
 * static buffer in the config struct and for now set such it may hold ONE large
 * PEM-encoded certificate.
 */
#define SeosTlsLib_SIZE_CA_CERT_MAX    3072
/**
 * Max values to enable static array allocation; we do not actually provide as
 * many ciphersuites, yet.
 */
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
 * Digest algorithms available; these need to match the values of the underlying
 * TLS provider library
 */
typedef enum
{
    SeosTlsLib_Digest_NONE     = 0x00,
    /**
     * Use SHA256 as hash algorithm.
     */
    SeosTlsLib_Digest_SHA256   = 0x06,
} SeosTlsLib_Digest;

/**
 * Cipher suites available; these need to match the values of the underlying
 * TLS provider library
 */
typedef enum
{
    SeosTlsLib_CipherSuite_NONE                              = 0x0000,
    /**
     * Use DHE_RSA_WITH_AES_128_GCM_SHA256 ciphersuite.
     */
    SeosTlsLib_CipherSuite_DHE_RSA_WITH_AES_128_GCM_SHA256   = 0x009e,
    /**
     * Use ECDHE_RSA_WITH_AES_128_GCM_SHA256 ciphersuite.
     */
    SeosTlsLib_CipherSuite_ECDHE_RSA_WITH_AES_128_GCM_SHA256 = 0xc02f
} SeosTlsLib_CipherSuite;

/**
 * Flags for setting options in the TLS provider lib.
 */
typedef enum
{
    SeosTlsLib_Flag_NONE          = (1u << 0),
    /**
     *  Produce debug output from underlying protocol provider
     */
    SeosTlsLib_Flag_DEBUG         = (1u << 1),
    /**
     * Do not attempt to validate server certificate. This is dangerous,
     * so you better know what you are doing!
     */
    SeosTlsLib_Flag_NO_VERIFY     = (1u << 2)
} SeosTlsLib_Flag;

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
    SeosTlsLib_Digest sessionDigests[SeosTlsLib_MAX_DIGESTS + 1];
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
    SeosTlsLib_Digest signatureDigests[SeosTlsLib_MAX_DIGESTS + 1];
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
} SeosTlsLib_Policy;

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
         * Policy can be NULL, then it is set automatically based on the ciphersuites
         * chosen by the user.
         */
        SeosTlsLib_Policy* policy;
        /**
         * Need an initialized for SEOS Crypto API handle for cryptographic
         * operations.
         */
        SeosCryptoApiH handle;
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
         * NOTE: We add +1 so the last element can be set to 0 internally.
         */
        SeosTlsLib_CipherSuite cipherSuites[SeosTlsLib_MAX_CIPHERSUITES + 1];
        /**
         * Amount of ciphersuites in \p cipherSuites.
         */
        size_t cipherSuitesLen;
    } crypto;
    SeosTlsLib_Flag flags;
} SeosTlsLib_Config;


/**
 * Mode the TLS API instance should be operated in.
 */
typedef enum
{
    SeosTlsApi_Mode_NONE = 0,
    /**
     * Use TLS module as library, all calls to the API will internally mapped to
     * be executed locally.
     */
    SeosTlsApi_Mode_LIBRARY,
    /**
     * Use as RPC Server; can only be accessed through an appropriately configured
     * RPC client instance of the TLS API.
     */
    SeosTlsApi_Mode_RPC_SERVER,
    /**
     * Forward all TLS API calls to a remote instance of the TLS API where the
     * TLS library is running in RPC Server mode. This allows for isolation of
     * the actual TLS protocol stack.
     */
    SeosTlsApi_Mode_RPC_CLIENT
} SeosTlsApi_Mode;

typedef struct SeosTlsApi SeosTlsApi;
/**
 * Handle of the main TLS API context.
 */
typedef SeosTlsApi* SeosTlsApiH;

/**
 * Configuration for TLS API in RPC Server mode.
 */
typedef struct
{
    /**
     * Configuration of the local TLS library instance.
     */
    SeosTlsLib_Config library;
    /**
     * Dataport to use for communication with the RPC Client.
     */
    void* dataport;
} SeosTlsRpc_Server_Config;

/**
 * Configuration for TLS API in RPC Client mode.
 */
typedef struct
{
    /**
     * Dataport to use for communication with the RPC Server.
     */
    void* dataport;
} SeosTlsRpc_Client_Config;

/**
 * Configuration of the TLS API. The mode value defines which of the union fields
 * needs to be filled in:
 *   SeosTlsApi_Mode_LIBRARY:       config.library
 *   SeosTlsApi_Mode_CLIENT:        config.client
 *   SeosTlsApi_Mode_SERVER:        config.server
 */
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
 * @brief Initialize TLS API.
 *
 * Set up the TLS API by providing a \p cfg parameter which holds the APIs
 * configuration.
 *
 * The TLS API can be configured to operate in three modes:
 * - Library:     Here all calls to handshake/read/write will be executed within the
 *                component of the caller.
 * - RPC Client:  Here, an RPC client is configured which passes all calls to
 *                handshake()/read()/write() to an RPC server that resides in a
 *                different component.
 * - RPC server:  This is the "other half" of the RPC system, which takes calls
 *                from the RPC client. Calling handshake()/read()/write() locally
 *                on an API instance in this configuration will not work.
 *
 * NOTE: The TLS API expects a pre-initialized SEOS Crypto API instance.
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
 * @brief Write to TLS connection.
 *
 * After handshake has been completed, this function can be used to write data
 * to the server via an established TLS tunnel.
 *
 * NOTE: Internally, it will use the callback provided via the config struct to
 *       write() data to the socket.
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
 * @brief Read from a TLS connection.
 *
 * After handshake has been completed, this function can be used to read from an
 * established TLS connection. The server may close the session, this will be
 * signaled by returning no error, but setting \p dataSize to zero.
 *
 * NOTE: Internally, it will use the callback provided via the config struct to
 *       read() data from the socket.
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
 * @brief Reset a TLS connection.
 *
 * Reset a TLS API context that has been already through a successful handshake()
 * and possibly multiple read()/write() calls.
 *
 * After a reset, provided that that the associated socket is still connected,
 * the TLS connection can be re-established with via the handshake() function.
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
 * @brief Free a TLS object.
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
 * @brief Get mode of TLS API.
 *
 * @param hTls (required) handle of the SEOS TLS API context
 *
 * @return mode the TLS API is used in
 */
SeosTlsApi_Mode
SeosTlsApi_getMode(
    SeosTlsApiH hTls);

/** @} */