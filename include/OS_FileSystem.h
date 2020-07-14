/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @defgroup OS_FileSystem OS FileSystem API
 * @{
 *
 * @file OS_FileSystem.h
 *
 * @brief OS FileSystem API library
 *
 */

#pragma once

#include "OS_Error.h"
#include "OS_Dataport.h"

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    OS_FileSystem_Type_NONE = 0,

    /**
     * Use FATFS (which type of FAT is used effectively depends on the
     * clustersize)
     */
    OS_FileSystem_Type_FATFS,

    /**
     * Use SPIFFS (optimized for FLASH storage)
     */
    OS_FileSystem_Type_SPIFFS,

    /**
     * Use LITTLEFS (optimized for FLASH storage)
     */
    OS_FileSystem_Type_LITTLEFS,
} OS_FileSystem_Type_t;

typedef enum
{
    OS_FileSystem_OpenMode_NONE   = 0,

    /**
     * Open READ-ONLY
     */
    OS_FileSystem_OpenMode_RDONLY,

    /**
     * Open WRITE-ONLY
     */
    OS_FileSystem_OpenMode_WRONLY,

    /**
     * Open READ-WRITE
     */
    OS_FileSystem_OpenMode_RDWR,
} OS_FileSystem_OpenMode_t;

typedef enum
{
    OS_FileSystem_OpenFlags_NONE        = 0,

    /**
     * Create file if it does not exist
     */
    OS_FileSystem_OpenFlags_CREATE      = (1u << 0),

    /**
     * Open file exclusively
     */
    OS_FileSystem_OpenFlags_EXCLUSIVE   = (1u << 1),

    /**
     * Opens a file and truncates it to zero length; the file must have write
     * permission
     */
    OS_FileSystem_OpenFlags_TRUNCATE    = (1u << 2),
} OS_FileSystem_OpenFlags_t;

typedef unsigned int OS_FileSystemFile_Handle_t;

typedef struct OS_FileSystem OS_FileSystem_t;
typedef OS_FileSystem_t* OS_FileSystem_Handle_t;

/**
 * Use this to indicate that the fs size is dependent on the size of the
 * underlying storage
 */
#define OS_FileSystem_STORAGE_MAX   ((size_t) -1)

/**
 * Use this to assign a Storage to a FileSystem API config
 */
#define OS_FILESYSTEM_ASSIGN_Storage(_rpc_, _dp_)   \
{                                                   \
    .write    = _rpc_ ## _write,                    \
    .read     = _rpc_ ## _read,                     \
    .erase    = _rpc_ ## _erase,                    \
    .getSize  = _rpc_ ## _getSize,                  \
    .getState = _rpc_ ## _getState,                 \
    .dataport = OS_DATAPORT_ASSIGN(_dp_)            \
}


/**
 * Pass file system specific configuration options by setting the respective
 * members of the union.
 */
typedef union
{
    struct
    {
        /**
         * Let FAT create a partiton; without this, it runs in SFD (super-floppy
         * disk) mode which is a non-partitioned disk format.
         */
        bool createPartition;

        /**
         * Disk sector size
         */
        uint16_t sectorSize;

        /**
         * Disk block size
         */
        uint32_t blockSize;

        /**
         * Size of FAT clusters, effectively determines if underlying FAT FS is
         * formatted as FAT12/FAT16 or FAT32.
         */
        uint16_t clusterSize;
    } fatFs;
    struct
    {
        /**
         * Minimum size of a block read. This determines the size of read buffers.
         * This may be larger than the physical read size to improve performance
         * by caching more of the block device.
          */
        uint32_t readSize;

        /**
         * Minimum size of a block write. This determines the size of write
         * buffers. This may be larger than the physical write size to improve
         * performance by caching more of the block device.
         */
        uint32_t writeSize;

        /**
         * Size of a logical block. This does not impact ram consumption and may
         * be larger than the physical erase block. If the physical erase block
         * is larger, littlefs will use that instead. Larger values will be faster
         * but waste more storage when files are not aligned to a block size.
         */
        uint32_t blockSize;

        /**
         * Number of erase cycles before a block is evicted.
         */
        int32_t blockCycles;
    } littleFs;
    struct
    {
        int dummy;
    } spifFs;
} OS_FileSystem_Format_t;

/**
 * Configuration struct to provide FS with callbacks with access to
 * underlying storage
 */
typedef struct
{
    /**
     * Type of file system to use.
     */
    OS_FileSystem_Type_t type;

    /**
     * Size of filesystem to use; set this to OS_FileSystem_STORAGE_MAX so the
     * filesystem will try to use the maximum size reported by the underlying
     * storage.
     */
    size_t size;

    /**
     * Interface to underlying storage; use OS_FILESYSTEM_ASSIGN_Storage() to
     * assign properly.
     */
    struct
    {
        OS_Dataport_t dataport;
        OS_Error_t (*write)(size_t offset, size_t size, size_t* written);
        OS_Error_t (*read)(size_t offset, size_t size, size_t* read);
        OS_Error_t (*erase)(size_t offset, size_t size, size_t* erased);
        OS_Error_t (*getSize)(size_t* size);
        OS_Error_t (*getState)(uint32_t* flags);
    } storage;

    /**
     * Formatting options for FS; can be NULL, then we will use defaults
     * specific to each FS implementation.
     */
    const OS_FileSystem_Format_t* format;
} OS_FileSystem_Config_t;

/**
 * @brief Initialize the FileSystem API
 *
 * This function allocates a FileSystem API object and sets up FileSystem API
 * functionality according to the configuration.
 *
 * @param hFs (required) pointer to handle of OS FileSystem API
 * @param cfg (required) pointer to configuration
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_NOT_SUPPORTED if \p cfg is not supported
 * @retval OS_ERROR_INSUFFICIENT_SPACE if allocation of the API object failed
 */
OS_Error_t
OS_FileSystem_init(
    OS_FileSystem_Handle_t*       hFs,
    const OS_FileSystem_Config_t* cfg);

/**
 * @brief Free a context associated with the FileSystem API
 *
 * @param hFs (required) handle of OS FileSystem API
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_FileSystem_free(
    OS_FileSystem_Handle_t hFs);

/**
 * @brief Format storage with chosen FS
 *
 * @param hFs (required) handle of OS FileSystem API
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 */
OS_Error_t
OS_FileSystem_format(
    OS_FileSystem_Handle_t hFs);

/**
 * @brief Mount storage
 *
 * @param hFs (required) handle of OS FileSystem API
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 */
OS_Error_t
OS_FileSystem_mount(
    OS_FileSystem_Handle_t hFs);

/**
 * @brief Unmount storage
 *
 * @param hFs (required) handle of OS FileSystem API
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 */
OS_Error_t
OS_FileSystem_unmount(
    OS_FileSystem_Handle_t hFs);

/**
 * @brief Open file
 *
 * @param hFs (required) handle of OS FileSystem API
 * @param hFile (required) pointer to file handle
 * @param name (required) name of file
 * @param mode (required) open mode
 * @param flags (required) open flags
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 * @retval OS_ERROR_OUT_OF_BOUNDS if too many file handles are already open
 *
 */
OS_Error_t
OS_FileSystemFile_open(
    OS_FileSystem_Handle_t          hFs,
    OS_FileSystemFile_Handle_t*     hFile,
    const char*                     name,
    const OS_FileSystem_OpenMode_t  mode,
    const OS_FileSystem_OpenFlags_t flags);

/**
 * @brief Close file
 *
 * @param hFs (required) handle of OS FileSystem API
 * @param hFile (required) handle of opened file
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 * @retval OS_ERROR_INVALID_HANDLE if file handle is invalid
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_FileSystemFile_close(
    OS_FileSystem_Handle_t     hFs,
    OS_FileSystemFile_Handle_t hFile);

/**
 * @brief Read from file
 *
 * @param hFs (required) handle of OS FileSystem API
 * @param hFile (required) handle of opened file
 * @param offset (required) offset to start reading at
 * @param len (required) amounts of bytes to read
 * @param buffer (required) buffer to write data to
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 * @retval OS_ERROR_INVALID_HANDLE if file handle is invalid
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_FileSystemFile_read(
    OS_FileSystem_Handle_t     hFs,
    OS_FileSystemFile_Handle_t hFile,
    const size_t               offset,
    const size_t               len,
    void*                      buffer);

/**
 * @brief Write to file
 *
 * @param hFs (required) handle of OS FileSystem API
 * @param hFile (required) handle of opened file
 * @param offset (required) offset to start writing at
 * @param len (required) amounts of bytes to write
 * @param buffer (required) buffer to write data to
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 * @retval OS_ERROR_INVALID_HANDLE if file handle is invalid
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_FileSystemFile_write(
    OS_FileSystem_Handle_t     hFs,
    OS_FileSystemFile_Handle_t hFile,
    const size_t               offset,
    const size_t               len,
    const void*                buffer);

/**
 * @brief Delete file
 *
 * @param hFs (required) handle of OS FileSystem API
 * @param name (required) name of file
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_FileSystemFile_delete(
    OS_FileSystem_Handle_t hFs,
    const char*            name);

/**
 * @brief Get size of file
 *
 * @param hFs (required) handle of OS FileSystem API
 * @param name (required) name of file
 * @param sz (required) file size in bytes
 *
 * @return an error code
 * @retval OS_SUCCESS if operation succeeded
 * @retval OS_ERROR_ABORTED if underlying FS implementation reported an error
 * @retval OS_ERROR_INVALID_PARAMETER if a parameter was missing or invalid
 */
OS_Error_t
OS_FileSystemFile_getSize(
    OS_FileSystem_Handle_t hFs,
    const char*            name,
    size_t*                sz);

/** @} */
