/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief TRENTOS-M filesystem.
 *
 * Translates SEOS filesystem library function with different prefixes, because
 * of different usages, to API function names uniformly.
*/

/**
 * @defgroup seos_fs TRENTOS-M filesystem core
 *
 * @brief   TRENTOS-M filesystem core.
 *
 * @see Filesystem core README.md.
 *
 * @todo    Testing the filesystem -> what happens:
 *          * When the filesystem is full.
 *          * If a write operation was unsuccessful.
 *          * If the filesystem is inconsistent.
 *
 * @todo    Parameter PARTITION_COUNT is temporarily necessary, because the
 *          internal disk/partition objects will be currently created static.
 *          Since the parameter must be set from the outside, the configuration
 *          file is also necessary to use the library.
 *          The parameter PARTITION_COUNT is given the max. size of this array.
 *          Remove occurrences of this parameter, when the objects are created
 *          dynamicly with a "Bitmap".
*/

/**
 * @defgroup    seos_fs_api TRENTOS-M filesystem API translation layer
 *
 * @brief       Translates filesystem library function with different prefixes,
 *              because of different usages, to API function name uniformly.
 *
 * @see Filesystem core README.md.
 *
 * @ingroup     seos_fs
*/


#pragma once

#include "OS_Error.h"
#include <stddef.h>

#include "seos_fs_conf.h"
#include "seos_fs_datatypes.h"

#if defined(SEOS_FS_BUILD_AS_COMPONENT)
#include "seos_fs_conf.h"
#include <string.h>

#include <camkes.h>


#ifndef OS_FS_DATABUFFER_SIZE
#error OS_FS_DATABUFFER_SIZE is not defined
#endif


#ifndef OS_FS_GET_PROPERTY_DATAPORT_BUFFER
#error OS_FS_GET_PROPERTY_DATAPORT_BUFFER is not defined
#endif


/**
 * @brief   The CAmkES data buffer for sending data over the component's
 *          interface.
 *
 * @ingroup seos_fs_api
*/
dataport_ptr_t buffer_send;
/**
 * @brief   The CAmkES data buffer for receiving data from the component's
 *          interface.
 *
 * @ingroup seos_fs_api
*/
dataport_ptr_t buffer_receive;
#elif defined (SEOS_FS_BUILD_AS_LIB)
#include "api_fs.h"
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
#include "api_fs_resolve_layer.h"
#endif


//------------------------------------------------------------------------------
static inline int
OS_Filesystem_validatePartitionHandle(
    hPartition_t phandle)
{
#if defined (SEOS_FS_BUILD_AS_LIB)
    return (NULL != phandle);
#else
    return (phandle >= 0);
#endif
}


//------------------------------------------------------------------------------
static inline int
OS_Filesystem_validateFileHandle(
    hFile_t fhandle)
{
#if defined (SEOS_FS_BUILD_AS_LIB)
    return (NULL != fhandle);
#else
    return (fhandle >= 0);
#endif
}


/***********************/
/* Partition functions */
/***********************/
#if !defined(SEOS_FS_BUILD_AS_COMPONENT)
/**
 * @brief Initializes file and partition handle by calling handle manager layer.
 *
 * The access function to partition for read and/or write can be set by
 * `open_flag` argument. The following options are available:
 *
 * * FS_PARTITION_MODE_R  - register only read low level partition function
 * * FS_PARTITION_MODE_RW - register read, write and format low level partition
 *                          functions.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                         If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER         If any of the required
 *                                               parameters is missing or wrong.
 * @retval  SEOS_ERROR_FS_NO_DISK                If the partition doesn't exist.
 * @retval  SEOS_ERROR_FS_INIT                   If failed to init.
 * @retval  SEOS_ERROR_FS_INVALID_PARTITION_MODE If the partition's access mode
 *                                               is invalid.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_init(
    uint8_t drv_id, //!< [in] The partitions identifier.
    int open_flag   //!< [in] The partition's access function.
)
{
#if defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_partition_init(drv_id, open_flag);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_partition_init(drv_id, open_flag);
#endif
}
#endif // SEOS_FS_BUILD_AS_COMPONENT

/**
 * @brief Opens the partition.
 *
 * This function checks if the partition can be used, creates the partition's
 * handle and opens the partition.
 *
 * @note The allocation of space for this handle will be manage internally.
 *
 * @return  partition handle (pointer to the struct)
 *
 * @retval  >= 0 If handle is valid.
 * @retval  -1   If handle is invalid.
 *
 * @ingroup seos_fs_api
*/
static inline hPartition_t
OS_Filesystem_open(
    uint8_t drv_id /**< [in] The partition's identifier. */)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_partition_open(drv_id);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_partition_open(drv_id);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_partition_open(drv_id);
#endif
}

/**
 * @brief Creates a filesystem image header.
 *
 * Filesystem can be created as FAT12, FAT16 or FAT32 filesystem. It depends on
 * the format argument. The following options are available:
 *
 * * FS_TYPE_FAT12
 * * FS_TYPE_FAT16
 * * FS_TYPE_FAT32
 *
 * FAT12, FAT16, and FAT32 variants of the FAT filesystems have clear limits
 * based on the number of clusters and the number of sectors per cluster (1, 2,
 * 4, ..., 128). For the typical value of 512 bytes per sector:
 *
 * \code
 * FAT12 range   :      1 to 4.084 clusters       :   1 to        12 sectors per copy of FAT
 * FAT16 range   :  4.085 to 65.524 clusters      :  16 to       256 sectors per copy of FAT
 * FAT32 range   : 65.525 to 268.435.444 clusters : 512 to 2.097.152 sectors per copy of FAT
 *
 * FAT12 minimum :  1 sector per cluster ×      1 clusters = 512 bytes (0.5 KB)
 * FAT16 minimum :  1 sector per cluster ×  4.085 clusters = 2.091.520 bytes (2.042.5 KB)
 * FAT32 minimum :  1 sector per cluster × 65.525 clusters = 33.548.800 bytes (32.762.5 KB)
 *
 * FAT12 maximum : 64 sectors per cluster ×       4.084 clusters = 133.824.512 bytes (≈ 127 MB)
 * FAT16 maximum : 64 sectors per cluster ×      65.524 clusters = 2.147.090.432 bytes (≈2.047 MB)
 * FAT32 maximum :  8 sectors per cluster × 268.435.444 clusters = 1.099.511.578.624 bytes (≈1.024 GB)
 * FAT32 maximum : 16 sectors per cluster × 268.173.557 clusters = 2.196.877.778.944 bytes (≈2.046 GB) ->bigger size
 * \endcode
 *
 * On the creation of the filesystem, it can be decided, if partition is
 * formatted firstly or only partitions data are overwritten.
 *
 * The following flags are available:
 *
 * * FS_PARTITION_OVERWRITE_CREATE - The partition will not be formatted and
 *                                   only partitions data are overwritten.
 * * FS_PARTITION_FORMAT_CREATE    - The partition will be formatted first and
 *                                   then created.
 *
 * If the fs_format parameters is 0, than will be used the default value.
 *
 * | Property  | The default value          |
 * |-----------|----------------------------|
 * | fs_format | FS_PARTITION_FORMAT_CREATE |
 *
 * @note    This function shall not be used, if the partition has been
 *          configured with the write protection.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                      If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER      If any of the required parameters is
 *                                            missing or wrong.
 * @retval  SEOS_ERROR_FS_NO_DISK             If partition does not exist.
 * @retval  SEOS_ERROR_INVALID_HANDLE         If partition handle is not valid.
 * @retval  SEOS_ERROR_FS_OPERATION_DENIED    If partition was opened in the
 *                                            read-only mode.
 * @retval  SEOS_ERROR_FS_FORMAT_FS           If partition can not be formatted.
 * @retval  SEOS_ERROR_FS_CREATE_FS           If fail to create filesystem.
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE      If failed to resolve handle.
 * @retval  SEOS_ERROR_FS_OPEN                If partition is not open.
 * @retval  SEOS_ERROR_FS_INVALID_FILESYSTEM  If filesystem type is
 *                                            unsupported.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_create(
    hPartition_t handle,     //!< [in] The partition handle (integer value).
    uint8_t format_option,   //!< [in] The format option.
    uint64_t partition_size, //!< [in] Size of the partition.
    uint16_t sector_size,    //!< [in] Size of the sector.
    uint32_t cluster_size,   //!< [in] Size of the cluster.

    uint16_t offset_sectors_count,   //!< [in] Reserved sectors count.
    uint16_t file_dir_entry_count,   //!< [in] File/dir entries count.
    uint32_t fs_header_sector_count, //!< [in] Header sectors count.

    int fs_format /**!< [in] Format or overwrite the partition. */)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_partition_fs_create(
               handle,
               format_option,
               partition_size,
               sector_size,
               cluster_size,
               offset_sectors_count,
               file_dir_entry_count,
               fs_header_sector_count,
               fs_format);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_partition_fs_create(
               &handle,
               format_option,
               partition_size,
               sector_size,
               cluster_size,
               offset_sectors_count,
               file_dir_entry_count,
               fs_header_sector_count,
               fs_format);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_partition_fs_create(
               handle,
               format_option,
               partition_size,
               sector_size,
               cluster_size,
               offset_sectors_count,
               file_dir_entry_count,
               fs_header_sector_count,
               fs_format);
#endif
}

/**
 * @brief   Mounts the partition.
 *
 * @param   handle: partition handle (pointer to the struct)
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                      If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER      If any of the required parameters
 *                                            is missing or wrong.
 * @retval  SEOS_ERROR_FS_NO_DISK             If partition doesn't exist.
 * @retval  SEOS_ERROR_FS_PARTITION_NOT_READY If partition is not ready for use.
 * @retval  SEOS_ERROR_FS_PARTITION_READ      If disk read operation has an
 *                                            error.
 * @retval  SEOS_ERROR_FS_INVALID_FILESYSTEM  If no valid supported filesystem
 *                                            is recognized.
 * @retval  SEOS_ERROR_INVALID_HANDLE         If partition handle is not valid.
 * @retval  SEOS_ERROR_FS_MOUNT               If fail to mount a partition.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_mount(
    hPartition_t handle /**!< [in] Partition handle (integer value) */)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_partition_fs_mount(handle);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_partition_fs_mount(&handle);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_partition_fs_mount(handle);
#endif
}

/**
 * @brief Unmounts the partition.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                      If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER      If any of the required parameters is
 *                                            missing or wrong.
 * @retval  SEOS_ERROR_INVALID_HANDLE         If partition handle is not valid.
 * @retval  SEOS_ERROR_FS_NO_DISK             If partition does not exist.
 * @retval  SEOS_ERROR_FS_UNMOUNT             If failed to unmount a partition.
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE      If failed to resolve handle.
 * @retval  SEOS_ERROR_FS_INVALID_FILESYSTEM  If filesystem type is
 *                                            unsupported.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_unmount(
    hPartition_t handle /**!< [in] Partition handle (integer value) */)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_partition_fs_unmount(handle);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_partition_fs_unmount(&handle);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_partition_fs_unmount(handle);
#endif
}

/**
 * @brief   Wipes the partition.
 *
 * This function deletes all data by writing 0 to all blocks.
 *
 * @note    Formatting is not allowed if the partition has been configured with
 *          the write protection.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                   If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER   If any of the required parameters is
 *                                         missing or wrong.
 * @retval  SEOS_ERROR_INVALID_HANDLE      If the partition handle is not valid.
 * @retval  SEOS_ERROR_FS_NO_DISK          If the partition doesn't exist.
 * @retval  SEOS_ERROR_FS_OPERATION_DENIED If the partition was opened in
 *                                         read-only mode.
 * @retval  SEOS_ERROR_FS_FORMAT_FS        If the partition can not be
 *                                         formatted.
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE   If failed to resolve handle.
 * @retval  SEOS_ERROR_FS_OPEN             If partition is not open.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_wipe(
    hPartition_t handle /**!< [in] Partition handle (integer value) */)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_partition_wipe(handle);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_partition_wipe(&handle);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_partition_wipe(handle);
#endif
}

/**
 * @brief Closes the partition.
 *
 * This function deregisteres internal objects and closes the given partition.
 *
 * Implicitly all open file handles will be closed.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                 If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER If any of the required parameters is
 *                                       missing or wrong.
 * @retval  SEOS_ERROR_INVALID_HANDLE    If the partition handle is not valid.
 * @retval  SEOS_ERROR_FS_NO_DISK        If the partition does doesn't exist.
 * @retval  SEOS_ERROR_FS_UNMOUNT        If the partition can not be unmounted.
 * @retval  SEOS_ERROR_FS_CLOSE          If the partition can not be closed.
 * @retval  SEOS_ERROR_FS_DELETE_HANDLE  If internal handle structure can not be
 *                                       deleted.
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE If failed to resolve the handle.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_close(
    hPartition_t handle /**!< [in] Partition handle (integer value) */)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_partition_close(handle);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_partition_close(&handle);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_partition_close(handle);
#endif
}


/******************/
/* File functions */
/******************/
/**
 * @brief Opens a file.
 *
 * This function check the partition handle and opens the file.
 *
 * When opening a file, information can be provided on how to open the file -
 * e.g. read, write, create, aso.
 *
 * A file can be opened with different flags:
 *
 * * FA_READ          - Read only.
 * * FA_WRITE         - Write only.
 * * FA_OPEN_EXISTING - Opens only if file exists.
 * * FA_CREATE_NEW    - Creates a file.
 * * FA_CREATE_ALWAYS - Creates a file, if a file exists it will be overwrite.
 * * FA_OPEN_ALWAYS   - Opens only.
 * * FA_OPEN_APPEND   - Opens in the append mode.
 *
 * This function needs a valid partition handle and returns a file handle as
 * a pointer to the struct.
 *
 * @return  The file handle.
 *
 * @retval Pointer to the struct if handle is valid
 * @retval NULL pointer if handle is invalid
 *
 * @ingroup seos_fs_api
*/
static inline hFile_t
OS_Filesystem_openFile(
    hPartition_t handle, //!< [in] Partition handle (integer value).
    const char*  name,   //!< [in] Name of a file.
    int          flag    //!< [in] Instruction for file opening.
)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_file_open(handle, name, flag);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_file_open(&handle, name, flag);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_file_open(handle, name, flag);
#endif
}

/**
 * @brief Closes the file.
 *
 * This function deregisters internal objects and closes the file.
 * It needs a valid file handle and returns a SEOS error code.
 *
 * The deallocation of file handle pointer will be handle internally.
 * The handle object is set to NULL in this function and the extern object
 * points to NULL.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                 If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER If any of the required parameters is
 *                                       missing or wrong.
 * @retval  SEOS_ERROR_INVALID_HANDLE    If file handle is not valid.
 * @retval  SEOS_ERROR_FS_STRUCTURE      If the internal structure can not be
 *                                       resolved.
 * @retval  SEOS_ERROR_FS_DELETE_HANDLE  If the internal handle structure can
 *                                       not be deleted.
 * @retval  SEOS_ERROR_FS_NO_DISK        If partition does doesn't exist.
 * @retval  SEOS_ERROR_FS_LIB            If library throws an error.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_closeFile(
    hFile_t handle /**! file handle (integer value) */)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_file_close(handle);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_file_close(&handle);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_file_close(handle);
#endif
}

/**
 * @brief Reads the file.
 *
 * This function reads data from the file and copies it into the given buffer.
 *
 * The buffer argument is a pointer, which shall not be NULL and is allocated
 * from the caller function.
 *
 * @note It needs a valid file handle and returns a SEOS error code.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                      If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER      If any of the required parameters
 *                                            is missing or wrong.
 * @retval  SEOS_ERROR_INVALID_HANDLE         If the file handle is not valid.
 * @retval  SEOS_ERROR_FS_STRUCTURE           If the internal structure can not
 *                                            be resolved.
 * @retval  SEOS_ERROR_FS_NO_DISK             If partition does doesn't exist.
 * @retval  SEOS_ERROR_FS_FILE_NOT_FOUND      If the file was not found on the
 *                                            partition.
 * @retval  SEOS_ERROR_FS_LIB                 If the library throws an error.
 * @retval  SEOS_ERROR_FS_DATABUFFER_OVERFLOW If the databuffer is too small.
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE      If failed to resolve the handle.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_readFile(
    hFile_t handle, //!< [in]  The file handle (integer value).
    long offset,    //!< [in]  The offset when reading the file.
    long len,       //!< [in]  The bytes count to read.
    void* buffer    //!< [out] Reads content into this buffer.
)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    OS_Error_t retval = SEOS_SUCCESS;
    const void* buf = (void*)0;

    // checks databuffer length
    if (OS_FS_DATABUFFER_SIZE < len)
    {
        return SEOS_ERROR_FS_DATABUFFER_OVERLOW;
    }

    // Cast/Wrap pointer to dataport pointer
    buffer_receive = dataport_wrap_ptr(OS_FS_GET_PROPERTY_DATAPORT_BUFFER);

    // Call filesystem API function to read from a file
    retval = api_fs_component_file_read(handle, offset, len, buffer_receive);

    buf = dataport_unwrap_ptr(buffer_receive);

    // copy data from databuffer into buffer
    memcpy(buffer, buf, (size_t)len);

    return retval;
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_file_read(&handle, offset, len, buffer);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_file_read(handle, offset, len, buffer);
#endif
}

/**
 * @brief Writes to the file.
 *
 * This function writes data from a given buffer to the file.
 *
 * The buffer argument is a pointer, which shall not be NULL and is allocated
 * from the caller function.
 *
 * It needs a valid file handle and returns a TRENTOS-M error code.
 *
 * Writing process is not allowed if the partition has been configured with the
 * write protection.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                     If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER     If any of the required parameters
 *                                           is missing or wrong.
 * @retval  SEOS_ERROR_INVALID_HANDLE        If the file handle is not valid.
 * @retval  SEOS_ERROR_FS_STRUCTURE          If the internal structure can not
 *                                           resolve.
 * @retval  SEOS_ERROR_FS_NO_DISK            If the partition doesn't exist.
 * @retval  SEOS_ERROR_FS_FILE_NOT_FOUND     If the file was not found on
 *                                           partition.
 * @retval  SEOS_ERROR_FS_OPERATION_DENIED   If the partition was opened in the
 *                                           read-only mode.
 * @retval  SEOS_ERROR_FS_DATABUFFER_OVERLOW If databuffer is too small.
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE     If failed to resolve handle.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_writeFile(
    hFile_t handle, //!< [in] The file handle (integer value).
    long offset,    //!< [in] The offset when writing a file.
    long len,       //!< [in] The bytes count to write.
    void* buffer    //!< [in] Writes content from this buffer.
)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    // checks databuffer length
    if (OS_FS_DATABUFFER_SIZE < len)
    {
        return SEOS_ERROR_FS_DATABUFFER_OVERLOW;
    }

    // copy data into databuffer
    memcpy(OS_FS_GET_PROPERTY_DATAPORT_BUFFER, buffer, (size_t)len);

    // Cast/Wrap pointer to dataport pointer
    buffer_send = dataport_wrap_ptr(OS_FS_GET_PROPERTY_DATAPORT_BUFFER);

    // Call filesystem API function to write into a file
    return api_fs_component_file_write(handle, offset, len, buffer_send);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_file_write(&handle, offset, len, buffer);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_file_write(handle, offset, len, buffer);
#endif
}

/**
 * @brief Deletes the file.
 *
 * This function needs a valid partition handle and returns a TRENTOS-M error
 * code.
 *
 * Delete process is not allowed if the partition has been configured with the
 * write protection.
 *
 * @return  An error code.
 *
 * @retval  SEOS_SUCCESS                   If all right.
 * @retval  SEOS_ERROR_INVALID_PARAMETER   If any of the required parameters is
 *                                         missing or wrong.
 * @retval  SEOS_ERROR_INVALID_HANDLE      If partition handle is not valid.
 * @retval  SEOS_ERROR_FS_NO_DISK          If partition doesn't exist.
 * @retval  SEOS_ERROR_FS_OPERATION_DENIED If partition was opened in read-only
 *                                         mode.
 * @retval  SEOS_ERROR_FS_LIB              If library throws an error.
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE   If failed to resolve the handle.
 *
 * @ingroup seos_fs_api
*/
static inline OS_Error_t
OS_Filesystem_deleteFile(
    hPartition_t handle, //!< [in] The partition handle (integer value).
    const char* name     //!< [in] The name of the file.
)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_file_delete(handle, name);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_file_delete(&handle, name);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_file_delete(handle, name);
#endif
}

/**
 * @brief Gets the file's size.
 *
 * This function needs a valid partition handle and returns the file size.
 *
 * @return Size of the file.
 *
 * @retval  size - File's size if file exists.
 * @retval  -1   - If an error occurred.
 *
 * @ingroup seos_fs_api
*/
static inline int64_t
OS_Filesystem_getSizeOfFile(
    hPartition_t handle, //!< [in] The partition handle (integer value).
    const char* name     //!< [in] The name of the file.
)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_file_getSize(handle, name);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_file_getSize(&handle, name);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_file_getSize(handle, name);
#endif
}
