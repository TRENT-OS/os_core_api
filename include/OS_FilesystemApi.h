/*
   * Provided SEOS filesystem.
   *
   * Translates SEOS filesystem library function with different prefixes, because of different usages, to API function name uniformly.
   *
   *  Copyright (C) 2019, Hensoldt Cyber GmbH
*/

/**
 * @defgroup    seos_fs SEOS filesystem core
 *
 * @brief       Provided SEOS filesystem.
 * @details     Include filesystem headers from SEOS filesystem core.\n
 *              The filesystem can be done in two different ways. Use as a library or use as a separate component.
 *              If decided for use the filesystem as a seperate component, note that a interface file and a CAmkES component file must created and configured.
 *              Additional must be create an entry for component in the CMakeLists.txt. \n
 *              \n
 *
 *              <b> Dependencies </b> \n
 *              The filesystem uses filesystem specific libraries. Furthermore it connects to the SEOS partition manager, which connects to a storage driver.
 *              For logging operations SEOS libraries are needed. \n
 *
 *              <b> Creating filesystem interface file: </b> \n
 *              The names for the interface functions can be found in OS_FilesystemApi.h. Make sure that the interface file includes the SEOS filesystem datatypes.
 *              \code
 *                  procedure <INTERFACE_NAME> {
 *                      hPartition_t     OS_FilesystemApi_open(uint8_t drv_id);
 *                      seos_err_t OS_FilesystemApi_create(hPartition_t handle,
 *                                                           uint8_t format_option,
 *                                                           uint64_t partition_size,
 *                                                           uint16_t sector_size,
 *                                                           uint32_t cluster_size,
 *                                                           uint16_t offset_sectors_count,
 *                                                           uint16_t file_dir_entry_count,
 *                                                           uint32_t fs_header_sector_count,
 *                                                           int fs_format);
 *                      seos_err_t OS_FilesystemApi_mount(hPartition_t handle);
 *                      seos_err_t OS_FilesystemApi_unmount(hPartition_t handle);
 *                      seos_err_t OS_FilesystemApi_wipe(hPartition_t handle);
 *                      seos_err_t OS_FilesystemApi_close(hPartition_t handle);
 *
 *                      hFile_t          OS_FilesystemApi_openFile(hPartition_t handle, string name, int flag);
 *                      seos_err_t OS_FilesystemApi_closeFile(hFile_t handle);
 *                      seos_err_t OS_FilesystemApi_readFile(hFile_t handle, long offset, long len, in dataport_ptr_t buffer);
 *                      seos_err_t OS_FilesystemApi_writeFile(hFile_t handle, long offset, long len, in dataport_ptr_t buffer);
 *                      seos_err_t OS_FilesystemApi_deleteFile(hPartition_t handle, string name);
 *                      int64_t          OS_FilesystemApi_getSizeOfFile(hPartition_t handle, string name);
 *                  };
 *              \endcode
 *
 *              <b> CAmkES configuration file: </b> \n
 *              Do not touch the filesystem library implementation, so configure the prefix in CAmkES configuration file by providing filesystem interface "api_fs_component".
 *              \code
 *                  component <COMPONENT_NAME> {
 *                      provides <INTERFACE_NAME> api_fs_component;
 *                      # ...
 *                  }
 *              \endcode
 *
 *              <b> CMakeLists.txt file: </b> \n
 *              If use the filesystem as library, there exists two options for use the filesystem handles. \n
 *              It is distinguished in use in a complex struct or a integer value. \n
 *              No matter what system will have be created, for each build design exists his own define. In order to build one of the variants,
 *              the corresponding define must be specified in the CMakeLists.txt file. There is always only one define variable possible at the same time.
 *              \code
 *                  Define                            | Description
 *                  ----------------------------------|---------------------------------------------------------------------------
 *                  SEOS_FS_BUILD_AS_COMPONENT        | use the filesystem as component (handles are integer values)
 *                  SEOS_FS_BUILD_AS_LIB              | use the filesystem as library (handles are pointer to their structs)
 *                  SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE | use the filesystem as library (handles will be resolved to integer values)
 *              \endcode
 *
 *              The library is required to have a set include path variable to the base directories of the FS core and the filesystem library itself.
 *              The set includes in this library are always defined with:
 *              \code
 *                   #include "<FILE_NAME>"
 *              \endcode
 *
 *              <b> Configuration file: </b> \n
 *              Make a configuration file and set the required defines. To use a separate configuration file add a folder (like config) in your application, create a
 *              filesystem configuration file and set the define parameter "SEOS_FS_CONFIG_H_FILE" follow by the path.
 *              Furtgermore, will be checked the build paramters. Make sure that only one build define parameter is set at the same time.
 *              Otherwise it's get an error message. \n
 *              \n
 *              The filesystem can be influenced in creating in all of the following properties. \n
 *              If no set this parameters the default values will be used.
 *              \code
 *                  Property                       | Configuration define     | default value
 *                  -------------------------------|--------------------------|--------------------------------------
 *                  size of sector                 | SECTOR_SIZE              | 512 bytes
 *                  size of cluster                | CLUSTER_SIZE_FAT         | 512 bytes
 *                  reserved sectors count         | OFFSET_SECTORS_COUNT_FAT | 3 -> [min. 1 (FAT12/FAT16), 3(FAT32)]
 *                  count file/dir entries         | FILE_DIR_ENTRY_COUNT     | 16 entries (only FAT12/FAT16)
 *                  count header sectors           | FS_HEADER_SECTOR_COUNT   | 1 * 512 bytes
 *                  max. file handle per partition | FILE_HANDLE_COUNT        | 10
 *              \endcode
 *
 *              The include path for the util functions must be inside quotation marks. Don't forget to set the relative include path in the CMakeLists.txt inside
 *              the include field for this functionalities. \n
 *              If filesystem build as component, there are additional configuration parameters. So the filesystem implementation files needn't be touched.
 *              \code
 *                  Property                      | Configuration define            | default value
 *                  ------------------------------|---------------------------------|---------------------
 *                  size of buffer dataport_ptr_t | OS_FS_DATABUFFER_SIZE                 | - empty - / not set
 *                  name of buffer dataport_ptr_t | OS_FS_GET_PROPERTY_DATAPORT_BUFFER | - empty - / not set
 *              \endcode
 *
 *              This both property are necessary defines for connection between the several components.
 *              If there not set, by compiling throws compiler an error message. \n
 *              \n
 *              If setup FAT12 or FAT16 filesystem please note this:
 *              \code
 *                  OFFSET_SECTORS_COUNT_FAT = 0 -> if no filesystem info and backup filesystem header is configured
 *                  OFFSET_SECTORS_COUNT_FAT = 1 -> if filesystem info and backup filesystem header is configured
 *                                                  FF_FS_FSINFO can be set in conf.h
 *              \endcode
 *
 *              If setup FAT32 filesystem please note this:
 *              \code
 *                  OFFSET_SECTORS_COUNT_FAT = 0 -> if no filesystem info and backup filesystem header is configured
 *                                                  && FS_HEADER_SECTOR_COUNT = 1
 *
 *                  OFFSET_SECTORS_COUNT_FAT = 1 -> if no filesystem info and backup filesystem header is configured
 *                                                  && FS_HEADER_SECTOR_COUNT = 0
 *
 *                  OFFSET_SECTORS_COUNT_FAT = 3 -> if filesystem info and backup filesystem header is configured...
 *                                                  ...one filesystem header structure...
 *                                                  ...for each filesystem header, will be needed a filesystem info structure
 *                                                  (summarize: FAT-header + filesystem info structure + backup FAT-header + filesystem info structure)
 *                                                  FF_FS_FSINFO can be set in conf.h
 *
 *                  OFFSET_SECTORS_COUNT_FAT = 4 -> if filesystem info and backup filesystem header is configured
 *                                                  && FS_HEADER_SECTOR_COUNT = 0
 *                                                  FF_FS_FSINFO can be set in conf.h
 *              \endcode
 *
 *              <b> FAT filesystem options </b> \n
 *              The FAT filesystem library allows to use different FAT formats. There are various defines for this, which can be found in the file seos_api_datatypes.h.
 *              When creating a filesystem on a partition different defnies are available:
 *              \code
 *                  FS_TYPE_FAT12
 *                  FS_TYPE_FAT16
 *                  FS_TYPE_FAT32
 *              \endcode
 *
 *              <b> How to use this library currently? </b> \n
 *              Includes \n
 *              \n
 *              In an application is required the header file: "OS_FilesystemApi.h".
 *              The OS_FilesystemApi.h provides filesystem datatypes (handle datatypes), includes configuration and API functions. \n
 *              \n
 *              Preperations \n
 *              \n
 *              The filesystem library must be initialized before used for the first time.
 *              Dependency of the file system on the partition manager ensures that not only the file system itself but also the partition manager are initialized.
 *              If the filesystem library is implemented as an dedicated component, the initialization can be performed in the "\_\_init()" function.
 *              Create a file in the filesystem component and implement the "\_\_init()" function. The content in this function will be the same like the initialization
 *              in the application itself. \n
 *              \n
 *              Partition manager initialization \n
 *              \n
 *              The header files for the partition manager must be included "seos_pm.h".
 *              Due to the possibility of using the partition manager as a library or as a stand alone component, when using the parition manager as an dedicated
 *              component it is already executed in its "\_\_init()" function.
 *              The "partition_manager_init" function uses deep level read and write function with NVM-API. So create first an NVM object and passes to the "init" function.
 *              They are register and initialize the internal low-level disk functions and the filesystem structure object for each partition and for the partition manager.
 *              \code
 *                  #include "<PATH>/seos_pm.h"
 *
 *                  // create NVM object
 *                  // ...
 *
 *                  // call constructor from NVM
 *                  // ...
 *
 *                  // necessary if the partition manager is not used as a standalone component
 *                  partition_manager_init(<NVM_OBJECT>)
 *                  // ...
 *              \endcode
 *
 *              The number of hard disk partitions can be retrieved from the function "partition_manager_get_info_disk".
 *              From function "partition_manager_get_info_partition" can be taken the identifier of a partition.
 *              This is required for the registration of the internal filesystem functions. Look at the example code:
 *              \code
 *                  pm_disk_data_t pm_disk_data = NULL;
 *                  pm_partition_data_t pm_partition_data = NULL;
 *                  uint8_t partition_id = 2;
 *
 *                  // pm_disk_data_t contain information about the disk
 *                  // pm_disk_data_t::partition_count -> get a valid partition id
 *                  if( (pm_disk_data = partition_manager_get_info_disk()) == NULL){
 *                      // ...
 *                  }
 *
 *                  if(partition_id < 0 || partition_id > pm_disk_data->partition_count){
 *                      // ...
 *                  }
 *
 *                  // pm_partition_data_t contain information about the partition
 *                  // pm_partition_data_t::partition_size -> get the partition size
 *                  if( (pm_partition_data = partition_manager_get_info_partition(partition_id)) == NULL){
 *                      // ...
 *                  }
 *              \endcode
 *
 *              Filesystem initialization \n
 *              \n
 *              First initialize the filesystem and open it. If initialization failed, open a partition is not possible.
 *              \code
 *                  #include "<PATH>/OS_FilesystemApi.h"
 *
 *                  hPartition_t phandle;
 *
 *                  OS_FilesystemApi_init(pm_partition_data.partition_id, 0);
 *
 *                  phandle = OS_FilesystemApi_open(pm_partition_data.partition_id);
 *              \endcode
 *
 *              Create a FAT image on disk or open an exists FAT image on partition. This function can be only done, if the partition was opened.
 *              \code
 *                  OS_FilesystemApi_create(phandle, <FAT_TYPE>, <PARTITION_SIZE>, ...);
 *
 *                  // ... or ...
 *
 *                  OS_FilesystemApi_mount(phandle);
 *              \endcode
 *
 *              After that it will be possible dealing with files. File operations are open, read, write, delete, get_size and close.
 *              A partition can be wiped with:
 *              \code
 *                  OS_FilesystemApi_wipe(phandle);
 *              \endcode
 *
 *              Make sure that the partition is still open. If not it will get an error message.
 *              Before close a partition the filesystem must be unmount.
 *              \code
 *                  OS_FilesystemApi_unmount(phandle);
 *
 *                  OS_FilesystemApi_close(phandle);
 *              \endcode
 *
 * @todo        Testing the filesystem -> what happens: \n
 *                  - when the filesystem is full \n
 *                  - if a write operation was unsuccessful \n
 *                  - if the filesystem is inconsistent \n
 *              \n
 *              Parameter PARTITION_COUNT is temp. necessary, because the internal disk/partition objects will be created currently static.
 *              Since the parameter must be set from the outside, the configuration file is also necessary to use the library.
 *              The parameter PARTITION_COUNT is given the max. size of this array.
 *              Remove this parameter in all occured cases, when update the object creating dynamicly with "Bitmap".
*/

/**
 * @defgroup    seos_fs_api SEOS filesystem API translation layer
 *
 * @brief       Translates filesystem library function with different prefixes, because of different usages, to API function name uniformly.
 * @details     This layer implements filesystem API interface. There will be only translate the internaly filesystem library functions to
 *              the API interface names. Depend on the configured build paramters will be call his respective implemention function. \n
 *              There are three different ways to use this API interface:
 *              \code
 *                  Define                            | Description
 *                  ----------------------------------|---------------------------------------------------------------------------
 *                  SEOS_FS_BUILD_AS_COMPONENT        | use the filesystem as component (handles are integer values)
 *                  SEOS_FS_BUILD_AS_LIB              | use the filesystem as library (handles are pointer to their structs)
 *                  SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE | use the filesystem as library (handles will be resolved to integer values)
 *              \endcode
 *
 *              By using the component-based design (SEOS_FS_BUILD_AS_COMPONENT) the functions "file_read" and "file_write" are using
 *              CAmkES dataports and databuffer. \n
 *              CAmkES databuffer has in the current configuration OS_FS_DATABUFFER_SIZE bytes. This define must be set in the config file.
 *              If this size is exceeded a corresponding error code will be returned.
 *              The cast from the passed databuffer to a CAmkES dataport is covered by her functions itself.
 *              This does not have to be done from outside.
 *
 * @ingroup     seos_fs
*/


#pragma once

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
 * @brief   buffer_send is a CAmkES data buffer for send data over component interface.
 *
 * @ingroup seos_fs_api
*/
dataport_ptr_t buffer_send;
/**
 * @brief   buffer_receive is a CAmkES data buffer for receive data from component interface.
 *
 * @ingroup seos_fs_api
*/
dataport_ptr_t buffer_receive;
#elif defined (SEOS_FS_BUILD_AS_LIB)
#include "api_fs.h"                 // include path to fs-core must be set in cmakelists.txt
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
#include "api_fs_resolve_layer.h"   // include path to fs-core must be set in cmakelists.txt
#endif


//------------------------------------------------------------------------------
static inline int
OS_FilesystemApi_validatePartitionHandle(
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
OS_FilesystemApi_validateFileHandle(
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
 * @details %OS_FilesystemApi_init defines the API interface name to initialize the filesystem core and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function initialize file and partition handling by calling handle manager layer.
 *          The access function to partition for read and/or write can be set by open_flag argument. The following options are available:
 *          \code
 *              FS_PARTITION_MODE_R  - register only read low level partition function
 *              FS_PARTITION_MODE_RW - register read, write and format low level partition functions
 *          \endcode
 *
 * @param   drv_id:     name of partition
 * @param   open_flag:  partition access function
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                         if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER         if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_FS_NO_DISK                if partition not exists
 * @retval  SEOS_ERROR_FS_INIT                   if failed to init
 * @retval  SEOS_ERROR_FS_INVALID_PARTITION_MODE if partition access mode is invalid
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_init(
    uint8_t drv_id,
    int open_flag)
{
#if defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_partition_init(drv_id, open_flag);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_partition_init(drv_id, open_flag);
#endif
}
#endif // SEOS_FS_BUILD_AS_COMPONENT


/**
 * @details %OS_FilesystemApi_open defines the API interface name to open a partition and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function checks if a partition can be used, creates an partitions handle and open the partition.
 *          The return value is affected by the set build parameter.
 *
 * @param   drv_id: name of partition
 *
 * @return  partition handle
 *
 * @retval  pointer to struct if handle is valid and build parameter SEOS_FS_BUILD_AS_LIB is set
 * @retval  NULL pointer      if handle is invalid and build parameter SEOS_FS_BUILD_AS_LIB is set
 * @retval  >= 0              if handle is valid and build parameter SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE or SEOS_FS_BUILD_AS_COMPONENT is set
 * @retval    -1              if handle is invalid and build parameter SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE or SEOS_FS_BUILD_AS_COMPONENT is set
 *
 *
 * @ingroup seos_fs_api
*/
static inline hPartition_t
OS_FilesystemApi_open(
    uint8_t drv_id)
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
 * @details %OS_FilesystemApi_create defines the API interface name to create a filesystem and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function create a filesystem image header.
 *          Filesystem can be created as FAT12, FAT16 or FAT32 filesystem. It depends on format option
 *          argument with following options:
 *          \code
 *              FS_TYPE_FAT12
 *              FS_TYPE_FAT16
 *              FS_TYPE_FAT32
 *          \endcode
 *
 *          FAT12, FAT16, and FAT32 variants of the FAT filesystems have clear limits based on the number of clusters
 *          and the number of sectors per cluster (1, 2, 4, ..., 128). For the typical value of 512 bytes per sector:
 *          \code
 *              FAT12 range   :      1 to 4.084 clusters       :   1 to        12 sectors per copy of FAT
 *              FAT16 range   :  4.085 to 65.524 clusters      :  16 to       256 sectors per copy of FAT
 *              FAT32 range   : 65.525 to 268.435.444 clusters : 512 to 2.097.152 sectors per copy of FAT
 *
 *              FAT12 minimum :  1 sector per cluster ×      1 clusters = 512 bytes (0.5 KB)
 *              FAT16 minimum :  1 sector per cluster ×  4.085 clusters = 2.091.520 bytes (2.042.5 KB)
 *              FAT32 minimum :  1 sector per cluster × 65.525 clusters = 33.548.800 bytes (32.762.5 KB)
 *
 *              FAT12 maximum : 64 sectors per cluster ×       4.084 clusters = 133.824.512 bytes (≈ 127 MB)
 *              FAT16 maximum : 64 sectors per cluster ×      65.524 clusters = 2.147.090.432 bytes (≈2.047 MB)
 *              FAT32 maximum :  8 sectors per cluster × 268.435.444 clusters = 1.099.511.578.624 bytes (≈1.024 GB)
 *              FAT32 maximum : 16 sectors per cluster × 268.173.557 clusters = 2.196.877.778.944 bytes (≈2.046 GB) ->bigger size
 *          \endcode
 *
 *          On creating the filesystem can be decided, if format the partition first or only be overwrite the partitions data.
 *          Set one of the following fag:
 *          \code
 *              FS_PARTITION_OVERWRITE_CREATE - the partition will not be format and only overwrite existing data on partition
 *              FS_PARTITION_FORMAT_CREATE    - the partition is formatted first and after then is created
 *          \endcode
 *
 *          If the fs_format parameters is 0, than will be used the default value.
 *          \code
 *              Property  | default value
 *              ----------|---------------------------
 *              fs_format | FS_PARTITION_FORMAT_CREATE
 *          \endcode
 *
 *          This function is not allowed if the partition has been configured with write protection.
 *
 * @param   handle:                 partition handle
 * @param   format_option:          format option
 * @param   partition_size:         size of partition
 * @param   sector_size:            size of sector
 * @param   cluster_size:           size of cluster
 * @param   offset_sectors_count:   reserved sectors count
 * @param   file_dir_entry_count:   count file/dir entries
 * @param   fs_header_sector_count: count header sectors
 * @param   fs_format:              format or overwriting partition
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                   if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER   if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_FS_NO_DISK          if partition not exists
 * @retval  SEOS_ERROR_INVALID_HANDLE      if partition handle is not valid
 * @retval  SEOS_ERROR_FS_OPERATION_DENIED if partition was opened in read-only mode
 * @retval  SEOS_ERROR_FS_FORMAT_FS        if partition can not be formated
 * @retval  SEOS_ERROR_FS_CREATE_FS        if fail to create filesystem
 * @retval  SEOS_ERROR_FS_OPEN             if partition is not open
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE   if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_create(
    hPartition_t handle,
    uint8_t format_option,
    uint64_t partition_size,
    uint16_t sector_size,
    uint32_t cluster_size,
    uint16_t offset_sectors_count,
    uint16_t file_dir_entry_count,
    uint32_t fs_header_sector_count,
    int fs_format)
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
 * @details %OS_FilesystemApi_mount defines the API interface name to open a partition and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function provides to mount a partition.
 *
 * @param   handle: partition handle
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                      if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER      if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_FS_NO_DISK             if partition not exists
 * @retval  SEOS_ERROR_FS_PARTITION_NOT_READY if partition is not ready for use
 * @retval  SEOS_ERROR_FS_PARTITION_READ      if disk read operation has an error
 * @retval  SEOS_ERROR_FS_INVALID_FILESYSTEM  if no filesystem
 * @retval  SEOS_ERROR_INVALID_HANDLE         if partition handle is not valid
 * @retval  SEOS_ERROR_FS_STRUCTURE           if internal structure can not resolve
 * @retval  SEOS_ERROR_FS_MOUNT               if fail to mount a partition
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE      if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_mount(
    hPartition_t handle)
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
 * @details %OS_FilesystemApi_unmount defines the API interface name to open a partition and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function provides to unmount a partition.
 *
 * @param   handle: partition handle
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                 if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_INVALID_HANDLE    if partition handle is not valid
 * @retval  SEOS_ERROR_FS_STRUCTURE      if internal structure can not resolve
 * @retval  SEOS_ERROR_FS_NO_DISK        if partition not exists
 * @retval  SEOS_ERROR_FS_UNMOUNT        if fail to unmount a partition
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_unmount(
    hPartition_t handle)
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
 * @details %OS_FilesystemApi_wipe defines the API interface name to wipe a partition and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function delete all data by writing 0 to all blocks.
 *          Formatting is not allowed if the partition has been configured with write protection.
 *
 * @param   handle: partition handle
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                   if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER   if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_INVALID_HANDLE      if partition handle is not valid
 * @retval  SEOS_ERROR_FS_STRUCTURE        if internal structure can not resolve
 * @retval  SEOS_ERROR_FS_NO_DISK          if partition not exists
 * @retval  SEOS_ERROR_FS_OPERATION_DENIED if partition was opened in read-only mode
 * @retval  SEOS_ERROR_FS_FORMAT_FS        if partition can not be formated
 * @retval  SEOS_ERROR_FS_OPEN             if partition is not open
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE   if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_wipe(
    hPartition_t handle)
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
 * @details %OS_FilesystemApi_close defines the API interface name to close a partition and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function deregistered internal objects and the partition will be closed. Implicit all open file handle handle will be closed.
 *          The return value is affected by the set build parameter.
 *
 * @param   handle: partition handle
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                 if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_INVALID_HANDLE    if partition handle is not valid
 * @retval  SEOS_ERROR_FS_STRUCTURE      if fail to resolve internal structure
 * @retval  SEOS_ERROR_FS_NO_DISK        if partition not exists
 * @retval  SEOS_ERROR_FS_UNMOUNT        if partition can not be unmount
 * @retval  SEOS_ERROR_FS_CLOSE          if partition can not be closed
 * @retval  SEOS_ERROR_FS_DELETE_HANDLE  if internal handle structure can not be deleted
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_close(
    hPartition_t handle)
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
 * @details %OS_FilesystemApi_openFile defines the API interface name to open a file and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function check the partition handle and open a file.\n
 *          \n
 *          When opening a file, information can be provided on how to open the file - e.g. read, write, create, aso.
 *          A file can be opened with different flags:
 *          \code
 *              FA_READ          - only read
 *              FA_WRITE         - only write
 *              FA_OPEN_EXISTING - open, yust if file exists
 *              FA_CREATE_NEW    - create a file
 *              FA_CREATE_ALWAYS - create a file, is a file exists it will be overwrite
 *              FA_OPEN_ALWAYS   - only open
 *              FA_OPEN_APPEND   - open in append mode
 *          \endcode
 *
 * @param   handle: partition handle
 * @param   name:   name of a file
 * @param   flag:   instruction for file opening
 *
 * @return  file handle
 *
 * @retval  pointer to struct if handle is valid and build parameter SEOS_FS_BUILD_AS_LIB is set
 * @retval  NULL pointer      if handle is invalid and build parameter SEOS_FS_BUILD_AS_LIB is set
 * @retval  >= 0              if handle is valid and build parameter SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE or SEOS_FS_BUILD_AS_COMPONENT is set
 * @retval    -1              if handle is invalid and build parameter SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE or SEOS_FS_BUILD_AS_COMPONENT is set
 *
 * @ingroup seos_fs_api
*/
static inline hFile_t
OS_FilesystemApi_openFile(
    hPartition_t handle,
    const char* name,
    int flag)
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
 * @details %OS_FilesystemApi_closeFile defines the API interface name to close a file and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function deregistered internal objects and the file closed.
 *
 * @param   handle: file handle
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                 if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_INVALID_HANDLE    if file handle is not valid
 * @retval  SEOS_ERROR_FS_STRUCTURE      if internal structure can not resolve
 * @retval  SEOS_ERROR_FS_DELETE_HANDLE  if internal handle structure can not be deleted
 * @retval  SEOS_ERROR_FS_NO_DISK        if partition not exists
 * @retval  SEOS_ERROR_FS_LIB            if library throws an error
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_closeFile(
    hFile_t handle)
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
 * @details %OS_FilesystemApi_readFile defines the API interface name to read a file and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function reads data from a file and copy it in given buffer.
 *          The buffer argument is a pointer, which is must be not NULL and is allocated from the caller function.
 *          If this library is build as component, the buffer is limited for OS_FS_DATABUFFER_SIZE bytes.
 *          so the argument len is checked for this limitation size.
 *
 * @param   handle: file handle
 * @param   offset: offset when reading a file
 * @param   len:    count bytes to read
 * @param   buffer: read content into buffer
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                     if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER     if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_INVALID_HANDLE        if file handle is not valid
 * @retval  SEOS_ERROR_FS_STRUCTURE          if internal structure can not resolve
 * @retval  SEOS_ERROR_FS_NO_DISK            if partition not exists
 * @retval  SEOS_ERROR_FS_FILE_NOT_FOUND     if file was not found on partition
 * @retval  SEOS_ERROR_FS_LIB                if library throws an error
 * @retval  SEOS_ERROR_FS_DATABUFFER_OVERLOW if databuffer is too small
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE     if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_readFile(
    hFile_t handle,
    long offset,
    long len,
    void* buffer)
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
 * @details %OS_FilesystemApi_writeFile defines the API interface name to write a file and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function writes data from a given buffer in a file.
 *          The buffer argument is a pointer, which is must be not NULL and is allocated from the caller function.
 *          If this library is build as component, the buffer is limited for OS_FS_DATABUFFER_SIZE bytes.
 *          so the argument len is checked for this limitation size.
 *          Writing process is not allowed if the partition has been configured with write protection.
 *
 * @param   handle: file handle
 * @param   offset: offset when writing a file
 * @param   len:    count bytes to write
 * @param   buffer: write content into buffer
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                     if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER     if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_INVALID_HANDLE        if file handle is not valid
 * @retval  SEOS_ERROR_FS_STRUCTURE          if internal structure can not resolve
 * @retval  SEOS_ERROR_FS_NO_DISK            if partition not exists
 * @retval  SEOS_ERROR_FS_FILE_NOT_FOUND     if file was not found on partition
 * @retval  SEOS_ERROR_FS_LIB                if library throws an error
 * @retval  SEOS_ERROR_FS_OPERATION_DENIED   if partition was opened in read-only mode
 * @retval  SEOS_ERROR_FS_DATABUFFER_OVERLOW if databuffer is too small
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE     if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_writeFile(
    hFile_t handle,
    long offset,
    long len,
    void* buffer)
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
 * @details %OS_FilesystemApi_deleteFile defines the API interface name to delete a file and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function deletes a file.
 *          Delete process is not allowed if the partition has been configured with write protection.
 *
 * @param   handle: partition handle
 * @param   name:   name of a file
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                   if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER   if any of the required parameters is missing or wrong
 * @retval  SEOS_ERROR_INVALID_HANDLE      if partition handle is not valid
 * @retval  SEOS_ERROR_FS_NO_DISK          if partition not exists
 * @retval  SEOS_ERROR_FS_OPERATION_DENIED if partition was opened in read-only mode
 * @retval  SEOS_ERROR_FS_LIB              if library throws an error
 * @retval  SEOS_ERROR_FS_RESOLVE_HANDLE   if failed to resolve handle
 *
 * @ingroup seos_fs_api
*/
static inline seos_err_t
OS_FilesystemApi_deleteFile(
    hPartition_t handle,
    const char* name)
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
 * @details %OS_FilesystemApi_getSizeOfFile defines the API interface name to get the size from a file and calls the underlying function from the filesystem library
 *          according to the set build parameter.
 *          This function returns the size of a file.
 *
 * @param   handle: partition handle
 * @param   name:   name of a file
 *
 * @return  size of a file
 *
 * @retval  size if file is exists and get the size from file
 * @retval  -1   if an error has occurred
 *
 * @ingroup seos_fs_api
*/
static inline int64_t
OS_FilesystemApi_getSizeOfFile(
    hPartition_t handle,
    const char* name)
{
#if defined(SEOS_FS_BUILD_AS_COMPONENT)
    return api_fs_component_file_getSize(handle, name);
#elif defined (SEOS_FS_BUILD_AS_LIB)
    return api_fs_file_getSize(&handle, name);
#elif defined(SEOS_FS_BUILD_AS_LIB_BASIC_HANDLE)
    return api_fs_resolve_layer_file_getSize(handle, name);
#endif
}
