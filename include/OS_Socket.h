/*
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 *
 * OS Socket API
 *
 * @ingroup OS_Socket
 * @defgroup OS_Socket The OS Socket API
 *
 * Contains all relevent types and functions for the OS Socket API.
 */

#pragma once

#include "OS_Error.h"
#include "interfaces/if_OS_Socket.h"

#include "network/OS_SocketTypes.h"
#include "network/OS_NetworkStackTypes.h"

#include <stdint.h>
#include <stddef.h>


typedef struct
{
    if_OS_Socket_t ctx;      /**< RPC vtable context */
    int            handleID; /**< Handle id*/
} OS_Socket_Handle_t;

#define OS_Socket_Handle_INVALID                                               \
    (OS_Socket_Handle_t) { .ctx = {NULL}, .handleID = -1 }

/**
 * Create a socket.
 *
 * @retval OS_SUCCESS                        Operation was successful.
 * @retval OS_ERROR_ABORTED                  If the Network Stack has
 *                                           experienced a fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED          If the function was called before
 *                                           the Network Stack was fully
 *                                           initialized.
 * @retval OS_ERROR_INVALID_PARAMETER        If one of the passed parameters is
 *                                           NULL or the handle context is
 *                                           invalid.
 * @retval OS_ERROR_NETWORK_PROTO_NO_SUPPORT If the passed domain or type
 *                                           are unsupported.
 * @retval OS_ERROR_NETWORK_UNREACHABLE      If the network is unreachable.
 * @retval OS_ERROR_INSUFFICIENT_SPACE       If no free sockets could be
 *                                           found.
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
OS_Socket_create(
    const if_OS_Socket_t* const ctx,
    OS_Socket_Handle_t* const   phandle,
    const int                   domain,
    const int                   type);

/**
 * Connect a socket to a specified address.
 *
 * @retval OS_SUCCESS                        Operation was successful.
 * @retval OS_ERROR_ABORTED                  If the Network Stack has
 *                                           experienced a fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED          If the function was called
 *                                           before the Network Stack was fully
 *                                           initialized.
 * @retval OS_ERROR_INVALID_HANDLE           If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER        If the passed address is invalid or
 *                                           the handle context is invalid.
 * @retval OS_ERROR_CONNECTION_CLOSED        If the socket connection was
 *                                           previously shut down.
 * @retval OS_ERROR_NETWORK_PROTO_NO_SUPPORT If the protocol is not supported.
 * @retval OS_ERROR_NETWORK_HOST_UNREACHABLE If the host is not unreachable.
 * @retval OS_ERROR_NETWORK_PROTO            If the function is called on the
 *                                           wrong socket type.
 * @retval other                             Each component implementing this
 *                                           might have additional error codes.
 *
 * @param[in] handle  Handle of the socket to connect.
 * @param[in] dstAddr Address of the destination to connect to.
 */
OS_Error_t
OS_Socket_connect(
    const OS_Socket_Handle_t handle,
    const OS_Socket_Addr_t*  dstAddr);

/**
 * Listen for connections on an opened and bound socket.
 *
 * @retval OS_SUCCESS                          Operation was successful.
 * @retval OS_ERROR_ABORTED                    If the Network Stack has
 *                                             experienced a fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED            If the function was called
 *                                             before the Network Stack was
 *                                             fully initialized.
 * @retval OS_ERROR_INVALID_HANDLE             If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER          If the handle context is invalid.
 * @retval OS_ERROR_CONNECTION_CLOSED          If the connection is in a closed
 *                                             state.
 * @retval OS_ERROR_NETWORK_CONN_ALREADY_BOUND If the socket is already
 *                                             connected.
 * @retval OS_ERROR_NETWORK_PROTO              If the function is called on the
 *                                             wrong socket type.
 * @retval other                               Each component implementing this
 *                                             might have additional error
 *                                             codes.
 *
 * @param[in] handle  Handle of the socket to listen on.
 * @param[in] backlog Sets the maximum size to which the queue of pending
 *                    connections may grow.
 */
OS_Error_t
OS_Socket_listen(
    const OS_Socket_Handle_t handle,
    const int                backlog);

/**
 * Accept the next connection request on the queue of pending connections
 * for the listening socket.
 *
 * @retval OS_SUCCESS                  Operation was successful.
 * @retval OS_ERROR_ABORTED            If the Network Stack has
 *                                     experienced a fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED    If the function was called before the
 *                                     Network Stack was fully initialized.
 * @retval OS_ERROR_INVALID_HANDLE     If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER  If the handle context is invalid.
 * @retval OS_ERROR_TRY_AGAIN          If the resource is temporarily
 *                                     unavailable and the caller should try
 *                                     again.
 * @retval OS_ERROR_NETWORK_PROTO      If the function is called on the
 *                                     wrong socket type.
 * @retval OS_ERROR_INSUFFICIENT_SPACE If no free sockets could be found.
 * @retval OS_ERROR_CONNECTION_CLOSED  If the connection was closed by remote
 *                                     before it was accepted by the local host.
 * @retval other                       Each component implementing this might
 *                                     have additional error codes.
 *
 * @param[in]  handle        Handle of the listening socket.
 * @param[out] pClientHandle Handle that will be used to map the accepted
 *                           connection to.
 * @param[out] srcAddr       Address of the accepted socket.
 */
OS_Error_t
OS_Socket_accept(
    const OS_Socket_Handle_t  handle,
    OS_Socket_Handle_t* const pClientHandle,
    OS_Socket_Addr_t* const   srcAddr);

/**
 * Read data from a socket. This function checks whether or not the socket
 * is bound and connected before it attempts to receive data.
 *
 * @retval OS_SUCCESS                     Operation was successful.
 * @retval OS_ERROR_ABORTED               If the Network Stack has
 *                                        experienced a fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED       If the function was called before the
 *                                        Network Stack was fully initialized.
 * @retval OS_ERROR_INVALID_HANDLE        If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER     If the requested length exceeds
 *                                        the dataport size or the handle
 *                                        context is invalid.
 * @retval OS_ERROR_IO                    If there is an input/output error.
 * @retval OS_ERROR_NETWORK_CONN_NONE     If no connection is established when
 *                                        calling this function.
 * @retval OS_ERROR_NETWORK_PROTO         If the function is called on the wrong
 *                                        socket type.
 * @retval OS_ERROR_TRY_AGAIN             If the resource is temporarily
 *                                        unavailable or the request would
 *                                        block and the caller should try again.
 * @retval OS_ERROR_CONNECTION_CLOSED     If the connection is in a closed
 *                                        state.
 * @retval OS_ERROR_NETWORK_CONN_SHUTDOWN If the connection got shut down.
 * @retval other                          Each component implementing this
 *                                        might have additional error codes.
 *
 * @param[in]  handle       Handle of the socket to read from.
 * @param[in]  buf          Buffer to store the read data.
 * @param[in]  requestedLen Length of the data that should be read.
 * @param[out] actualLen    Actual length that was read from the socket.
 */
OS_Error_t
OS_Socket_read(
    const OS_Socket_Handle_t handle,
    void* const              buf,
    size_t                   requestedLen,
    size_t* const            actualLen);

/**
 * Receive data from a specified socket. This operation checks if the socket
 * is bound but not if it is connected and is therefore not
 * connection-oriented.
 *
 * @retval OS_SUCCESS                          Operation was successful.
 * @retval OS_ERROR_ABORTED                    If the Network Stack has
 *                                             experienced a fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED            If the function was called before
 *                                             the Network Stack was fully
 *                                             initialized.
 * @retval OS_ERROR_INVALID_HANDLE             If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER          If the handle context is invalid
 *                                             or the requested length exceeds
 *                                             the dataport size.
 * @retval OS_ERROR_NETWORK_PROTO              If the function is called on the
 *                                             wrong socket type.
 * @retval OS_ERROR_TRY_AGAIN                  If the resource is temporarily
 *                                             unavailable or the request would
 *                                             block and the caller should try
 *                                             again.
 * @retval OS_ERROR_CONNECTION_CLOSED          If no further communication is
 *                                             possible on the socket.
 * @retval OS_ERROR_NETWORK_CONN_SHUTDOWN      If the connection got shut down.
 * @retval OS_ERROR_NETWORK_ADDR_NOT_AVAILABLE If the address is not available.
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
OS_Socket_recvfrom(
    const OS_Socket_Handle_t handle,
    void* const              buf,
    size_t                   requestedLen,
    size_t* const            actualLen,
    OS_Socket_Addr_t* const  srcAddr);

/**
 * Write data on a socket. This function checks if the socket is bound,
 * connected and that it isn't shutdown locally.
 *
 * @retval OS_SUCCESS                          Operation was successful.
 * @retval OS_ERROR_ABORTED                    If the Network Stack has
 *                                             experienced a fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED            If the function was called before
 *                                             the Network Stack was fully
 *                                             initialized.
 * @retval OS_ERROR_INVALID_HANDLE             If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER          If NULL is passed as a buffer
 *                                             pointer, the handle context is
 *                                             invalid or the requested length
 *                                             exceeds the dataport size.
 * @retval OS_ERROR_IO                         If there is an input/output
 *                                             error.
 * @retval OS_ERROR_NETWORK_PROTO              If the function is called on the
 *                                             wrong socket type.
 * @retval OS_ERROR_INSUFFICIENT_SPACE         If there is not enough space.
 * @retval OS_ERROR_TRY_AGAIN                  If the resource is temporarily
 *                                             unavailable or the request would
 *                                             block and the caller should try
 *                                             again.
 * @retval OS_ERROR_CONNECTION_CLOSED          If the connection is in a closed
 *                                             state.
 * @retval OS_ERROR_NETWORK_CONN_NONE          If the socket is not connected.
 * @retval OS_ERROR_NETWORK_CONN_SHUTDOWN      If the connection got shut down.
 * @retval OS_ERROR_NETWORK_ADDR_NOT_AVAILABLE If the address is not available.
 * @retval OS_ERROR_NETWORK_HOST_UNREACHABLE   If the host is not unreachable.
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
OS_Socket_write(
    const OS_Socket_Handle_t handle,
    const void* const        buf,
    const size_t             requestedLen,
    size_t* const            actualLen);

/**
 * Send data on a destination socket without checking if the destination is
 * connected or not and is therefore not connection-oriented.
 *
 * @retval OS_SUCCESS                          Operation was successful.
 * @retval OS_ERROR_ABORTED                    If the Network Stack has
 *                                             experienced a fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED            If the function was called before
 *                                             the Network Stack was fully
 *                                             initialized.
 * @retval OS_ERROR_INVALID_HANDLE             If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER          If the handle context is invalid
 *                                             or the requested length exceeds
 *                                             the dataport size.
 * @retval OS_ERROR_INSUFFICIENT_SPACE         If there is not enough space.
 * @retval OS_ERROR_NETWORK_PROTO              If the function is called on the
 *                                             wrong socket type.
 * @retval OS_ERROR_TRY_AGAIN                  If the resource is temporarily
 *                                             unavailable or the request would
 *                                             block and the caller should try
 *                                             again.
 * @retval OS_ERROR_CONNECTION_CLOSED          If no further communication is
 *                                             possible on the socket.
 * @retval OS_ERROR_NETWORK_ADDR_NOT_AVAILABLE If the address is not available.
 * @retval OS_ERROR_NETWORK_HOST_UNREACHABLE   If the host is not unreachable.
 * @retval other                               Each component implementing this
 *                                             might have additional error
 *                                             codes.
 *
 * @param[in]  handle       Handle to a previously created socket.
 * @param[in]  buf          Buffer containing data to be written.
 * @param[in]  requestedLen Length of the data that should be written on the
 *                          destination socket.
 * @param[out] actualLen    Actual length that was written on the socket.
 * @param[in]  dstAddr      Address of the destination socket to send data on.
 */
OS_Error_t
OS_Socket_sendto(
    const OS_Socket_Handle_t      handle,
    const void* const             buf,
    size_t                        requestedLen,
    size_t* const                 actualLen,
    const OS_Socket_Addr_t* const dstAddr);

/**
 * Bind a specified local IP-address and port to a socket.
 *
 * @retval OS_SUCCESS                  Operation was successful.
 * @retval OS_ERROR_ABORTED            If the Network Stack has experienced a
 *                                     fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED    If the function was called before the
 *                                     Network Stack was fully initialized.
 * @retval OS_ERROR_INVALID_HANDLE     If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER  If the handle context is invalid or the
                                       requested length exceeds the dataport
                                       size.
 * @retval OS_ERROR_IO                 If the specified address can not be
 *                                     found.
 * @retval OS_ERROR_INSUFFICIENT_SPACE If there is not enough space.
 * @retval OS_ERROR_CONNECTION_CLOSED  If the connection is in a closed state.
 * @retval other                       Each component implementing this might
 *                                     have additional error codes.
 *
 * @param[in] handle    Handle of the socket to bind.
 * @param[in] localAddr Local address to bind the socket to.
 */
OS_Error_t
OS_Socket_bind(
    const OS_Socket_Handle_t      handle,
    const OS_Socket_Addr_t* const localAddr);

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
OS_Socket_getStatus(
    const if_OS_Socket_t* const ctx);

/**
 * Get the pending events for the opened sockets.
 *
 * @retval OS_SUCCESS                 Operation was successful.
 * @retval OS_ERROR_ABORTED           If the Network Stack has experienced a
 *                                    fatal error or an internal error occurred.
 * @retval OS_ERROR_NOT_INITIALIZED   If the function was called before the
 *                                    Network Stack was fully initialized.
 * @retval OS_ERROR_INVALID_HANDLE    If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER If the handle context is invalid.
 * @retval OS_ERROR_BUFFER_TOO_SMALL  If the buffer is not even large enough for
 *                                    one event.
 * @retval other                      Each component implementing this might
 *                                    have additional error codes.
 *
 * @param[in]  ctx            Interface context that should be used.
 * @param[in]  buf            Buffer to store the event data.
 * @param[in]  bufSize        Size of the buffer to store the event data.
 * @param[out] numberOfEvents Will be overwritten with the number of events.
 */
OS_Error_t
OS_Socket_getPendingEvents(
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
 * @param[in] ctx Interface context that should be used.
 */
OS_Error_t
OS_Socket_wait(
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
 * @param[in] ctx Interface context that should be used.
 */
OS_Error_t
OS_Socket_poll(
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
 * @param[in] ctx      Interface context that should be used.
 * @param[in] callback Function pointer to the function that should be invoked.
 * @param[in] arg      Argument that the registered function should be invoked
 *                     with.
 */
OS_Error_t
OS_Socket_regCallback(
    const if_OS_Socket_t* const ctx,
    void (*callback)(void*),
    void* arg);

/**
 * Closes a network socket. No further socket communication is possible after
 * closure.
 *
 * @retval OS_SUCCESS                 Operation was successful.
 * @retval OS_ERROR_ABORTED           If the Network Stack has experienced a
 *                                    fatal error.
 * @retval OS_ERROR_NOT_INITIALIZED   If the function was called before the
 *                                    Network Stack was fully initialized.
 * @retval OS_ERROR_INVALID_HANDLE    If an invalid handle was passed.
 * @retval OS_ERROR_INVALID_PARAMETER If the handle context is invalid.
 * @retval other                      Each component implementing this might
 *                                    have additional error codes.
 *
 * @param[in] handle Handle of the socket that should be closed.
 */
OS_Error_t
OS_Socket_close(
    const OS_Socket_Handle_t handle);
