/**
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
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
    OS_FileSystem_Type_FATFS,
    OS_FileSystem_Type_LITTLEFS,
} OS_FileSystem_Type_t;

typedef enum
{
    OS_FileSystem_OpenMode_NONE   = 0,
    OS_FileSystem_OpenMode_RDONLY,
    OS_FileSystem_OpenMode_WRONLY,
    OS_FileSystem_OpenMode_RDWR,
} OS_FileSystem_OpenMode_t;

typedef enum
{
    OS_FileSystem_OpenFlags_NONE        = 0,
    OS_FileSystem_OpenFlags_CREATE      = (1u << 0),
    OS_FileSystem_OpenFlags_EXCLUSIVE   = (1u << 1),
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

typedef union
{
    struct
    {
        bool createPartition;
        uint16_t sectorSize;
        uint32_t blockSize;
        uint16_t clusterSize;
    } fatFs;
    struct
    {
        uint32_t readSize;
        uint32_t writeSize;
        uint32_t blockSize;
        int32_t blockCycles;
    } littleFs;
} OS_FileSystem_Format_t;

/**
 * Configuration struct to provide FS with callbacks with access to
 * underlying storage
 */
typedef struct
{
    /**
     * Type of file system to use
     */
    OS_FileSystem_Type_t type;

    /**
     * Size of filesystem to use
     */
    size_t size;

    /**
     * Interface to underlying storage
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
     */
    const OS_FileSystem_Format_t* format;
} OS_FileSystem_Config_t;

// -----------------------------------------------------------------------------

OS_Error_t
OS_FileSystem_init(
    OS_FileSystem_Handle_t*       hFs,
    const OS_FileSystem_Config_t* cfg);

OS_Error_t
OS_FileSystem_free(
    OS_FileSystem_Handle_t hFs);

OS_Error_t
OS_FileSystem_format(
    OS_FileSystem_Handle_t hFs);

OS_Error_t
OS_FileSystem_mount(
    OS_FileSystem_Handle_t hFs);

OS_Error_t
OS_FileSystem_unmount(
    OS_FileSystem_Handle_t hFs);

// -----------------------------------------------------------------------------

OS_Error_t
OS_FileSystemFile_open(
    OS_FileSystem_Handle_t          hFs,
    OS_FileSystemFile_Handle_t*     hFile,
    const char*                     name,
    const OS_FileSystem_OpenMode_t  mode,
    const OS_FileSystem_OpenFlags_t flags);

OS_Error_t
OS_FileSystemFile_close(
    OS_FileSystem_Handle_t     hFs,
    OS_FileSystemFile_Handle_t hFile);

OS_Error_t
OS_FileSystemFile_read(
    OS_FileSystem_Handle_t     hFs,
    OS_FileSystemFile_Handle_t hFile,
    const size_t               offset,
    const size_t               len,
    void*                      buffer);

OS_Error_t
OS_FileSystemFile_write(
    OS_FileSystem_Handle_t     hFs,
    OS_FileSystemFile_Handle_t hFile,
    const size_t               offset,
    const size_t               len,
    const void*                buffer);

OS_Error_t
OS_FileSystemFile_delete(
    OS_FileSystem_Handle_t hFs,
    const char*            name);

OS_Error_t
OS_FileSystemFile_getSize(
    OS_FileSystem_Handle_t hFs,
    const char*            name,
    size_t*                sz);

/** @} */
