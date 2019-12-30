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
#include "OS_Types.h"
#include "SeosError.h"
#include "seos_types.h"
#include <stdint.h>

/**
 * @defgroup os_network_api
 * @file OS_Network.h
 *
 * @brief Network stack API.These  API mostly interacts with core SEOS NWStack. \n
 *        This layer mostly supports all API's for the app for socket related operations.
 */




// bit 0-3 define the IP protocol version. We stick to RFC791 here, which
// defines the 'version' field or the IP header to be 4 bits wide. IANA
// maintains a list at
// http://www.iana.org/assignments/version-numbers/version-numbers.xhtml
//
//   0  reserved (RFC4928)
//   1  reserved (RFC4928)
//   2  unassigned
//   3  unassigned
//   4  IPv4 (Internet Protocol Version 4, RFC791)
//   5  ST (ST Datagram Mode, RFC1190
//   6  IPv6 (Internet Protocol version 6, RFC1752)
//   7  TP/IX (TP/IX: The Next Internet, RFC1475)
//   8  PiP (The P Internet Protocol, RFC1621)
//   9  TUBA, RFC1347)
//  10  unassigned
//  11  unassigned
//  12  unassigned
//  13  unassigned
//  14  unassigned
//  15  reserved
//
#define OS_SOCKET_IPV4    (4u << 0)
#define OS_SOCKET_IPV6    (6u << 0)

#define OS_GET_SOCKET_PROTOCOL_FROM_MODE(mode)    ( (mode) & 0x0F )

// bit 4 defines if stream (TCP) or stream (UDP) is used
#define OS_SOCKET_STREAM     (0u << 4)
#define OS_SOCKET_DGRAM      (1u << 4)

#define OS_GET_SOCKET_TYPE_FROM_MODE(mode)        ( (mode) & 0x10 )



/**
 * @brief   seos_nw_server_struct contains elements that must be filled by APP to configure it
 *          as server.
 *
 * @ingroup seos_nw_api
*/
typedef struct
{
    unsigned int  mode;
    unsigned int  backlog;
    uint16_t      port;
} OS_server_socket_params_t;


/**
 * @brief   seos_nw_client_struct contains elements that must be filled by APP to configure it
 *          as client.
 *
 * @ingroup seos_nw_api
*/
typedef struct
{
    unsigned int  mode;
    // RFC1035 limits a fully qualified domain name to 255 characters, where
    // each node (delimited by a dot) is limited to 63 characters
    char          name[256];
    uint16_t      port;
} OS_client_socket_params_t;


#define DECLARE_TYPED_SOCKET_SOCKET_API(_type_, _handle_type_t_) \
    typedef struct \
    { \
        seos_err_t (*server_socket_create)( \
            const OS_server_socket_params_t*  params, \
            _handle_type_t_*                  pServerSocketHandle); \
        \
        seos_err_t (*client_socket_create)(  \
            const OS_client_socket_params_t*  params,  \
            _handle_type_t_*                  pClientSocketHandle);  \
        \
        seos_err_t (*close)(  \
            _handle_type_t_  handle);  \
        \
        seos_err_t (*accept)( \
            _handle_type_t_    serverSocketHandle, \
            _handle_type_t_ *  pClientSocketHandle); \
         \
        seos_err_t (*read)( \
            _handle_type_t_  handle, \
            size_t *         pLen); \
         \
        seos_err_t (*write)( \
            _handle_type_t_  handle, \
            size_t *         pLen); \
        \
    } _type_;


#if !defined(CUSTOM_SEOS_NETWORK_STACK_API_TYPES)

typedef unsigned int  OS_socket_handle_t;
typedef unsigned int  OS_server_socket_handle_t;

DECLARE_TYPED_SOCKET_SOCKET_API(OS_socket_api_ctx_t, unsigned int);

#endif


#define SETUP_SOCKET_SOCKET_API_INSTANCE(_prefix_) \
    { \
        .server_socket_create = _prefix_##_server_socket_create, \
        .client_socket_create = _prefix_##_client_socket_create, \
        .close                = _prefix_##_socket_close, \
        .accept               = _prefix_##_socket_accept, \
        .read                 = _prefix_##_socket_read, \
        .write                = _prefix_##_socket_write, \
    }



typedef struct
{
    event_wait_func_t      wait_init_done;
    OS_socket_api_ctx_t    api;
    seos_shared_buffer_t   port;
} OS_network_client_lib_config_t;


void
OS_network_client_lib_init(
    const OS_network_client_lib_config_t*   cfg);


//------------------------------------------------------------------------------
// SEOS Socket API
//------------------------------------------------------------------------------

OS_Error_t
OS_client_socket_create(
    const OS_network_client_lib_config_t*  ctx,
    const OS_client_socket_params_t*       params,
    OS_socket_handle_t*                    phandle);


OS_Error_t
OS_server_socket_create(
    const OS_network_client_lib_config_t*  ctx,
    const OS_server_socket_params_t*       params,
    OS_server_socket_handle_t*             pHandle);


OS_Error_t
OS_socket_accept(
    OS_server_socket_handle_t serverSocketHandle,
    OS_socket_handle_t*       pHandle);


OS_Error_t
OS_server_socket_close(
    OS_server_socket_handle_t  handle);


OS_Error_t
OS_socket_close(
    OS_socket_handle_t  handle);


OS_Error_t
OS_socket_write(
    OS_socket_handle_t  handle,
    const void*         buf,
    size_t*             len);


OS_Error_t
OS_socket_read(
    OS_socket_handle_t  handle,
    void*               buf,
    size_t*             len);
