/*
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

// TODO: The section below needs to be updated in its content and form to match
// to our doxygen guidelines.
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
#include "interfaces/if_OS_Socket.h"
#include "network/OS_Network_types.h"
#include <stdint.h>
#include <stddef.h>


// TODO: We should discuss if we need to hide this struct in the implementation
// and just have the typedef in the API without showing the implementation
// (similar as its done for OS_Crypto for example).
typedef struct
{
    if_OS_Socket_t ctx;      /**< Pointer to the RPC vtable used */
    int            handleID; /**< Handle id*/
} OS_NetworkSocket_Handle_t;

#define OS_NetworkSocket_Handle_INVALID                                        \
    (OS_NetworkSocket_Handle_t) { .ctx = {NULL}, .handleID = -1 }

/**
 * Create a socket.
 *
 * @retval OS_SUCCESS                        Operation was successful.
 * @retval OS_ERROR_NETWORK_PROTO_NO_SUPPORT If the passed domain or type
 *                                           are unsupported.
 * @retval OS_ERROR_NETWORK_UNREACHABLE      If the network is unreachable.
 * @retval OS_ERROR_INSUFFICIENT_SPACE       If no free sockets could be
 *                                           found.
 * @retval OS_ERROR_INVALID_PARAMETER        If one of the passed parameters is
 *                                           NULL or the handle context is
 *                                           invalid.
 * @retval OS_ERROR_NOT_INITIALIZED          If the function was called before
 *                                           the network stack was fully
 *                                           initialized.
 * @retval other                             Each component implementing
 *                                           this might have additional
 *                                           error codes.
 *
 * @param[in] ctx     Interface context that should be used with the handle.
 * @param[in] phandle Handle that will be assigned to the created socket.
 * @param[in] domain  Domain of the socket that should be created.
 * @param[in] type    Type of the socket that should be created.
 */
OS_Error_t
OS_NetworkSocket_create(
    const if_OS_Socket_t* const      ctx,
    OS_NetworkSocket_Handle_t* const phandle,
    const int                        domain,
    const int                        type);

/**
 * Connect a socket to a specified address.
 *
 * @retval OS_SUCCESS                        Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE           If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER        If the passed address is invalid or
 *                                           the handle context is invalid.
 * @retval OS_ERROR_NETWORK_PROTO_NO_SUPPORT If the protocol is not supported.
 * @retval OS_ERROR_NETWORK_HOST_UNREACHABLE If the host is not unreachable.
 * @retval OS_ERROR_NOT_INITIALIZED          If the function was called
 *                                           before the network stack was fully
 *                                           initialized.
 * @retval other                             Each component implementing this
 *                                           might have additional error codes.
 *
 * @param[in] handle  Handle of the socket to connect.
 * @param[in] dstAddr Address of the destination to connect to.
 */
OS_Error_t
OS_NetworkSocket_connect(
    const OS_NetworkSocket_Handle_t handle,
    const OS_NetworkSocket_Addr_t*  dstAddr);

/**
 * Listen for connections on an opened and bound socket.
 *
 * @retval OS_SUCCESS                          Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE             If an invalid handle was passed.
 * @retval OS_ERROR_NETWORK_CONN_ALREADY_BOUND If the socket is already
 *                                             connected.
 * @retval OS_ERROR_INVALID_PARAMETER          If the handle context is invalid.
 * @retval OS_ERROR_NOT_INITIALIZED            If the function was called
 *                                             before the network stack was
 *                                             fully initialized.
 * @retval other                               Each component implementing this
 *                                             might have additional error
 *                                             codes.
 *
 * @param[in] handle  Handle of the socket to listen on.
 * @param[in] backlog Sets the maximum size to which the queue of pending
 *                    connections may grow.
 */
OS_Error_t
OS_NetworkSocket_listen(
    const OS_NetworkSocket_Handle_t handle,
    const int                       backlog);

/**
 * Accept the next connection request on the queue of pending connections
 * for the listening socket.
 *
 * @retval OS_SUCCESS                  Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE     If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER  If the handle context is invalid.
 * @retval OS_ERROR_TRY_AGAIN          If the resource is temporarily
 *                                     unavailable and the caller should try
 *                                     again.
 * @retval OS_ERROR_NOT_INITIALIZED    If the function was called
 *                                     before the network stack was fully
 *                                     initialized.
 * @retval OS_ERROR_CONNECTION_CLOSED  Connection was closed by remote before it
 *                                     was accepted by the local host.
 * @retval OS_ERROR_INSUFFICIENT_SPACE If no free sockets could be found.
 *
 * @retval other                    Each component implementing this might
 *                                  have additional error codes.
 *
 * @param[in]  handle         Handle of the listening socket.
 * @param[out] pClientHandle  Handle that will be used to map the accepted
 *                            connection to.
 * @param[out] srcAddr        Address of the accepted socket.
 */
OS_Error_t
OS_NetworkSocket_accept(
    const OS_NetworkSocket_Handle_t  handle,
    OS_NetworkSocket_Handle_t* const pClientHandle,
    OS_NetworkSocket_Addr_t* const   srcAddr);

/**
 * Read data from a socket. This function checks whether or not the socket
 * is bound and connected before it attempts to receive data.
 *
 * @retval OS_SUCCESS                     Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE        If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER     If the requested length exceeds
 *                                        the dataport size or the handle
 *                                        context is invalid.
 * @retval OS_ERROR_IO                    If there is an input/output error.
 * @retval OS_ERROR_CONNECTION_CLOSED     If the connection got shut down.
 * @retval OS_ERROR_NOT_INITIALIZED       If the function was called
 *                                        before the network stack was fully
 *                                        initialized.
 * @retval OS_ERROR_TRY_AGAIN             If the resource is temporarily
 *                                        unavailable or the request would
 *                                        block and the caller should try again.
 *                                        try again.
 * @retval other                          Each component implementing this
 *                                        might have additional error codes.
 *
 * @param[in]  handle       Handle of the socket to read from.
 * @param[in]  buf          Buffer to store the read data.
 * @param[in]  requestedLen Length of the data that should be read.
 * @param[out] actualLen    Actual length that was read from the socket.
 */
OS_Error_t
OS_NetworkSocket_read(
    const OS_NetworkSocket_Handle_t handle,
    void* const                     buf,
    size_t                          requestedLen,
    size_t* const                   actualLen);

/**
 * Receive data from a specified socket. This operation checks if the socket
 * is bound but not if it is connected and is therefore not
 * connection-oriented.
 *
 * @retval OS_SUCCESS                          Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE             If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER          If the handle context is invalid.
                                               or the requested length exceeds
 *                                             the dataport size.
 * @retval OS_ERROR_NETWORK_CONN_SHUTDOWN      If the connection got shut down.
 * @retval OS_ERROR_NETWORK_ADDR_NOT_AVAILABLE If the address is not available.
 * @retval OS_ERROR_NOT_INITIALIZED            If the function was called
 *                                             before the network stack was
 *                                             fully initialized.
 * @retval OS_ERROR_TRY_AGAIN                  If the resource is temporarily
 *                                             unavailable or the request would
 *                                             block and the caller should try
 *                                             again.
 * @retval other                               Each component implementing this
 *                                             might have additional error
 *                                             codes.
 *
 * @param[in]  handle       Handle to a previously created socket.
 * @param[in]  buf          Buffer to store the read data.
 * @param[in]  requestedLen Length of the data that should be read.
 * @param[out] actualLen    Actual length that was read from the socket.
 * @param[out] srcAddr      Address of the source socket that data was received
 *                          from.
 */
OS_Error_t
OS_NetworkSocket_recvfrom(
    const OS_NetworkSocket_Handle_t handle,
    void* const                     buf,
    size_t                          requestedLen,
    size_t* const                   actualLen,
    OS_NetworkSocket_Addr_t* const  srcAddr);

/**
 * Write data on a socket. This function checks if the socket is bound,
 * connected and that it isn't shutdown locally.
 *
 * @retval OS_SUCCESS                          Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE             If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER          If NULL is passed as a buffer
 *                                             pointer, the handle context is
 *                                             invalid or the requested length
 *                                             exceeds the dataport size.
 * @retval OS_ERROR_IO                         If there is an input/output
 *                                             error.
 * @retval OS_ERROR_NETWORK_CONN_NONE          If the socket is not connected.
 * @retval OS_ERROR_NETWORK_CONN_SHUTDOWN      If the connection got shut down.
 * @retval OS_ERROR_NETWORK_ADDR_NOT_AVAILABLE If the address is not available.
 * @retval OS_ERROR_NETWORK_HOST_UNREACHABLE   If the host is not unreachable.
 * @retval OS_ERROR_INSUFFICIENT_SPACE         If there is not enough space.
 * @retval OS_ERROR_TRY_AGAIN                  If the resource is temporarily
 *                                             unavailable or the request would
 *                                             block and the caller should try
 *                                             again.
 * @retval OS_ERROR_NOT_INITIALIZED            If the function was called
 *                                             before the network stack was
 *                                             fully initialized.
 * @retval other                               Each component implementing this
 *                                             might have additional error
 *                                             codes.
 *
 * @param[in]  handle       Handle of the socket to write on.
 * @param[in]  buf          Buffer containing data that should be sent.
 * @param[in]  requestedLen Amount of data that should be written.
 * @param[out] actualLen    Actual length that was written on the socket.
 */
OS_Error_t
OS_NetworkSocket_write(
    const OS_NetworkSocket_Handle_t handle,
    const void* const               buf,
    const size_t                    requestedLen,
    size_t* const                   actualLen);

/**
 * Send data on a destination socket without checking if the destination is
 * connected or not and is therefore not connection-oriented.
 *
 * @retval OS_SUCCESS                          Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE             If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER          If the handle context is invalid
 *                                             or the requested length exceeds
 *                                             the dataport size.
 * @retval OS_ERROR_NETWORK_ADDR_NOT_AVAILABLE If the address is not available.
 * @retval OS_ERROR_NETWORK_HOST_UNREACHABLE   If the host is not unreachable.
 * @retval OS_ERROR_INSUFFICIENT_SPACE         If there is not enough space.
 * @retval OS_ERROR_TRY_AGAIN                  If the resource is temporarily
 *                                             unavailable or the request would
 *                                             block and the caller should try
 *                                             again.
 * @retval OS_ERROR_NOT_INITIALIZED            If the function was called
 *                                             before the network stack was
 *                                             fully initialized.
 * @retval other                               Each component implementing this
 *                                             might have additional error
 *                                             codes.
 *
 * @param[in]  handle        Handle to a previously created socket.
 * @param[in]  buf           Buffer containing data to be written.
 * @param[in]  requestedLen  Length of the data that should be written on the
 *                           destination socket.
 * @param[out] actualLen     Actual length that was written on the socket.
 * @param[in]  dstAddr       Address of the destination socket to send data on.
 */
OS_Error_t
OS_NetworkSocket_sendto(
    const OS_NetworkSocket_Handle_t      handle,
    const void* const                    buf,
    size_t                               requestedLen,
    size_t* const                        actualLen,
    const OS_NetworkSocket_Addr_t* const dstAddr);

/**
 * Bind a specified local IP-address and port to a socket.
 *
 * @retval OS_SUCCESS                  Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE     If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER  If the handle context is invalid or the
                                       requested length exceeds the dataport
                                       size.
 * @retval OS_ERROR_IO                 If the specified address can not be
 *                                     found.
 * @retval OS_ERROR_INSUFFICIENT_SPACE If there is not enough space.
 * @retval OS_ERROR_NOT_INITIALIZED    If the function was called
 *                                     before the network stack was fully
 *                                     initialized.
 * @retval other                       Each component implementing this might
 *                                     have additional error codes.
 *
 * @param[in] handle    Handle of the socket to bind.
 * @param[in] localAddr Local address to bind the socket to.
 */
OS_Error_t
OS_NetworkSocket_bind(
    const OS_NetworkSocket_Handle_t      handle,
    const OS_NetworkSocket_Addr_t* const localAddr);

/**
 * Query the current state of the Network Stack component.
 *
 * @retval UNINITIALIZED Network Stack is uninitialized.
 * @retval INITIALIZED   Network Stack is initialized.
 * @retval RUNNING       Network Stack is running.
 * @retval FATAL_ERROR   Network Stack has experienced a fatal error.
 *
 * @param[in] ctx Interface context that should be used to query the respective
 *                Network Stack component.
 */
OS_NetworkStack_State_t
OS_NetworkSocket_getStatus(
    const if_OS_Socket_t* const ctx);

/**
 * Get the events for the opened sockets socket communication is possible after
 * closure.
 *
 * @retval OS_SUCCESS                 Operation was successful.
 * @retval OS_ERROR_ABORTED           If an internal error occurred.
 * @retval OS_ERROR_INVALID_HANDLE    If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER If the handle context is invalid.
 * @retval OS_ERROR_NOT_INITIALIZED   If the function was called before the
 *                                    network stack was fully initialized.
 * @retval other                      Each component implementing this might
 *                                    have additional error codes.
 *
 * @param[in]  ctx            Interface context that should be used with the
 *                            handle.
 * @param[in]  buf            Buffer to store the event data.
 * @param[in]  bufSize        Size of the buffer to store the event data.
 * @param[out] numberOfEvents Will be overwritten with the number of events.
 */
OS_Error_t
OS_NetworkSocket_getPendingEvents(
    const if_OS_Socket_t* const ctx,
    void* const                 buf,
    const size_t                bufSize,
    int* const                  numberOfEvents);

/**
 * Block until a notification event gets received.
 *
 * @retval OS_SUCCESS                 Notification event found.
 * @retval OS_ERROR_INVALID_PARAMETER If the handle context is invalid.
 * @retval other                      Each component implementing this might
 *                                    have additional error codes.
 *
 * @param[in] ctx Interface context that should be used with the
 *                handle.
 */
OS_Error_t
OS_NetworkSocket_wait(
    const if_OS_Socket_t* const ctx);

/**
 * Check whether there is a pending notification.
 *
 * @retval OS_SUCCESS                 Notification event found.
 * @retval OS_ERROR_INVALID_PARAMETER If the handle context is invalid.
 * @retval OS_ERROR_TRY_AGAIN         If no notification event was found.
 * @retval other                      Each component implementing this might
 *                                    have additional error codes.
 *
 * @param[in] ctx Interface context that should be used with the
 *                handle.
 */
OS_Error_t
OS_NetworkSocket_poll(
    const if_OS_Socket_t* const ctx);

/**
 * Register a callback function that should be invoked with the provided
 * argument when a notification event is received.
 *
 * @retval OS_SUCCESS                 Callback successfully registered.
 * @retval OS_ERROR_GENERIC           If the callback could not be registered.
 * @retval OS_ERROR_INVALID_PARAMETER If the handle context is invalid.
 * @retval other                      Each component implementing this might
 *                                    have additional error codes.
 *
 * @param[in] ctx      Interface context that should be used with the
 *                     handle.
 * @param[in] callback Function pointer to the function that should be invoked.
 * @param[in] arg      Argument that the registered function should be invoked
 *                     with.
 */
OS_Error_t
OS_NetworkSocket_regCallback(
    const if_OS_Socket_t* const ctx,
    void (*callback)(void*),
    void* arg);

/**
 * Closes a network socket. No further socket communication is possible after
 * closure.
 *
 * @retval OS_SUCCESS                 Operation was successful.
 * @retval OS_ERROR_INVALID_HANDLE    If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER If the handle context is invalid.
 * @retval OS_ERROR_NOT_INITIALIZED   If the function was called before the
 *                                    network stack was fully initialized.
 * @retval other                      Each component implementing this might
 *                                    have additional error codes.
 *
 * @param[in] handle Handle of the socket that should be closed.
 */
OS_Error_t
OS_NetworkSocket_close(
    const OS_NetworkSocket_Handle_t handle);

/** @} */
