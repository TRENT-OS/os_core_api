/**
 *  Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 * @file OS_Network.h
 * @defgroup OS_Network OS Network API
 * @brief TRENTOS-M Network Socket API. These API interacts with the Network
 *        Stack to offer IP functionality to the system using a socket based
 *        interface.
 * @details Include system header for TRENTOS-M Network Socket API. The
 *          Network stack can be used in two different ways, in a SERVER
 *          configuration (where the APP listens for an incoming connection)
 *          and a CLIENT configuration (where the APP can connect to a remote
 *          host).\n\n
 *          The Network Stack is implemented as a CAmkES component and in
 *          order to use it you must instantiate at least one such component
 *          and additionally an interface file and a CAmkES component file must
 *          be created/configured. \n\n The reference implementation can be
 *          found in the test_network_api folder in tests.\n
 *
 *          <b> Dependencies </b> \n
 *          The Network depends on a network interface driver, the
 *          configuration server and the system libraries.\n\n
 *
 *          <b> Network interface file: </b> \n
 *          The CAmkES interface implementation can be found in
 *          if_OS_socket.camkes file.
 *
 *          <b> CAmkES configuration file: </b> \n
 *          In order to use the Stack, your component definition needs to
 *          include the following.
 *          \code
 *              component <COMPONENT_NAME> {
 *                      //Reference to the RPC socket interface
 *                      uses      if_OS_socket    network_stack_rpc;
 *                      //A buffer to send data to/from the stack
 *                     dataport  Buf             buffer;
 *                      //Event received when the stack is ready to be used
 *                      consumes  ServiceReady    event_init_done;
 *              }
 *          \endcode
 *
 *          <b> CMakeLists.txt file: </b> \n
 *          The component must be build with either of the 2 "OS_NWSTACK_AS_" defines.
 *          \code
 *              Define                            | Description
 *              ----------------------------------|---------------------------------------------------------------------------
 *               OS_NWSTACK_AS_CLIENT             | use the Network stack as client (connect to remote host)
 *               OS_NWSTACK_AS_SERVER             | use the Network stack as server (accept connection from remote host)
 *               OS_NETWORK_STACK_USE_CONFIGSERVER| use configuration server to configure IP addresses
 *          \endcode
 *
 *          <b> Limitations: </b> \n
 *          + Only IPv4 connections are supported
 *          + Only TCP socket is supported
 *          + Only 1 connection can be active at a given time
 *          + Each APP needs its own Network Stack
 *          + Network Stack needs to be specialized in Server/Client
 * @{
 */

#pragma once

#include "OS_Error.h"
#include <stdint.h>

/**
 * @brief   contains elements that must be filled
 *          out by the APP to configure it to listen on a given port.
*/
typedef struct
{
    unsigned domain;      /**< domain is as of now AF_INET (IPv4)*/
    unsigned type;        /**< type is as of now SOCK_STREAM (TCP)*/
    uint16_t listen_port; /**< port to listen to e.g. 5555 */
    unsigned backlog;     /**< # of connections accepted (currently fixed =1)*/
} OS_NetworkServer_Socket_t;

/**
 * @brief   contains elements that must be filled by APP
 *          to connect to a remote host.
*/

typedef struct
{
    unsigned domain;  /**< domain is as of now AF_INET (IPv4) */
    unsigned type;    /**< type is as of now SOCK_STREAM (TCP) */
    const char* name; /**< IP addr to connect to (e.g. "10.0.0.1" ) */
    uint16_t port;    /**< port is for e.g. HTTP port 80 */
} OS_Network_Socket_t;

#if !defined(CUSTOM_OS_NETWORK_STACK_API_TYPES)

// Protocol families.
/** IPv4 */
#define OS_AF_INET   2 // IP protocol family.
/** IPv6 */
#define OS_AF_INET6 10 // IP version 6.

// Socket definition
/** Stream socket - TCP */
#define OS_SOCK_STREAM 2
/** Datagram socket - UDP */
#define OS_SOCK_DGRAM  1

typedef void* OS_Network_Context_t;
typedef unsigned OS_NetworkSocket_Handle_t;
typedef unsigned OS_NetworkServer_Handle_t;

#endif

/**
 * @details Create a socket and connect to it
 *
 * @param[in] ctx:  Runtime context of the APP. As of now
 * NULL.
 *
 * @param[in] pClientStruct: Configuration parameters for the socket to
 * be created.
 *
 * @param[out] phandle: Handle to the created socket.
 *
 * @return  OS_SUCCESS or OS_ERROR
 */

OS_Error_t
OS_NetworkSocket_create(
    OS_Network_Context_t       ctx,
    OS_Network_Socket_t*       pClientStruct,
    OS_NetworkSocket_Handle_t* phandle);

/**
 * @details Create a server socket, binds to a
 * port and listen for incoming connections
 *
 * @param[in] ctx:  Runtime context of the APP. As of now
 * NULL.
 *
 * @param[in] pServerStruct: Configuration parameters for the
 * server socket to be created.
 *
 * @param[out] pSrvHandle: Handle to the created socket.
 *
 * @return  OS_SUCCESS or OS_ERROR
 */

OS_Error_t
OS_NetworkServerSocket_create(
    OS_Network_Context_t       ctx,
    OS_NetworkServer_Socket_t* pServerStruct,
    OS_NetworkServer_Handle_t* pSrvHandle);
/**
 * @details Closes a NetworkServer socket. No
 * further socket communication is possible after closure.
 *
 * @param[in] srvHandle: Handle to the server socket we want
 * to close.
 *
 * @return  OS_SUCCESS or OS_ERROR
 */

OS_Error_t
OS_NetworkServerSocket_close(
    OS_NetworkServer_Handle_t srvHandle);

/**
 * @details Closes a network socket. No further
 * socket communication is possible after closure.
 *
 * @param[in] handle: Handle to the socket we want to close.
 *
 * @return OS_SUCCESS or OS_ERROR
 *
 */

OS_Error_t
OS_NetworkSocket_close(
    OS_NetworkSocket_Handle_t handle);

/**
 * @details Writes to a connected network socket.
 *
 * @param[in] handle: Handle to the socket to be used.
 *
 * @param[in] buf: Buffer containing data to be written.
 *
 * @param[in] len: Length of data to be written.
 *
 * @return Actual Number of Bytes written or OS_ERROR
 *
 */
OS_Error_t
OS_NetworkSocket_write(
    OS_NetworkSocket_Handle_t handle,
    const void*               buf,
    size_t*                   len);

/**
 * @details Accept incoming connections.
 *
 * @param[in] srvHandle: Handle used to create server socket.
 *
 * @param[out] phSocket: Network Socket containing the data
 * needed for further communication with the connecting host. All subsequent
 * read/write commands in this conversation need this handle.
 *
 * @return OS_SUCCESS or OS_ERROR
 */

OS_Error_t
OS_NetworkServerSocket_accept(
    OS_NetworkServer_Handle_t  srvHandle,
    OS_NetworkSocket_Handle_t* phSocket);

/**
 * @details Read data from connected socket.

 * @param[in] handle: Used to create/open socket

 * @param[in] buf: Buffer to read data into
 *
 * @param[in,out] len: Indicates how much data to read. After read it indicates how much was actually read
 *
 * @return OS_Error_t, following combinations with value of len

 * OS_ERROR_CONNECTION_CLOSED and length = 0, connection closed\n
 * OS_ERROR_GENERIC  error during read\n
 * OS_SUCCESS and len = 0, nothing read, connection still established\n
 * OS_SUCCESS and len > 0, data read, connection still established\n
 */

OS_Error_t
OS_NetworkSocket_read(
    OS_NetworkSocket_Handle_t handle,
    void*                     buf,
    size_t*                   len);
/** @} */
