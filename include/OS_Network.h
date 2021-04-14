/*
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @defgroup OS_Network OS Network API
 * @brief    OS Network API library
 *
 * @details These API interacts with the Network
 *          Stack to offer IP functionality to the system using a socket based
 *          interface.
 *
 *          Include system header for TRENTOS-M Network Socket API. The
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
 *          if_OS_Socket.camkes file.
 *
 *          <b> CAmkES configuration file: </b> \n
 *          In order to use the Stack, your component definition needs to
 *          include the following.
 *          \code
 *              component <COMPONENT_NAME> {
 *                      //Reference to the RPC socket interface
 *                      uses      if_OS_Socket    networkStack_rpc;
 *                      //A buffer to send data to/from the stack
 *                     dataport  Buf             buffer;
 *              }
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
#include "network/OS_Network_types.h"
#include <stdint.h>

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
 * @return OS_SUCCESS or error code for network (OS_ERROR_NETWORK_*) or general
 * error (OS_ERROR_*)
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
 * @return OS_SUCCESS or error code for network (OS_ERROR_NETWORK_*) or general
 * error (OS_ERROR_*)
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
 * @return OS_SUCCESS or error code for network (OS_ERROR_NETWORK_*) or general
 * error (OS_ERROR_*)
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
 * @return OS_SUCCESS or error code for network (OS_ERROR_NETWORK_*) or general
 * error (OS_ERROR_*)
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
 * @param[in] requestedLen: Length of data to be written to the socket.
 *
 * @param[out] actualLen: Length of data that was actually written to the socket.
 *
 * @return OS_SUCCESS or error code for network (OS_ERROR_NETWORK_*) or general
 * error (OS_ERROR_*)
 *
 */

OS_Error_t
OS_NetworkSocket_write(
    OS_NetworkSocket_Handle_t handle,
    const void*               buf,
    size_t                    requestedLen,
    size_t*                   actualLen);

/**
 * @details Accept incoming connections.
 *
 * @param[in] srvHandle: Handle used to create server socket.
 *
 * @param[out] phSocket: Network Socket containing the data
 * needed for further communication with the connecting host. All subsequent
 * read/write commands in this conversation need this handle.
 *
 * @return OS_SUCCESS or error code for network (OS_ERROR_NETWORK_*) or general
 * error (OS_ERROR_*)
 */

OS_Error_t
OS_NetworkServerSocket_accept(
    OS_NetworkServer_Handle_t  srvHandle,
    OS_NetworkSocket_Handle_t* phSocket);

/**
 * @details Read data from connected socket.
 *
 * @param[in] handle: Handle used to create/open socket
 *
 * @param[in] buf: Buffer to read data into
 *
 * @param[in] requestedLen: Indicates how much data to read.
 *
 * @param[out] actualLen: Contains the number of bytes that were actually read
 *
 * @return OS_Error_t, following combinations with value of len
 *
 * OS_ERROR_CONNECTION_CLOSED and length = 0, connection closed\n
 * OS_ERROR_GENERIC  error during read\n
 * OS_SUCCESS and len = 0, nothing read, connection still established\n
 * OS_SUCCESS and len > 0, data read, connection still established\n
 */

OS_Error_t
OS_NetworkSocket_read(
    OS_NetworkSocket_Handle_t handle,
    void*                     buf,
    size_t                    requestedLen,
    size_t*                   actualLen);

/**
 * @details Receives one UDP packet of up to len bytes in size.
 * Excess data is discarded

 * @param[in] handle: Handle used to create/open socket

 * @param[in] buf: Buffer to read data into
 *
 * @param[in] requestedLen: Indicates how much data to read.
 *
 * @param[out] actualLen: After the function returns it contains how many bytes
 *  were read from the socket
 *
 * @param[out] src_socket: contains a socket with the information of the
 * remote host which sent the UDP frame
 *
 * @return OS_Error_t, following combinations with value of length

 * OS_ERROR_GENERIC  error during read\n
 * OS_SUCCESS and len > 0, data read\n
 */

OS_Error_t
OS_NetworkSocket_recvfrom(
    OS_NetworkSocket_Handle_t handle,
    void*                     buf,
    size_t                    requestedLen,
    size_t*                   actualLen,
    OS_Network_Socket_t*      src_socket);

/**
 * @details Sends one UDP packet of up to len bytes in size. Can send less data
 * than requested.
 *
 * @return OS_SUCCESS or error code for network (OS_ERROR_NETWORK_*) or general
 * error (OS_ERROR_*)
 *
 */
OS_Error_t
OS_NetworkSocket_sendto(
    OS_NetworkSocket_Handle_t handle, /**< [in] Handle to the used socket. */
    const void* buf, /**< [in] Buffer containing data to be written. */
    size_t requestedLen, /**< [in]  Length of data to be written. */
    size_t* actualLen, /**< [out] Number of bytes written to the socket. */
    OS_Network_Socket_t dst_socket /**< [in] Socket containing the information
                                             of the destination */);

/**
 * @details Binds the socket to the given port.
 *
 * @return OS_SUCCESS or error code for network (OS_ERROR_NETWORK_*) or general
 * error (OS_ERROR_*)
 *
 */

OS_Error_t
OS_NetworkSocket_bind(
    OS_NetworkSocket_Handle_t handle, //!< [in] Handle to the socket to be used.
    uint16_t receiving_port           //!< [in] Port to be used.
);

/** @} */
