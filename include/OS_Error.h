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
    _OS_ERROR_MIN = -59,

    // File System specific error codes
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

    // Partition Manager specific error codes
    OS_ERROR_PM_PARTITION_ID,                  /**< wrong partition id or partition id doesn't exist */
    OS_ERROR_PM_INIT,                          /**< fail to init */
    OS_ERROR_PM_OPEN,                          /**< fail to open */
    OS_ERROR_PM_READ,                          /**< fail to read */
    OS_ERROR_PM_WRITE,                         /**< fail to write */
    OS_ERROR_PM_CLOSE,                         /**< fail to close */
    OS_ERROR_PM_OFFSET,                        /**< fail to get offset */
    OS_ERROR_PM_GET_STRUCT,                    /**< fail to get struct */
    OS_ERROR_PM_POINTER_START_POS,             /**< wrong pointer to start position */
    OS_ERROR_PM_POINTER_END_POS,               /**< wrong pointer to end position */
    OS_ERROR_PM_POINTER_POS,                   /**< pointer position of start and end pointer do not match the size of the partition */
    OS_ERROR_PM_INTERNAL_OBJECT,               /**< invalid object internally */
    OS_ERROR_PM_NO_PARTITIONS,                 /**< no partition exists */
    OS_ERROR_PM_DISK_SIZE,                     /**< real disk size is smaller than configured */
    OS_ERROR_PM_BLOCK_SIZE,                    /**< block size is undefined */
    OS_ERROR_PM_REGISTER_INTERNAL_OBJECT,      /**< fail to register internal object */
    OS_ERROR_PM_DATABUFFER_OVERLOW,            /**< databuffer is too small */

    // Configuration Server specific error codes
    OS_ERROR_CONFIG_DOMAIN_NOT_FOUND,          /**< configuration domain not found */
    OS_ERROR_CONFIG_PARAMETER_NOT_FOUND,       /**< configuration parameter not found */
    OS_ERROR_CONFIG_TYPE_MISMATCH,             /**< configuration parameter type mismatch */

    // General error codes
    OS_ERROR_CONNECTION_CLOSED,                /**< connection closed */
    OS_ERROR_OVERFLOW_DETECTED,                /**< overflow detected */
    OS_ERROR_INSUFFICIENT_SPACE,               /**< insufficient space */
    OS_ERROR_BUFFER_TOO_SMALL,                 /**< buffer too small */
    OS_ERROR_ABORTED,                          /**< operation abordet */
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
    OS_SUCCESS                                 /**< operation successful */
}
OS_Error_t;

static_assert(OS_SUCCESS == 0, "OS_SUCCESS_MUST BE 0");


#ifdef __cplusplus
}
#endif
