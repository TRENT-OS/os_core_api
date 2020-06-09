/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Partition Manager API
 *
 * Translates TRENTOS-M partition manager library functions with different prefixes,
 * because of different usages, to API function names uniformly.
 *
 */

/**
 * @defgroup    OS_PartitionManager Partition Manager API
 *
 * @brief       Partition Manager API
 *
 * @see os_partition_manager/README.MD for more details.
 *
 */

#pragma once

#if defined(OS_PARTITION_MANAGER_BUILD_AS_COMPONENT)

#include "OS_PartitionManagerConf.h"
#include <string.h>

#include <camkes.h>

#ifndef DATABUFFER_SIZE
#error DATABUFFER_SIZE is not defined
#endif

#ifndef GET_PROPERTY_PM_DATAPORT_BUFFER
#error GET_PROPERTY_PM_DATAPORT_BUFFER is not defined
#endif

/**
 * @brief   The CAmkES data buffer for sending data over the component's
 *          interface.
 *
 * @ingroup OS_PartitionManager
 */
dataport_ptr_t buffer_send;

/**
 * @brief   The CAmkES data buffer for receiving data from the component's
 *          interface.
 *
 * @ingroup OS_PartitionManager
 */
dataport_ptr_t buffer_receive;

#elif defined(OS_PARTITION_MANAGER_BUILD_AS_LIB)

#include "api_pm.h"

#endif

#include "OS_Error.h"

// Include path to partition_manager must be set in CMakeLists.txt
#include "OS_PartitionManagerConf.h"
#include "OS_PartitionManagerDataTypes.h"

#if defined(OS_PARTITION_MANAGER_BUILD_AS_LIB)
/**
 * @brief Defines the API interface name to initialize partition manager.
 *
 * It will initialize the partition manager and calls the underlying function
 * from the partition manager library according to the set build parameter.
 *
 * This function initializes and creates internal objects. It registers the
 * block i/o device and initializes it. Furthermore, it will run a self test,
 * which checks the configuration.
 *
 * It checks the following details:
 *
 * * If the size of all merged partitions is larger than the size of the disk
 *   itself.
 * * If start and end position of each partition are correctly defined.
 * * If the partition id are correctly defined.
 * * If the block size is set.
 *
 * @return  An error code.
 *
 * @retval  OS_SUCCESS                           If all right.
 * @retval  OS_ERROR_PM_INTERNAL_OBJECT          If invalid object internally.
 * @retval  OS_ERROR_PM_NO_PARTITIONS            If no partition exists.
 * @retval  OS_ERROR_PM_BLOCK_SIZE               If block size is undefined.
 * @retval  OS_ERROR_PM_REGISTER_INTERNAL_OBJECT If failed to register the
 *                                               internal object.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER           If argument is missing or
 *                                               wrong value.
 * @retval  OS_ERROR_PM_DISK_SIZE                If real disk size is smaller
 *                                               than the configured.
 * @retval  OS_ERROR_PM_POINTER_POS              If the pointer position of
 *                                               the start and the end
 *                                               pointers do not match the
 *                                               size of the partition.
 *
 * @retval  OS_ERROR_PM_POINTER_START_POS        If the wrong pointer to the
 *                                               start position is given.
 * @retval  OS_ERROR_PM_POINTER_END_POS          If the wrong pointer to the
 *                                               end position is given.
 * @retval  OS_ERROR_PM_PARTITION_ID             If the partition id is wrong
 *                                               or partition id doesn't exist.
 *
 * @ingroup OS_PartitionManager
 */
static
__attribute__((unused))
OS_Error_t
OS_PartitionManager_init(
    void* nvm_object /**< [in,out] The NVM object. */)
{
    return api_pm_partition_manager_init(nvm_object);
}
#endif // OS_PARTITION_MANAGER_BUILD_AS_LIB

/**
 * @brief Opens a physical partition.
 *
 * @return  An error code.
 *
 * @retval  OS_SUCCESS                  If all right.
 * @retval  OS_ERROR_PM_PARTITION_ID    If wrong partition id or partition id
 *                                      doesn't exist.
 * @retval  OS_ERROR_PM_INTERNAL_OBJECT If invalid object internally.
 * @retval  OS_ERROR_PM_INIT            If the partition manager is uninitialized.
 * @retval  OS_ERROR_PM_OPEN            If no open operation has been done.
 *
 * @ingroup OS_PartitionManager
 */
static
__attribute__((unused))
OS_Error_t
OS_PartitionManager_open(
    uint8_t partition_id /**< [in] Partition's identifier. */)
{
#if defined(OS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    return api_pm_component_partition_manager_open(partition_id);
#elif defined(OS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_open(partition_id);
#endif
}

/**
 * @brief   Reads data and calls the underlying function from the partition
 *          manager library accordingly to the set build parameters.
 *
 * This function reads data and copies it into the given buffer.
 *
 * This function determines from the raw data of the hard disk the start pointer
 * to your partition, and if necessary, calculates the offset on the start
 * pointer of the partition. At the same time the range of the offset is checked
 * with the limits of the partition.
 *
 * The buffer argument is a pointer, which must not be NULL and is allocated
 * from the caller function.
 *
 * The buffer is limited to the DATABUFFER_SIZE bytes, so the argument length is
 * checked for this limit.
 *
 * @param   partition_id:   identifier of partition
 * @param   offset:         offset when reading from partition
 * @param   len:            count bytes to read
 * @param   buffer:         read content into buffer
 *
 * @return  an error code
 *
 * @retval  OS_SUCCESS                    if all right
 * @retval  OS_ERROR_INVALID_PARAMETER    if argument is missing or wrong
 *                                        value
 * @retval  OS_ERROR_PM_PARTITION_ID      if wrong partition id or partition
 *                                        id doesn't exist
 * @retval  OS_ERROR_PM_INTERNAL_OBJECT   if invalid object internally
 * @retval  OS_ERROR_PM_INIT              if the partition manager is uninitialized.
 * @retval  OS_ERROR_PM_OFFSET            if offset is out of range
 * @retval  OS_ERROR_PM_READ              if no read operation has been done
 *
 * @ingroup OS_PartitionManager
 */
static
__attribute__((unused))
OS_Error_t
OS_PartitionManager_read(
    uint8_t partition_id,   /**< [in]  Partition's Identifier. */
    uint64_t offset,        /**< [in]  Offset when reading from partition. */
    uint64_t len,           /**< [in]  Bytes count to read. */
    void* buffer            /**< [out] Reads content into this buffer. */)
{
#if defined(OS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    OS_Error_t retval = OS_SUCCESS;
    const void* buf = (void*)0;
    OS_PartitionManagerDataTypes_PartitionData_t pm_partition_data;

    // checks databuffer length
    if (DATABUFFER_SIZE < len)
    {
        return OS_ERROR_PM_DATABUFFER_OVERLOW;
    }

    // Cast/Wrap pointer to dataport pointer
    buffer_receive = dataport_wrap_ptr(GET_PROPERTY_PM_DATAPORT_BUFFER);

    // Call partition manager API function
    retval = api_pm_component_partition_manager_read(partition_id, offset, len,
                                                     buffer_receive);
    if (retval != OS_SUCCESS)
    {
        return retval;
    }

    buf = dataport_unwrap_ptr(buffer_receive);

    retval = api_pm_component_partition_manager_get_info_partition(
                 partition_id, &pm_partition_data);
    if (retval != OS_SUCCESS)
    {
        return retval;
    }

    // copy data from databuffer into buffer
    memcpy(buffer, buf, (len * (size_t)pm_partition_data.block_size));

    return retval;
#elif defined(OS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_read(partition_id, offset, len, buffer);
#endif
}

/**
 * @brief   Writes data and calls the underlying function from the partition
 *          manager library accordingly to the set build parameter.
 *
 * This function determines from the raw data of the hard disk the start pointer
 * to your partition, and if necessary, calculates the offset on the start
 * pointer of the partition. At the same time the range of the offset is checked
 * within the limits of the partition.
 *
 * The buffer argument is a pointer, which must not be NULL and is allocated
 * from the caller function.
 *
 * The buffer is limited to the DATABUFFER_SIZE bytes, so the argument length is
 * checked for this limit.
 *
 * @return  An error code.
 *
 * @retval  OS_SUCCESS                    If all right.
 * @retval  OS_ERROR_INVALID_PARAMETER    If argument is missing or wrong
 *                                        value is given.
 * @retval  OS_ERROR_PM_PARTITION_ID      If wrong partition id or partition
 *                                        id doesn't exist.
 * @retval  OS_ERROR_PM_INTERNAL_OBJECT   If invalid object internally.
 * @retval  OS_ERROR_PM_INIT              If the partition manager is uninitialized.
 * @retval  OS_ERROR_PM_OFFSET            If offset is out of range.
 * @retval  OS_ERROR_PM_WRITE             If no write operation has been done.
 *
 * @ingroup OS_PartitionManager
 */
static
__attribute__((unused))
OS_Error_t
OS_PartitionManager_write(
    uint8_t partition_id,   /**< [in]  Partition's identifier. */
    uint64_t offset,        /**< [in]  Offset when writing to the partition. */
    uint64_t len,           /**< [in]  Bytes count to write. */
    const void* buffer      /**< [in]  Writes content from this buffer. */)
{
#if defined(OS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    OS_PartitionManagerDataTypes_PartitionData_t pm_partition_data;
    OS_Error_t retval = OS_SUCCESS;

    // checks databuffer length
    if (DATABUFFER_SIZE < len)
    {
        return OS_ERROR_PM_DATABUFFER_OVERLOW;
    }

    retval = api_pm_component_partition_manager_get_info_partition(
                 partition_id, &pm_partition_data);
    if (retval != OS_SUCCESS)
    {
        return retval;
    }

    // copy data into databuffer
    memcpy(GET_PROPERTY_PM_DATAPORT_BUFFER, buffer,
           (len * (size_t)pm_partition_data.block_size));

    // Cast/Wrap pointer to dataport pointer
    buffer_send = dataport_wrap_ptr(GET_PROPERTY_PM_DATAPORT_BUFFER);

    // Call partition manager API function
    return api_pm_component_partition_manager_write(partition_id, offset, len,
                                                    buffer_send);
#elif defined(OS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_write(partition_id, offset, len, buffer);
#endif
}

/**
 * @brief Closes a physical partition.
 *
 * It calls the underlying function from the partition manager library
 * accordingly to the set build parameter.
 *
 * @return  An error code.
 *
 * @retval  OS_SUCCESS                  If all right
 * @retval  OS_ERROR_PM_PARTITION_ID    If the wrong partition id or the
 *                                      partition id doesn't exist.
 * @retval  OS_ERROR_PM_INTERNAL_OBJECT If invalid object internally.
 * @retval  OS_ERROR_PM_INIT            If the partition manager is uninitialized.
 * @retval  OS_ERROR_PM_CLOSE           If no close operation has been done.
 *
 * @ingroup OS_PartitionManager
 */
static
__attribute__((unused))
OS_Error_t
OS_PartitionManager_close(
    uint8_t partition_id /**< [in] Partition's identifier. */)
{
#if defined(OS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    return api_pm_component_partition_manager_close(partition_id);
#elif defined(OS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_close(partition_id);
#endif
}

/**
 * @brief Gets disk data.
 *
 * It calls the underlying function from the partition manager library
 * accordingly to the set build parameter.
 *
 * @return  An error code.
 *
 * @retval  OS_SUCCESS                 If all right.
 * @retval  OS_ERROR_INVALID_PARAMETER If argument is missing or wrong value.
 * @retval  OS_ERROR_PM_INIT           If the partition manager is uninitialized.
 * @retval  OS_ERROR_PM_GET_STRUCT     If failed to get a structure.
 *
 * @ingroup OS_PartitionManager
 */
static
__attribute__((unused))
OS_Error_t
OS_PartitionManager_getInfoDisk(
    OS_PartitionManagerDataTypes_DiskData_t*
    info_disk /**< [out] Disk data (pointer to the struct). */)
{
#if defined(OS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    return api_pm_component_partition_manager_get_info_disk(info_disk);
#elif defined(OS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_get_info_disk(info_disk);
#endif
}

/**
 * @brief Gets partition's data based on the given id.
 *
 * It calls the underlying function from the partition manager library
 * accordingly to the set build parameter.
 *
 * @return  An error code.
 *
 * @retval  OS_SUCCESS                 If all right.
 * @retval  OS_ERROR_PM_PARTITION_ID   If the wrong partition id or the
 *                                     partition id doesn't exist.
 * @retval  OS_ERROR_INVALID_PARAMETER If argument is missing or wrong value.
 * @retval  OS_ERROR_PM_INIT           If the partition manager is uninitialized.
 * @retval  OS_ERROR_PM_GET_STRUCT     If failed to get a struct.
 *
 * @ingroup OS_PartitionManager
 */
static
__attribute__((unused))
OS_Error_t
OS_PartitionManager_getInfoPartition(
    uint8_t partition_id,               /**< [in]  Partition's identifier. */
    OS_PartitionManagerDataTypes_PartitionData_t*
    info_partition /**< [out] Partition's data (pointer to the struct) */)
{
#if defined(OS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    return api_pm_component_partition_manager_get_info_partition(partition_id,
            info_partition);
#elif defined(OS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_get_info_partition(partition_id,
                                                       info_partition);
#endif
}
