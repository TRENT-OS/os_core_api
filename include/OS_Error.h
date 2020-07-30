/*
 *  OS return codes
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

/**
 * @brief   OS error codes
 *
 * @ingroup OS_error
*/
typedef enum
{
    //--------------------------------------------------------------------------
    // space reserved for custom error codes range
    //--------------------------------------------------------------------------
    __START_OS_ERROR_CODES_CUSTOM = -1000000,
    // OS_ERROR_CUSTOM_FOO = -999999
    // ...
    // OS_ERROR_CUSTOM_BAR = -100001
    __END_OS_ERROR_CODES_CUSTOM = -100000,

    //--------------------------------------------------------------------------
    // File System specific error codes
    //--------------------------------------------------------------------------
    /// @cond OS_ERROR_HELPER_DEFINES
    __START_OS_ERROR_CODES_FS = -1124,
    /// @endcond
    //----------------------------------------
    OS_ERROR_FS_NO_FREE_HANDLE,                /**< failed cause no free handle */
    OS_ERROR_FS_DELETE_HANDLE,                 /**< failed due to delete handle */
    OS_ERROR_FS_NO_DISK,                       /**< disk doesn't exist */
    OS_ERROR_FS_INIT,                          /**< failed to init */
    OS_ERROR_FS_REGISTER,                      /**< failed to register low level functions */
    OS_ERROR_FS_CREATE_FS,                     /**< failed to create filesystem */
    OS_ERROR_FS_FORMAT_FS,                     /**< failed to format filesystem */
    OS_ERROR_FS_PARTITION_READ,                /**< partiton read throws an error */
    OS_ERROR_FS_OPEN,                          /**< failed to open */
    OS_ERROR_FS_CLOSE,                         /**< failed to close */
    OS_ERROR_FS_MOUNT,                         /**< failed to mount */
    OS_ERROR_FS_UNMOUNT,                       /**< failed to unmount */
    OS_ERROR_FS_FILE_NOT_FOUND,                /**< file is missing (if FS_O_CREATE has not been passed) */
    OS_ERROR_FS_OPERATION_DENIED,              /**< partition was opened in read-only mode */
    OS_ERROR_FS_INSUFFICIENT_STORAGE_CAPACITY, /**< less space or file to big */
    OS_ERROR_FS_STRUCTURE,                     /**< failed to get internal structure */
    OS_ERROR_FS_RESOLVE_HANDLE,                /**< failed to resolve handle */
    OS_ERROR_FS_DELETE_RESOLVE_HANDLE,         /**< failed to delete resolve handle */
    OS_ERROR_FS_LIB,                           /**< error from lib */
    OS_ERROR_FS_DATABUFFER_OVERLOW,            /**< databuffer is too small */
    OS_ERROR_FS_INVALID_PARTITION_MODE,        /**< partition access mode is invalid */
    OS_ERROR_FS_PARTITION_NOT_READY,           /**< partition not ready */
    OS_ERROR_FS_INVALID_FILESYSTEM,            /**< no valid filesystem */
    //----------------------------------------
    /// @cond OS_ERROR_HELPER_DEFINES
    __END_OS_ERROR_CODES_FS,                   /**< -1100 */
    /// @endcond

    //--------------------------------------------------------------------------
    // Configuration Server specific error codes
    //--------------------------------------------------------------------------
    /// @cond OS_ERROR_HELPER_DEFINES
    __START_OS_ERROR_CODES_CONFIG = -1004,
    /// @endcond
    //----------------------------------------
    OS_ERROR_CONFIG_DOMAIN_NOT_FOUND,          /**< configuration domain not found */
    OS_ERROR_CONFIG_PARAMETER_NOT_FOUND,       /**< configuration parameter not found */
    OS_ERROR_CONFIG_TYPE_MISMATCH,             /**< configuration parameter type mismatch */
    //----------------------------------------
    /// @cond OS_ERROR_HELPER_DEFINES
    __END_OS_ERROR_CODES_CONFIG,              /**< -1000 */
    /// @endcond

    //--------------------------------------------------------------------------
    // General error codes
    //--------------------------------------------------------------------------
    /// @cond OS_ERROR_HELPER_DEFINES
    __START_OS_ERROR_CODES_GENERIC = -17,
    /// @endcond
    //----------------------------------------
    OS_ERROR_OUT_OF_BOUNDS,                    /**< operation violated boundaries */
    OS_ERROR_CONNECTION_CLOSED,                /**< connection closed */
    OS_ERROR_OVERFLOW_DETECTED,                /**< overflow detected */
    OS_ERROR_INSUFFICIENT_SPACE,               /**< insufficient space */
    OS_ERROR_BUFFER_TOO_SMALL,                 /**< buffer too small */
    OS_ERROR_ABORTED,                          /**< operation aborted */
    OS_ERROR_OPERATION_DENIED,                 /**< operation denied */
    OS_ERROR_ACCESS_DENIED,                    /**< access denied */
    OS_ERROR_NOT_FOUND,                        /**< not found */
    OS_ERROR_INVALID_HANDLE,                   /**< invalid handle */
    OS_ERROR_INVALID_NAME,                     /**< invalid name */
    OS_ERROR_INVALID_PARAMETER,                /**< invalid parameter */
    OS_ERROR_INVALID_STATE,                    /**< invalid state */
    OS_ERROR_NOT_SUPPORTED,                    /**< not supported */
    OS_ERROR_NOT_IMPLEMENTED,                  /**< not implemented */
    OS_ERROR_GENERIC,                          /**< general error, not further details available */

    //----------------------------------------
    OS_SUCCESS                                 /**< operation successful */
}
OS_Error_t;


/// @cond OS_ERROR_HELPER_DEFINES
#define CHECK_OS_ERROR_VALUE(e, v) \
        static_assert(e == v, #e " must be " #v)

#define CHECK_OS_ERROR_OVERLAP(v1, v2) \
        static_assert(v1 > v2, #v1 " must be greater " #v2)


CHECK_OS_ERROR_VALUE(OS_SUCCESS, 0);

CHECK_OS_ERROR_OVERLAP(
    __START_OS_ERROR_CODES_GENERIC,
    __END_OS_ERROR_CODES_CONFIG);

CHECK_OS_ERROR_VALUE(__END_OS_ERROR_CODES_CONFIG, -1000);

CHECK_OS_ERROR_OVERLAP(
    __START_OS_ERROR_CODES_CONFIG,
    __END_OS_ERROR_CODES_FS);

CHECK_OS_ERROR_VALUE(__END_OS_ERROR_CODES_FS, -1100);

CHECK_OS_ERROR_OVERLAP(
    __START_OS_ERROR_CODES_FS,
    __END_OS_ERROR_CODES_CUSTOM);

/// @endcond


#ifdef __cplusplus
}
#endif
