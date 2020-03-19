/*
 *  SEOS return codes
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

/**
 * @brief   SEOS error codes
 *
 * @ingroup seos_error
*/
typedef enum
{
    _SEOS_ERROR_MIN = -58,

    // File System specific error codes
    SEOS_ERROR_FS_NO_FREE_HANDLE,                /**< failed cause no free handle */
    SEOS_ERROR_FS_DELETE_HANDLE,                 /**< failed due to delete handle */
    SEOS_ERROR_FS_NO_DISK,                       /**< disk not exists */
    SEOS_ERROR_FS_INIT,                          /**< failed to init */
    SEOS_ERROR_FS_REGISTER,                      /**< failed to register low level functions */
    SEOS_ERROR_FS_CREATE_FS,                     /**< failed to create filesystem */
    SEOS_ERROR_FS_FORMAT_FS,                     /**< failed to format filesystem */
    SEOS_ERROR_FS_PARTITION_READ,                /**< partiton read throws an error */
    SEOS_ERROR_FS_OPEN,                          /**< failed to open */
    SEOS_ERROR_FS_CLOSE,                         /**< failed to close */
    SEOS_ERROR_FS_MOUNT,                         /**< failed to mount */
    SEOS_ERROR_FS_UNMOUNT,                       /**< failed to unmount */
    SEOS_ERROR_FS_FILE_NOT_FOUND,                /**< file is missing (if FS_O_CREATE has not been passed) */
    SEOS_ERROR_FS_OPERATION_DENIED,              /**< partition was opened in read-only mode */
    SEOS_ERROR_FS_INSUFFICIENT_STORAGE_CAPACITY, /**< less space or file to big */
    SEOS_ERROR_FS_STRUCTURE,                     /**< failed to get internal structure */
    SEOS_ERROR_FS_RESOLVE_HANDLE,                /**< failed to resolve handle */
    SEOS_ERROR_FS_DELETE_RESOLVE_HANDLE,         /**< failed to delete resolve handle */
    SEOS_ERROR_FS_LIB,                           /**< error from lib */
    SEOS_ERROR_FS_DATABUFFER_OVERLOW,            /**< databuffer is too small */
    SEOS_ERROR_FS_INVALID_PARTITION_MODE,        /**< partition access mode is invalid */
    SEOS_ERROR_FS_PARTITION_NOT_READY,           /**< partition not ready */
    SEOS_ERROR_FS_INVALID_FILESYSTEM,            /**< no valid filesystem */

    // Partition Manager specific error codes
    SEOS_ERROR_PM_PARTITION_ID,                  /**< wrong partition id or partition id don't exists */
    SEOS_ERROR_PM_OPEN,                          /**< fail to open */
    SEOS_ERROR_PM_READ,                          /**< fail to read */
    SEOS_ERROR_PM_WRITE,                         /**< fail to write */
    SEOS_ERROR_PM_CLOSE,                         /**< fail to close */
    SEOS_ERROR_PM_OFFSET,                        /**< fail to get offset */
    SEOS_ERROR_PM_GET_STRUCT,                    /**< fail to get struct */
    SEOS_ERROR_PM_POINTER_START_POS,             /**< wrong pointer to start position */
    SEOS_ERROR_PM_POINTER_END_POS,               /**< wrong pointer to end position */
    SEOS_ERROR_PM_POINTER_POS,                   /**< pointer position of start and end pointer do not match the size of the partition */
    SEOS_ERROR_PM_INTERNAL_OBJECT,               /**< invalid object internally */
    SEOS_ERROR_PM_NO_PARTITIONS,                 /**< no partition exists */
    SEOS_ERROR_PM_DISK_SIZE,                     /**< real disk size is smaller than configured */
    SEOS_ERROR_PM_BLOCK_SIZE,                    /**< block size is undefined */
    SEOS_ERROR_PM_REGISTER_INTERNAL_OBJECT,      /**< fail to register internal object */
    SEOS_ERROR_PM_DATABUFFER_OVERLOW,            /**< databuffer is too small */

    // Configuration Server specific error codes
    SEOS_ERROR_CONFIG_DOMAIN_NOT_FOUND,          /**< configuration domain not found */
    SEOS_ERROR_CONFIG_PARAMETER_NOT_FOUND,       /**< configuration parameter not found */
    SEOS_ERROR_CONFIG_TYPE_MISMATCH,             /**< configuration parameter type mismatch */

    // General error codes
    SEOS_ERROR_CONNECTION_CLOSED,                /**< connection closed */
    SEOS_ERROR_OVERFLOW_DETECTED,                /**< overflow detected */
    SEOS_ERROR_INSUFFICIENT_SPACE,               /**< insufficient space */
    SEOS_ERROR_BUFFER_TOO_SMALL,                 /**< buffer too small */
    SEOS_ERROR_ABORTED,                          /**< operation abordet */
    SEOS_ERROR_OPERATION_DENIED,                 /**< operation denied */
    SEOS_ERROR_ACCESS_DENIED,                    /**< access denied */
    SEOS_ERROR_NOT_FOUND,                        /**< not found */
    SEOS_ERROR_INVALID_HANDLE,                   /**< invalid handle */
    SEOS_ERROR_INVALID_NAME,                     /**< invalid name */
    SEOS_ERROR_INVALID_PARAMETER,                /**< invalid parameter */
    SEOS_ERROR_INVALID_STATE,                    /**< invalid state */
    SEOS_ERROR_NOT_SUPPORTED,                    /**< not supported */
    SEOS_ERROR_NOT_IMPLEMENTED,                  /**< not implemented */
    SEOS_ERROR_GENERIC,                          /**< general error, not further details available */
    SEOS_SUCCESS                                 /**< operation successful */
}
seos_err_t;

static_assert(SEOS_SUCCESS == 0, "SEOS_SUCCESS_MUST BE 0");


#ifdef __cplusplus
}
#endif
