/*
 *  seos_nw_api.h
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
*/


/**
 * @defgroup seos_nw_api SEOS Nwstack API
 * @file seos_nw_api.h
 *
 * @brief Network stack API.These  API mostly interacts with core SEOS NWStack. \n
 *        This layer mostly supports all API's for the app for socket related operations.
 *
 *
 */
#pragma once


/**
 * @brief   seos_nw_server_struct contains elements that must be filled by APP to configure it
            as server.

 * @ingroup seos_nw_api

*/
typedef struct _Seos_NW_Server_t
{
    int domain;                /**< domain is as of now AF_INET */
    int type;                  /**< type is as of now SOCK_STREAM */
    uint16_t listen_port;      /**< port to listen to e.g. 5555 */
    int backlog;               /**< number of connections accepted (currently fixed =1)*/
} seos_nw_server_struct;


/**
 * @brief   seos_nw_client_struct contains elements that must be filled by APP to configure it
            as client.

 * @ingroup seos_nw_api

*/


typedef struct _Seos_NW_Client_t
{
    int domain;                     /**< domain is as of now AF_INET */
    int type;                       /**< type is as of now SOCK_STREAM */
    const char* name;               /**< name must be IP addr to connect to */
    uint16_t port;                  /**< port is for e.g. HTTP port 80 */
} seos_nw_client_struct;


#if !defined(CUSTOM_SEOS_NETWORK_STACK_API_TYPES)

    // Protocol families.
    #define PF_INET     2   // IP protocol family.
    #define PF_INET6    10  // IP version 6.


    #define AF_INET     PF_INET
    #define AF_INET6    PF_INET6

    // Socket definition
    #define SOCK_STREAM    2
    #define SOCK_DGRAM     1


    typedef void*  Seos_nw_context;
    typedef int    seos_socket_handle_t;
    typedef int    seos_nw_server_handle_t;

#endif



/**
 * @details %Seos_client_socket_create,  Create a socket and connect to it

 * @param Seos_nw_context:  Passed by App which is the run time context received by APP. As of now NULL

 * @param seos_nw_client_struct: Must be filled by app. Please see the struct seos_nw_client_struct for details

          domain(required) -> Domain is of type AF_INET or AF_INET6. As of now we work with AF_INET only
          type  (required) -> Type is of type SOCK_STREAM or SOCK_DGRAM. As of now we work only with SOCK_STREAM
          name (required)  -> Connect to an IP addresss, such as "192.168.82.45"
          port (required)  -> Port  number to connect to . e.g. 80

 * @param seos_socket_handle_t: Handle will be filled by after the call to Seos_client_socket_create().

 * @return  SEOS_SUCCESS or SEOS_ERROR
 */

seos_err_t
Seos_client_socket_create(Seos_nw_context ctx,
                          seos_nw_client_struct* pClientStruct,
                          seos_socket_handle_t* phandle);


/**
 * @details %Seos_server_socket_create, Create a server socket, binds to a port and listen for incoming connections

 * @param Seos_nw_context: Passed by App which is the run time context received by APP.

 * @param seos_nw_server_struct: Must be filled by app. Please see the struct seos_nw_server_struct for details

          domain(required)  -> Domain is of type AF_INET or AF_INET6. As of now we work with AF_INET only
          type  (required)  -> Type is of type SOCK_STREAM or SOCK_DGRAM. As of now we work only with SOCK_STREAM
          backlog(required) -> Indicates how many incoming connections is possible (for now it is only 1 connection)
                               Set to 1
          port  (required)  -> Port to which you want to bind. e.g. 5555

 * @param seos_nw_server_handle_t: Handle will be filled by after the call to Seos_server_socket_create.
 *
 * @return  SEOS_SUCCESS or SEOS_ERROR
 */

seos_err_t
Seos_server_socket_create(Seos_nw_context ctx,
                          seos_nw_server_struct* pServerStruct,
                          seos_nw_server_handle_t* pSrvHandle);



/**
 * @details %Seos_server_socket_close, Must be called when the server app wants to close a comnection.
          No further socket communication is possible after closure.

 * @param seos_nw_server_handle_t: Handle that was used to create a server socket

 * @return  SEOS_SUCCESS or SEOS_ERROR
 */


seos_err_t
Seos_server_socket_close(seos_nw_server_handle_t srvHandle);


/**
 * @details %Seos_socket_close, Closes a network socket. Once the close is done no further socket communication is possible.
 *
 * @param seos_socket_handle_t: Handle used to open/create socket.
 *
 * @return SEOS_SUCCESS or SEOS_ERROR
 *
 */

seos_err_t
Seos_socket_close(seos_socket_handle_t handle);


/**
 * @details %Seos_socket_write, Write to a network socket. Write data to a socket after connecting.
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
Seos_socket_write(seos_socket_handle_t handle,
                  const void* buf,
                  size_t* len);



/**
 * @details %Seos_socket_accept, Accept incoming connections. This is useful when the Network stack is working as Server.
 *
 * @param seos_nw_server_handle_t: Handle used to create server socket.

 * @param seos_socket_handle_t*. At the end accept() will fill the handle of an incoming
                                 connection.This handle is used for further read/write.

 * @return SEOS_SUCCESS or SEOS_ERROR
 */

seos_err_t
Seos_socket_accept(seos_nw_server_handle_t srvHandle,
                   seos_socket_handle_t *phSocket);

/**
 * @details %Seos_scoket_read, Read data from connected socket.

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
Seos_socket_read(seos_socket_handle_t handle,
                 void* buf,
                 size_t* len);
