
/*
 * OS return codes
 *
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   OS error codes
 *
 * @ingroup OS_error
*/
typedef enum
{

    //--------------------------------------------------------------------------
    // Success codes
    //--------------------------------------------------------------------------
    OS_SUCCESS = 0, ///< Operation successful.


    //--------------------------------------------------------------------------
    // General error codes
    //--------------------------------------------------------------------------
    OS_ERROR_IN_PROGRESS = -1, ///< operation in progress
    OS_ERROR_TIMEOUT = -2, ///< operation timeout
    OS_ERROR_IO = -3, ///< generic IO error
    OS_ERROR_EXISTS = -4, ///< alredy exists
    OS_ERROR_BUFFER_FULL = -5, ///< buffer is full
    OS_ERROR_BUFFER_EMPTY = -6, ///< buffer is empty
    OS_ERROR_NO_DATA = -7, ///< no data available to fullfill the request
    OS_ERROR_NOT_INITIALIZED = -8, ///< not initialized
    OS_ERROR_TRY_AGAIN = -9, ///< caller should retry operation
    OS_ERROR_WOULD_BLOCK = -10, ///< operation would block
    OS_ERROR_OUT_OF_BOUNDS = -11, ///< operation violated boundaries
    OS_ERROR_CONNECTION_CLOSED = -12, ///< connection closed
    OS_ERROR_OVERFLOW_DETECTED = -13, ///< overflow detected
    OS_ERROR_INSUFFICIENT_SPACE = -14, ///< insufficient space
    OS_ERROR_BUFFER_TOO_SMALL = -15, ///< buffer too small
    OS_ERROR_ABORTED = -16, ///< operation aborted
    OS_ERROR_OPERATION_DENIED = -17, ///< operation denied
    OS_ERROR_ACCESS_DENIED = -18, ///< access denied
    OS_ERROR_NOT_FOUND = -19, ///< not found
    OS_ERROR_INVALID_HANDLE = -20, ///< invalid handle
    OS_ERROR_INVALID_NAME = -21, ///< invalid name
    OS_ERROR_INVALID_PARAMETER = -22, ///< invalid parameter
    OS_ERROR_INVALID_STATE = -23, ///< invalid state
    OS_ERROR_NOT_SUPPORTED = -24, ///< not supported
    OS_ERROR_NOT_IMPLEMENTED = -25, ///< not implemented
    OS_ERROR_GENERIC = -26, ///< general error, not further details available


    //--------------------------------------------------------------------------
    // Configuration Server specific
    //--------------------------------------------------------------------------
    OS_ERROR_CONFIG_DOMAIN_NOT_FOUND = -1000, ///< configuration domain not found
    OS_ERROR_CONFIG_PARAMETER_NOT_FOUND = -1001, ///< configuration parameter not found
    OS_ERROR_CONFIG_TYPE_MISMATCH = -1002, ///< configuration parameter type mismatc


    //--------------------------------------------------------------------------
    // File System specific error codes
    //--------------------------------------------------------------------------
    OS_ERROR_FS_NO_FREE_HANDLE = -1100, ///< failed cause no free handle
    OS_ERROR_FS_DELETE_HANDLE = -1101, ///< failed due to delete handle
    OS_ERROR_FS_NO_DISK = -1102, ///< disk doesn't exist
    OS_ERROR_FS_INIT = -1103, ///< failed to init
    OS_ERROR_FS_REGISTER = -1104, ///< failed to register low level functions
    OS_ERROR_FS_CREATE_FS = -1105, ///< failed to create filesystem
    OS_ERROR_FS_FORMAT_FS = -1106, ///< failed to format filesystem
    OS_ERROR_FS_PARTITION_READ = -1107, ///< partiton read throws an error
    OS_ERROR_FS_OPEN = -1108, ///< failed to open
    OS_ERROR_FS_CLOSE = -1109, ///< failed to close
    OS_ERROR_FS_MOUNT = -1110, ///< failed to mount
    OS_ERROR_FS_UNMOUNT = -1111, ///< failed to unmount
    OS_ERROR_FS_FILE_NOT_FOUND = -1112, ///< file is missing (if FS_O_CREATE has not been passed)
    OS_ERROR_FS_OPERATION_DENIED = -1113, ///< partition was opened in read-only mode
    OS_ERROR_FS_INSUFFICIENT_STORAGE_CAPACITY = -1114, ///< less space or file to big
    OS_ERROR_FS_STRUCTURE = -1115, ///< failed to get internal structure
    OS_ERROR_FS_RESOLVE_HANDLE = -1116, ///< failed to resolve handle
    OS_ERROR_FS_DELETE_RESOLVE_HANDLE = -1117, ///< failed to delete resolve handle
    OS_ERROR_FS_LIB = -1118, ///< error from lib
    OS_ERROR_FS_DATABUFFER_OVERLOW = -1119, ///< databuffer is too small
    OS_ERROR_FS_INVALID_PARTITION_MODE = -1120, ///< partition access mode is invalid
    OS_ERROR_FS_PARTITION_NOT_READY = -1121, ///< partition not ready
    OS_ERROR_FS_INVALID_FILESYSTEM = -1122, ///< no valid filesystem


    //--------------------------------------------------------------------------
    // Device specific error codes
    //--------------------------------------------------------------------------
    OS_ERROR_DEVICE_INVALID = -1200, ///< The device is present, but it can't be used.
    OS_ERROR_DEVICE_NOT_PRESENT = -1201, ///< The device is not present.
    OS_ERROR_DEVICE_BUSY = -1202, ///< The device is present, but temporarily not accessible.


    //--------------------------------------------------------------------------
    // Network specific error codes
    //--------------------------------------------------------------------------
    OS_ERROR_NETWORK_NO_SUPPORT = -1300, ///< no network support
    OS_ERROR_NETWORK_OP_NO_SUPPORT = -1301, ///< operation not supported
    OS_ERROR_NETWORK_DOWN = -1302, ///< network stack is down
    OS_ERROR_NETWORK_UNREACHABLE = -1303, ///< network is unreachable
    OS_ERROR_NETWORK_NO_ROUTE = -1304, ///< no route to host or network
    OS_ERROR_NETWORK_PROTO = -1305, ///< protocol error
    OS_ERROR_NETWORK_PROTO_NO_SUPPORT = -1306, ///< protocol not supported
    OS_ERROR_NETWORK_PROTO_OPT_NO_SUPPORT = -1307, ///< protocol option not supported
    OS_ERROR_NETWORK_ADDR_IN_USE = -1308, ///< address already in use
    OS_ERROR_NETWORK_ADDR_NOT_AVAILABLE = -1309, ///< address not available
    OS_ERROR_NETWORK_CONN_RESET = -1310, ///< connection reset
    OS_ERROR_NETWORK_CONN_ALREADY_BOUND = -1311, ///< already connected
    OS_ERROR_NETWORK_CONN_NONE = -1312, ///< no connection
    OS_ERROR_NETWORK_CONN_SHUTDOWN = -1313, ///< connection shutdown
    OS_ERROR_NETWORK_CONN_REFUSED = -1314, ///< connection refused
    OS_ERROR_NETWORK_HOST_DOWN = -1315, ///< host is down
    OS_ERROR_NETWORK_HOST_UNREACHABLE = -1316, ///< host is unreachable

}
OS_Error_t;

#ifdef __cplusplus
}
#endif
