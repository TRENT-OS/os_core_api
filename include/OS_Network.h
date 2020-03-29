/*
 *  OS_Network.h
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "SeosError.h"
#include <stdint.h>

/**
 * @defgroup os_network_api
 * @file OS_Network.h
 *
 * @brief Network stack API.These  API mostly interacts with core SEOS NWStack. \n
 *        This layer mostly supports all API's for the app for socket related operations.
 */

/**
 * @brief   OS_NetworkServer_Socket_t contains elements that must be filled by APP to configure it
            as server.

 * @ingroup os_network_api

*/
typedef struct
{
    unsigned domain;      /**< domain is as of now AF_INET */
    unsigned type;        /**< type is as of now SOCK_STREAM */
    uint16_t listen_port; /**< port to listen to e.g. 5555 */
    unsigned backlog;     /**< number of connections accepted (currently fixed =1)*/
} OS_NetworkServer_Socket_t;

/**
 * @brief   OS_Network_Socket_t contains elements that must be filled by APP to configure it
            as client.

 * @ingroup os_network_api

*/

typedef struct
{
    unsigned domain;  /**< domain is as of now AF_INET */
    unsigned type;    /**< type is as of now SOCK_STREAM */
    const char *name; /**< name must be IP addr to connect to */
    uint16_t port;    /**< port is for e.g. HTTP port 80 */
} OS_Network_Socket_t;

#if !defined(CUSTOM_OS_NETWORK_STACK_API_TYPES)

// Protocol families.
#define OS_AF_INET 2   // IP protocol family.
#define OS_AF_INET6 10 // IP version 6.

// Socket definition
#define OS_SOCK_STREAM 2
#define OS_SOCK_DGRAM 1

typedef void *OS_Network_Context_t;
typedef unsigned OS_NetworkSocket_Handle_t;
typedef unsigned OS_NetworkServer_Handle_t;

#endif

/**
 * @details %OS_NetworkSocket_create,  Create a socket and connect to it

 * @param OS_Network_Context_t:  Passed by App which is the run time context received by APP. As of now NULL

 * @param OS_Network_Socket_t: Must be filled by app. Please see the struct OS_Network_Socket_t for details

          domain(required) -> Domain is of type AF_INET or AF_INET6. As of now we work with AF_INET only
          type  (required) -> Type is of type SOCK_STREAM or SOCK_DGRAM. As of now we work only with SOCK_STREAM
          name (required)  -> Connect to an IP addresss, such as "192.168.82.45"
          port (required)  -> Port  number to connect to . e.g. 80

 * @param OS_NetworkSocket_Handle_t: Handle will be filled by after the call to OS_NetworkSocket_create().

 * @return  SEOS_SUCCESS or SEOS_ERROR
 */

seos_err_t
OS_NetworkSocket_create(OS_Network_Context_t ctx,
                        OS_Network_Socket_t *pClientStruct,
                        OS_NetworkSocket_Handle_t *phandle);

/**
 * @details %OS_NetworkServerSocket_create, Create a server socket, binds to a port and listen for incoming connections

 * @param OS_Network_Context_t: Passed by App which is the run time context received by APP.

 * @param OS_NetworkServer_Socket_t: Must be filled by app. Please see the struct OS_NetworkServer_Socket_t for details

          domain(required)  -> Domain is of type AF_INET or AF_INET6. As of now we work with AF_INET only
          type  (required)  -> Type is of type SOCK_STREAM or SOCK_DGRAM. As of now we work only with SOCK_STREAM
          backlog(required) -> Indicates how many incoming connections is possible (for now it is only 1 connection)
                               Set to 1
          port  (required)  -> Port to which you want to bind. e.g. 5555

 * @param OS_NetworkServer_Handle_t: Handle will be filled by after the call to OS_NetworkServerSocket_create.
 *
 * @return  SEOS_SUCCESS or SEOS_ERROR
 */

seos_err_t
OS_NetworkServerSocket_create(OS_Network_Context_t ctx,
                              OS_NetworkServer_Socket_t *pServerStruct,
                              OS_NetworkServer_Handle_t *pSrvHandle);
/**
 * @details %OS_NetworkServerSocket_close, Must be called when the server app wants to close a comnection.
          No further socket communication is possible after closure.

 * @param OS_NetworkServer_Handle_t: Handle that was used to create a server socket

 * @return  SEOS_SUCCESS or SEOS_ERROR
 */

seos_err_t
OS_NetworkServerSocket_close(OS_NetworkServer_Handle_t srvHandle);

/**
 * @details %OS_NetworkSocket_close, Closes a network socket. Once the close is done no further socket communication is possible.
 *
 * @param OS_NetworkSocket_Handle_t: Handle used to open/create socket.
 *
 * @return SEOS_SUCCESS or SEOS_ERROR
 *
 */

seos_err_t
OS_NetworkSocket_close(OS_NetworkSocket_Handle_t handle);

/**
 * @details %OS_NetworkSocket_write, Write to a network socket. Write data to a socket after connecting.
            Copy the data to be writtenin the CamkES App dataport.
 *
 * @param handle: Used to create/open socket
 *
 * @param buf: Buffer containing data to be written

 * @param len: Length of data to write
 *
 * @return Actual Number of Bytes written  or SEOS_ERROR
 *
 */
seos_err_t
OS_NetworkSocket_write(OS_NetworkSocket_Handle_t handle,
                       const void *buf,
                       size_t *len);

/**
 * @details %OS_NetworkServerSocket_accept, Accept incoming connections. This is useful when the Network stack is working as Server.
 *
 * @param OS_NetworkServer_Handle_t: Handle used to create server socket.

 * @param OS_NetworkSocket_Handle_t*. At the end accept() will fill the handle of an incoming
                                 connection.This handle is used for further read/write.

 * @return SEOS_SUCCESS or SEOS_ERROR
 */

seos_err_t
OS_NetworkServerSocket_accept(OS_NetworkServer_Handle_t srvHandle,
                              OS_NetworkSocket_Handle_t *phSocket);

/**
 * @details %OS_NetworkSocket_read, Read data from connected socket.

 * @param handle: Used to create/open socket

 * @param buf: Buffer to read data into
 *
 * @param len: Indicates how much data to read. After read it indicates how much was actually read
 *
 * @return seos_err_t, following are the values
                       SEOS_ERROR_CONNECTION_CLOSED and length = 0, end of data and connection close
                       SEOS_ERROR_GENERIC  due to error in read operation
                       SEOS_SUCCESS and length = 0, nothing to read but there is a connection
                       SEOS_SUCCESS and length > 0, data to read and there is connection

 */

seos_err_t
OS_NetworkSocket_read(OS_NetworkSocket_Handle_t handle,
                      void *buf,
                      size_t *len);
