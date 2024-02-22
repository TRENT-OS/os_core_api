/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @file
 * @defgroup OS_Tls OS TLS API
 * @{
 * @brief OS TLS API library
 */

#pragma once

#include "OS_Crypto.h"
#include "OS_Dataport.h"

#include "interfaces/if_OS_Tls.h"

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
     * Use TLS module as library, all calls to the API will be internally mapped
     * and executed locally.
     */
    OS_Tls_MODE_LIBRARY,

    /**
     * Forward all TLS API calls to a remote instance of the TLS API via CAmkES
     * RPC. This allows for isolation of the actual TLS protocol stack.
     */
    OS_Tls_MODE_CLIENT
} OS_Tls_Mode_t;

/**
 * Digest algorithms available.
 */
typedef enum
{
    OS_Tls_DIGEST_NONE = 0,

    /**
     * Use SHA256 as hash algorithm.
     */
    OS_Tls_DIGEST_SHA256,

/// @cond INTERNAL
//------------------------------------------------------------------------------
    __OS_Tls_DIGEST_MAX
//------------------------------------------------------------------------------
/// @endcond
} OS_Tls_Digest_t;

/**
 * Cipher suites available
 */
typedef enum
{
    OS_Tls_CIPHERSUITE_NONE = 0,

    /**
     * Use DHE_RSA_WITH_AES_128_GCM_SHA256 ciphersuite.
     */
    OS_Tls_CIPHERSUITE_DHE_RSA_WITH_AES_128_GCM_SHA256,

    /**
     * Use ECDHE_RSA_WITH_AES_128_GCM_SHA256 ciphersuite.
     */
    OS_Tls_CIPHERSUITE_ECDHE_RSA_WITH_AES_128_GCM_SHA256,

/// @cond INTERNAL
//------------------------------------------------------------------------------
    __OS_Tls_CIPHERSUITE_MAX
//------------------------------------------------------------------------------
/// @endcond
} OS_Tls_CipherSuite_t;

/**
 * Flags for setting options in the TLS library.
 */
typedef enum
{
    OS_Tls_FLAG_NONE          = (1u << 0),

    /**
     * Produce debug output from underlying protocol provider.
     */
    OS_Tls_FLAG_DEBUG         = (1u << 1),

    /**
     * Do not attempt to authenticate the peer.
     * WARNING: Be careful when disabling peer authentication!
     * NOTE: In TLS usually per default a client requires to authenticate the
     * server, but a server does not require to authenticate the client.
     */
    OS_Tls_FLAG_NO_VERIFY     = (1u << 2),

    /**
     * In case the socket I/O indicates that an operation would block, do not
     * attempt to resume I/O but return OS_ERROR_WOULD_BLOCK.
     */
    OS_Tls_FLAG_NON_BLOCKING  = (1u << 3)
} OS_Tls_Flag_t;

/**
 * Special return codes for socket I/O, in case they would block on read or
 * write. Theses specific values are expected by mbedTLS, so they cannot be
 * changed.
 */
#define OS_Tls_SOCKET_READ_WOULD_BLOCK     -0x6900
#define OS_Tls_SOCKET_WRITE_WOULD_BLOCK    -0x6880

/// @cond INTERNAL
//------------------------------------------------------------------------------
// For now, we don't have many ciphersuites or digests, so uint8_t is enough.
typedef uint8_t OS_Tls_CipherSuite_Flags_t;
typedef uint8_t OS_Tls_Digest_Flags_t;
//------------------------------------------------------------------------------
/// @endcond

/**
 * For legacy reasons it may be important to override the param/algorithm
 * choices automatically derived from the chosen ciphersuites (where possible).
 */
typedef struct
{
    /**
     * Allow these digest algorithms to be used for generation of hashes
     * during the TLS handshake.
     */
    OS_Tls_Digest_Flags_t handshakeDigests;

    /**
     * Allow these digest algorithms to be used for the generation of hashes
     * on certificates.
     */
    OS_Tls_Digest_Flags_t certDigests;

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
 * Configuration for the TLS provider library used by the TLS API layer.
 */
typedef struct
{
    /**
     * Functions to use for socket I/O.
     */
    struct
    {
        /**
         * Used by the TLS library to receive data from a connected socket.
         * If NULL, a default function will be used. This function is based on
         * the OS_Network API and requires ctx to be of type
         * OS_Socket_Handle_t*.
         */
        int (*recv)(void* ctx, unsigned char* buf, size_t len);

        /**
         * Used by the TLS library to send data to a connected socket.
         * If NULL, a default function will be used. This function is based on
         * the OS_Network API and requires ctx to be of type
         * OS_Socket_Handle_t*.
         */
        int (*send)(void* ctx, const unsigned char* buf, size_t len);

        /**
         * This is a parameter which is passed into every call to send/recv.
         * Typically it would be a socket handle or similar.
         */
        void* context;
    } socket;

    /**
     * Configuration options related to cryptography.
     */
    struct
    {
        /**
         * Policy can be NULL, then it is set automatically based on the
         * ciphersuites chosen by the user.
         *
         * This will be copied on call to OS_Tls_init().
         */
        const OS_Tls_Policy_t* policy;

        /**
         * Need an initialized for OS Crypto API handle for cryptographic
         * operations.
         */
        OS_Crypto_Handle_t handle;

        /**
         * Here a list of CA certificates is passed to the TLS API in PEM
         * encoding (including headers) so it can be used to verify the root of
         * the peer's certificate chain.
         *
         * This will be copied on call to OS_Tls_init().
         */
        const char* caCerts;

        /**
         * Here the own certificate is passed to the TLS API in PEM encoding
         * (including headers) so that it can be used for authentication.
         *
         * This will be copied on call to OS_Tls_init().
         */
        const char* ownCert;

        /**
         * Here a private key is passed to the TLS API in PEM encoding
         * (including headers) so it can be used for authentication.
         *
         * This will be copied on call to OS_Tls_init().
         */
        const char* privateKey;

        /**
         * For simplicity, a user can just set some ciphersuites and be fine.
         * The hash given in the ciphersuites will be ENFORCED for everything
         * (incl. session hash, signature hashes etc.). Similary, the key size
         * of the AES key will be used to determine the minimum asymmetric key
         * lengths automatically, so all parameters and algorithms will be
         * internally consistent (as far as the suite allows it).
         */
        OS_Tls_CipherSuite_Flags_t cipherSuites;
    } crypto;

    /**
     * Flags to set for TLS.
     */
    OS_Tls_Flag_t flags;
} TlsLib_Config_t;

/// @cond INTERNAL
//------------------------------------------------------------------------------
typedef struct OS_Tls OS_Tls_t;
typedef OS_Tls_t* OS_Tls_Handle_t;
//------------------------------------------------------------------------------
/// @endcond

/**
 * Configuration of the TLS API. The mode defines which of the union fields
 * needs to be filled in.
 */
typedef struct
{
    /**
     * Mode to operate the API.
     */
    OS_Tls_Mode_t mode;

    /**
     * Configuration options for LIBRARY mode.
     */
    TlsLib_Config_t library;

    /**
     * CAmkES interface for remote RPC server to be used in CLIENT mode.
     */
    if_OS_Tls_t rpc;
} OS_Tls_Config_t;

/// @cond INTERNAL
//------------------------------------------------------------------------------
// Variadic macro, add more FE_x as the size of the underlying flag type
// increases. Currently we have uint8_t, so having 8 FE macros here is enough.
#define OS_Tls_FE_1(WHAT,F)     WHAT(F)
#define OS_Tls_FE_2(WHAT,F,...) WHAT(F) | OS_Tls_FE_1(WHAT,__VA_ARGS__)
#define OS_Tls_FE_3(WHAT,F,...) WHAT(F) | OS_Tls_FE_2(WHAT,__VA_ARGS__)
#define OS_Tls_FE_4(WHAT,F,...) WHAT(F) | OS_Tls_FE_3(WHAT,__VA_ARGS__)
#define OS_Tls_FE_5(WHAT,F,...) WHAT(F) | OS_Tls_FE_4(WHAT,__VA_ARGS__)
#define OS_Tls_FE_6(WHAT,F,...) WHAT(F) | OS_Tls_FE_5(WHAT,__VA_ARGS__)
#define OS_Tls_FE_7(WHAT,F,...) WHAT(F) | OS_Tls_FE_6(WHAT,__VA_ARGS__)
#define OS_Tls_FE_8(WHAT,F,...) WHAT(F) | OS_Tls_FE_7(WHAT,__VA_ARGS__)
// Select the right FE macro based on the number of input args.
#define OS_Tls_GET_FE( _1,_2,_3,_4,_5,_6,_7,_8,NAME,...)                \
    OS_Tls_ ## NAME
// Apply the "action macro" to each of the inputs.
#define OS_Tls_FOR_EACH(action, ...)                                    \
    OS_Tls_GET_FE(__VA_ARGS__,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1)  \
        (action, __VA_ARGS__)
// This is an "action macro" for: turn an ID into a flag bit (for uint8_t),
// add more as the underlying flag field increases in width.
#define OS_Tls_ID_TO_FLAGS_U8(id) ( (1u << (id)) )
//------------------------------------------------------------------------------
/// @endcond

/**
 * \brief Translate up to eight OS_Tls_CipherSuite_t values into a single
 *  OS_Tls_CipherSuite_Flags_t value.
 */
#define OS_Tls_CIPHERSUITE_FLAGS(...) \
    OS_Tls_FOR_EACH(OS_Tls_ID_TO_FLAGS_U8,__VA_ARGS__)

/**
 * \brief Translate up to eight OS_Tls_Digest_t values into a single
 *  OS_Tls_Digest_Flags_t value.
 */
#define OS_Tls_DIGEST_FLAGS(...) \
    OS_Tls_FOR_EACH(OS_Tls_ID_TO_FLAGS_U8,__VA_ARGS__)

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
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid
 *  (e.g., NULL pointer, invalid sizes, etc.)
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
 * The critical part of a TLS protocol run is performing the TLS handshake.
 * After a TLS object has been initialized, the handshake has to performed.
 *
 * Only after the handshake has been executed successfully, the read()/write()
 * functions can be called successfully.
 *
 * NOTE: Before this function is called, the socket handle passed via the config
 *       struct during init() must be ALREADY connected. The TLS API will never
 *       change the state of the socket explicitly, it will only call read()/
 *       write() for the transfer of protcol data.
 *
 * @param hTls (required) handle of the OS TLS API context
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid
 *  (e.g., NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_OPERATION_DENIED if the TLS session is already established
 * @retval OS_ERROR_WOULD_BLOCK if the socket send()/recv() signals during the
 *  handshake that it would block
 * @retval OS_ERROR_CONNECTION_CLOSED if the socket send()/recv() signals that
 *  the connection was closed by the other side during the handshake
 */
OS_Error_t
OS_Tls_handshake(
    OS_Tls_Handle_t hTls);

/**
 * @brief Write to TLS connection.
 *
 * After handshake has been completed, this function can be used to write data
 * to the peer via an established TLS tunnel.
 *
 * NOTE: Internally, it will use the callback provided via the config struct to
 *       write() data to the socket.
 *
 * @param hTls (required) handle of the OS TLS API context
 * @param data (required) pointer data buffer
 * @param dataSize (required) amount of bytes to write, will be set to the
 *  amount of bytes that have been sent (only valid if OS_SUCCESS is returned)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded (number of bytes returned by
 *  \p dataSize has been sent)
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid
 *  (e.g., NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_ABORTED if the write failed
 * @retval OS_ERROR_OPERATION_DENIED if the TLS session is not yet established
 * @retval OS_ERROR_WOULD_BLOCK if the socket send() signals during the write
 *  that it would block and the call has to be repeated (no partial data has
 *  been sent)
 * @retval OS_ERROR_CONNECTION_CLOSED if the socket send() signals that the
 *  connection was closed by the other side during the write
 */
OS_Error_t
OS_Tls_write(
    OS_Tls_Handle_t hTls,
    const void*     data,
    size_t*         dataSize);

/**
 * @brief Read from a TLS connection.
 *
 * After handshake has been completed, this function can be used to read from an
 * established TLS connection. The peer may close the session, this will be
 * signaled by returning OS_ERROR_CONNECTION_CLOSED.
 *
 * NOTE: Internally, it will use the callback provided via the config struct to
 *       read() data from the socket.
 *
 * @param hTls (required) handle of the OS TLS API context
 * @param data (required) pointer data buffer
 * @param dataSize (required) amount of bytes requested, will be set to the
 *  amount of bytes that could be read (only valid if OS_SUCCESS is returned)
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded (number of bytes returned by
 *  \p dataSize has been read)
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid
 *  (e.g., NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_ABORTED if the read failed
 * @retval OS_ERROR_OPERATION_DENIED if the TLS session is not yet established
 * @retval OS_ERROR_WOULD_BLOCK if the socket recv() signals during the read
 *  that it would block and the call has to be repeated (no partial data has
 *  been read)
 * @retval OS_ERROR_CONNECTION_CLOSED if the socket recv() signals that the
 *  connection was closed by the other side during the read
 */
OS_Error_t
OS_Tls_read(
    OS_Tls_Handle_t hTls,
    void*           data,
    size_t*         dataSize);

/**
 * @brief Reset a TLS connection.
 *
 * Reset a TLS API context that has been already through a successful
 * handshake() and possibly multiple read()/write() calls.
 *
 * After a reset, provided that that the associated socket is still connected,
 * the TLS connection can be re-established with via the handshake() function.
 *
 * @param hTls (required) handle of the OS TLS API context
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid
 *  (e.g., NULL pointer, invalid sizes, etc.)
 * @retval OS_ERROR_ABORTED if the re-set failed
 */
OS_Error_t
OS_Tls_reset(
    OS_Tls_Handle_t hTls);

/**
 * @brief Free a TLS object.
 *
 * Frees the memory allocated with a TLS object.
 *
 * @param hTls (required) handle of the OS TLS API context
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if one of the parameters was invalid
 *  (e.g., NULL pointer, invalid sizes, etc.)
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
