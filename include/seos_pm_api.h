/*
   * Provided SEOS partition manager library.
   *
   * Translates SEOS partition manager library function with different prefixes, because of different usages, to API function name uniformly.
   *
   *  Copyright (C) 2019, Hensoldt Cyber GmbH
*/

/**
 * @defgroup    seos_pm SEOS partition manager library
 *
 * @brief       Provided SEOS partition manager.
 * @details     Include specific partition manager headers from SEOS partition manager library.\n
 *              The partition manager library provides the functions for manage partitions. A disk is logically divided into partitions.
 *              The logical partitions are delimited by start and end addresses. This API provide functions to subdivide this disk into logical partitions and to access the memory
 *              area of the disk. The reading and writing operations are done block by frame on a partition. For this purpose, there is a block i/o device.
 *              This block i/o device is not realized in this library but uses the interface of it. \n
 *              \n
 *              The partition manager can be done in two different ways. Use as a library or use as a separate component. \n
 *              If decided for use the partition manager as a seperate component, note that a interface file and a CAmkES component file must created and configured.
 *              Additional must be create an entry for component in the CMakeLists.txt. \n
 *              \n
 *              <b> Creating partition manager interface file: </b> \n
 *              The names for the interface functions can be found in seos_pm_api.h. Make sure that the interface file includes the SEOS partition manager datatypes.
 *              \code
 *                  procedure <INTERFACE_NAME> {
 *                      include "<PATH_NAME>/seos_pm_datatypes.h";
 *
 *                      seos_err_t partition_manager_open(uint8_t partition_id);
 *                      seos_err_t partition_manager_read(uint8_t partition_id, uint64_t offset, uint64_t len, dataport_ptr_t buffer);
 *                      seos_err_t partition_manager_write(uint8_t partition_id, uint64_t offset, uint64_t len, dataport_ptr_t buffer);
 *                      seos_err_t partition_manager_close(uint8_t partition_id);
 *
 *                      seos_err_t partition_manager_get_info_disk(out pm_disk_data_t info_disk);
 *                      seos_err_t partition_manager_get_info_partition(uint8_t partition_id, out pm_partition_data_t info_partition);
 *                  };
 *              \endcode
 *
 *              <b> CAmkES configuration file: </b> \n
 *              Do not touch the partition manager library implementation, so configure the prefix in CAmkES configuration file by providing partition manager interface "api_pm_component".
 *              \code
 *                  component <COMPONENT_NAME> {
 *                      provides <INTERFACE_NAME> api_pm_component;
 *                      # ...
 *                  }
 *              \endcode
 *
 *              <b> CMakeLists.txt file: </b> \n
 *              The partition manager can use as library or as dedicated component. For each build design exists his own define. In order to build one of the variants,
 *              the corresponding define must be specified in the CMakeLists.txt file. There is always only one define variable possible at the same time.
 *              \code
 *                  Define                                    | Description
 *                  ------------------------------------------|---------------------------------------
 *                  SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT | use the partition manager as component
 *                  SEOS_PARTITION_MANAGER_BUILD_AS_LIB       | use the partition manager as library
 *              \endcode
 *
 *              The library is required to have a set include path variable to the base directories of the partition manager library itself.
 *              The set includes in this library are always defined with:
 *              \code
 *                   #include "<FILE_NAME>"
 *              \endcode
 *
 *              <b> Configuration file: </b> \n
 *              The partition manager can be influenced in creating in all of the following properties.
 *              Make a configuration file (\link seos_pm_conf \endlink) and set the required defines.
 *              If no set this parameters the default values will be used.
 *              \code
 *                  Property                        | Configuration define           | default value
 *                  --------------------------------|--------------------------------|--------------------
 *                  size of disk                    | GET_PROPERTY_DISK_SIZE         | - empty - / not set
 *                  block size                      | GET_PROPERTY_BLOCK_SIZE        | 512 bytes
 *                  name of specific partiton       | GET_PROPERTY_PARTITION_NAME(x) | - empty - / not set
 *                  size of specific partition      | GET_PROPERTY_PARTITION_SIZE(x) | - empty - / not set
 *              \endcode
 *
 *              The include path for the util functions must be inside quotation marks. Don't forget to set the relative include path in the CMakeLists.txt inside the include field for this functionalities. \n
 *              \n
 *              If partition manager build as component, there are additional configuration parameters. So the partition manager implementation files needn't be touched.
 *              \code
 *                  Property                      | Configuration define                 | default value
 *                  ------------------------------|--------------------------------------|--------------------
 *                  name of buffer dataport_ptr_t | GET_PROPERTY_PM_DATAPORT_BUFFER      | - empty - / not set
 *                  size of buffer dataport_ptr_t | DATABUFFER_SIZE                      | - empty - / not set
 *              \endcode
 *
 *              The property names which are empty or not set must be set with an value, because they are necessary for internally usage.
 *              If these parameters are not set, by compiling throws compiler an error message.
 *
 *              Inside the CAmkES component configuration file can be define the properties about a disk and their partitions. Set the following structs:
 *              \code
 *                  struct disk_info {
 *                      int disk_size;
 *                      int block_size_default;
 *                  }
 *
 *                  struct partition_config {
 *                      string partition_name;
 *                      int partition_size;
 *                  }
 *
 *                  struct partition_cat {
 *                      partition_config partition[];
 *                  }
 *              \endcode
 *
 *              Aftwards set the attribute to the component like:
 *              \code
 *                  component <COMPONENT_NAME> {
 *                      # ...
 *
 *                      attribute disk_info disk_information;
 *                      attribute partition_cat partition_conf;
 *                  }
 *              \endcode
 *
 *              Then set the content of the attributes in the main.camkes file inside their component:
 *              \code
 *                  assembly {
 *                      composition {
 *                          # ...
 *                      }
 *                      configuration{
 *                          # ...
 *
 *                          <COMPONENT_NAME>.disk_information = { "disk_size" : <VALUE>, "block_size_default" : <VALUE> };
 *                          <COMPONENT_NAME>.partition_conf = { "partition" : [
 *                                                              { "partition_name" : <VALUE>, "partition_size" : <VALUE> },
 *                                                              { "partition_name" : <VALUE>, "partition_size" : <VALUE> },
 *                                                              { "partition_name" : <VALUE>, "partition_size" : <VALUE> }
 *                                                            ] };
 *                      }
 *                  }
 *              \endcode
 *
 *              <b> Dependencies: </b>
 *              The partition manager library requires the proxy NVM library for reading and writing data and the chanmux library for store data in a extern file.
 *
 *              <b> How to use this library currently? </b> \n
 *              In an application is required the header file "seos_pm_api.h".
 *              The seos_pm_api.h provides partition manager datatypes, includes configuration and API functions. \n
 *              \n
 *              If the partition manager library is implemented as an dedicated component, the initialization can be performed in the "\_\_init()" function.
 *              Create a file in the partition manager component and implement the "\_\_init()" function. The content in this function will be the same like the initialization
 *              in the application itself. \n
 *              \n
 *              Due to the possibility of using the partition manager as a library or as a stand alone component, when using the parition manager as an dedicated
 *              component it is already executed in its "\_\_init()" function.
 *              The "partition_manager_init" function uses deep level read and write function with NVM-API. So create first an NVM object and passes to the "init" function.
 *              \code
 *                  #include "<PATH>/seos_pm_api.h"
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
 *              Look at the example code:
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
*/

/**
 * @defgroup    seos_pm_api SEOS partition manager API translation layer
 *
 * @brief       Translates partition manager library function with different prefixes, because of different usages, to API function name uniformly.
 * @details     This layer implements partition manager API interface. There will be only translate the internaly partition manager library functions to the API interface names.
 *              Depend on the configured build paramters will be call his respective implemention function. \n
 *              There are two different ways to use this API interface:
 *              \code
 *                  Define                                    | Description
 *                  ------------------------------------------|---------------------------------------
 *                  SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT | use the partition manager as component
 *                  SEOS_PARTITION_MANAGER_BUILD_AS_LIB       | use the partition manager as library
 *              \endcode
 *
 *              By using the component-based design (SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT) the functions "partition_manager_read" and "partition_manager_write"
 *              are using CAmkES dataports and databuffer. The databuffer has in the current configuration DATABUFFER_SIZE bytes. \n
 *              If this size is exceeded a corresponding error code will be returned.
 *              The cast from the passed databuffer to a CAmkES dataport is covered by her functions itself.
 *              This does not have to be done from outside.
 *
 * @ingroup     seos_pm
*/


#pragma once

#if defined(SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT)

#include "seos_pm_conf.h"
#include <string.h>

#include <camkes.h>


#ifndef DATABUFFER_SIZE
#error DATABUFFER_SIZE is not defined
#endif


#ifndef GET_PROPERTY_PM_DATAPORT_BUFFER
#error GET_PROPERTY_PM_DATAPORT_BUFFER is not defined
#endif


/**
 * @brief   buffer_send is a CAmkES data buffer for send data over component interface.
 *
 * @ingroup seos_pm_api
*/
dataport_ptr_t buffer_send;

/**
 * @brief   buffer_receive is a CAmkES data buffer for receive data from component interface.
 *
 * @ingroup seos_pm_api
*/
dataport_ptr_t buffer_receive;

#elif defined (SEOS_PARTITION_MANAGER_BUILD_AS_LIB)

#include "api_pm.h"

#endif

#include "SeosError.h"
#include "seos_pm_datatypes.h"  // include path to partition_manager must be set in cmakelists.txt
#include "seos_pm_conf.h"       // include path to partition_manager must be set in cmakelists.txt

#if defined (SEOS_PARTITION_MANAGER_BUILD_AS_LIB)
/**
 * @details %partition_manager_init defines the API interface name to initialize the partiton manager and calls the underlying function from the partition manager library
 *          according to the set build parameter.
 *          This function initialize and create internal objects. It register the block i/o device and initialize it.
 *          Further it will be run an self test, which is checked the configuration. It checked the following details:
 *          \code
 *              -> if the size of all merged partitions are larger than the size of the disk itself
 *              -> if start and end of position of each partition are correctly defined
 *              -> if the partition id are correctly defined
 *              -> if the block size is set
 *          \endcode
 *
 * @param   nvm_object: nvm object
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                           if all right
 * @retval  SEOS_ERROR_PM_INTERNAL_OBJECT          if invalid object internally
 * @retval  SEOS_ERROR_PM_NO_PARTITIONS            if no partition exists
 * @retval  SEOS_ERROR_PM_BLOCK_SIZE               if block size is undefined
 * @retval  SEOS_ERROR_PM_REGISTER_INTERNAL_OBJECT if fail to register internal object
 * @retval  SEOS_ERROR_INVALID_PARAMETER           if argument is missing or wrong value
 * @retval  SEOS_ERROR_PM_DISK_SIZE                if real disk size is smaller than configured
 * @retval  SEOS_ERROR_PM_POINTER_POS              if pointer position of start and end pointer do not match the size of the partition
 * @retval  SEOS_ERROR_PM_POINTER_START_POS        if wrong pointer to start position
 * @retval  SEOS_ERROR_PM_POINTER_END_POS          if wrong pointer to end position
 * @retval  SEOS_ERROR_PM_PARTITION_ID             if wrong partition id or partition id don't exists
 *
 * @ingroup seos_pm_api
*/
static __attribute__((unused)) seos_err_t
partition_manager_init(
    void* nvm_object)
{
    return api_pm_partition_manager_init(nvm_object);
}
#endif // SEOS_PARTITION_MANAGER_BUILD_AS_LIB


/**
 * @details %partition_manager_open open a physical partition.
 *
 * @param   partition_id:   identifier of partition
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                  if all right
 * @retval  SEOS_ERROR_PM_PARTITION_ID    if wrong partition id or partition id don't exists
 * @retval  SEOS_ERROR_PM_INTERNAL_OBJECT if invalid object internally
 * @retval  SEOS_ERROR_PM_OPEN            if no open operation has be done
 *
 * @ingroup seos_pm_api
*/
static __attribute__((unused)) seos_err_t
partition_manager_open(
    uint8_t partition_id)
{
#if defined(SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    return api_pm_component_partition_manager_open(partition_id);
#elif defined (SEOS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_open(partition_id);
#endif
}


/**
 * @details %partition_manager_read defines the API interface name to read data and calls the underlying function from the partition manager library
 *          according to the set build parameter.
 *          This function reads data and copy it in given buffer. It determines from the raw data of the hard disk the start pointer to your partition and
 *          if necessary, calculates the offset on the start pointer of the partition. at the same time the range of the offset is checked with the limits of the partition.
 *          The buffer argument is a pointer, which is must be not NULL and is allocated from the caller function.
 *          The buffer is limited for DATABUFFER_SIZE bytes, so the argument len is checked for this limitation size.
 *
 * @param   partition_id:   identifier of partition
 * @param   offset:         offset when reading from partition
 * @param   len:            count bytes to read
 * @param   buffer:         read content into buffer
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                    if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER    if argument is missing or wrong value
 * @retval  SEOS_ERROR_PM_PARTITION_ID      if wrong partition id or partition id don't exists
 * @retval  SEOS_ERROR_PM_INTERNAL_OBJECT   if invalid object internally
 * @retval  SEOS_ERROR_PM_OFFSET            if offset is out of range
 * @retval  SEOS_ERROR_PM_READ              if no read operation has be done
 *
 * @ingroup seos_pm_api
*/
static __attribute__((unused)) seos_err_t
partition_manager_read(
    uint8_t partition_id,
    uint64_t offset,
    uint64_t len,
    void* buffer)
{
#if defined(SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    seos_err_t retval = SEOS_SUCCESS;
    const void* buf = (void*)0;
    pm_partition_data_t pm_partition_data;

    // checks databuffer length
    if (DATABUFFER_SIZE < len)
    {
        return SEOS_ERROR_PM_DATABUFFER_OVERLOW;
    }

    // Cast/Wrap pointer to dataport pointer
    buffer_receive = dataport_wrap_ptr(GET_PROPERTY_PM_DATAPORT_BUFFER);

    // Call partition manager API function
    retval = api_pm_component_partition_manager_read(partition_id, offset, len,
                                                     buffer_receive);
    if (retval != SEOS_SUCCESS)
    {
        return retval;
    }

    buf = dataport_unwrap_ptr(buffer_receive);

    retval = api_pm_component_partition_manager_get_info_partition(partition_id,
             &pm_partition_data);
    if (retval != SEOS_SUCCESS)
    {
        return retval;
    }

    // copy data from databuffer into buffer
    memcpy(buffer, buf, (len * (size_t)pm_partition_data.block_size));

    return retval;
#elif defined (SEOS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_read(partition_id, offset, len, buffer);
#endif
}


/**
 * @details %partition_manager_write defines the API interface name to write data and calls the underlying function from the partition manager library
 *          according to the set build parameter.
 *          This function writes data from a given buffer to partition. It determines from the raw data of the hard disk the start pointer to your partition and
 *          if necessary, calculates the offset on the start pointer of the partition. at the same time the range of the offset is checked with the limits of the partition.
 *          The buffer argument is a pointer, which is must be not NULL and is allocated from the caller function.
 *          The buffer is limited for DATABUFFER_SIZE bytes, so the argument len is checked for this limitation size.
 *
 * @param   partition_id:   identifier of partition
 * @param   offset:         offset when writing to partition
 * @param   len:            count bytes to write
 * @param   buffer:         write content into buffer
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                    if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER    if argument is missing or wrong value
 * @retval  SEOS_ERROR_PM_PARTITION_ID      if wrong partition id or partition id don't exists
 * @retval  SEOS_ERROR_PM_INTERNAL_OBJECT   if invalid object internally
 * @retval  SEOS_ERROR_PM_OFFSET            if offset is out of range
 * @retval  SEOS_ERROR_PM_WRITE             if no write operation has be done
 *
 * @ingroup seos_pm_api
*/
static __attribute__((unused)) seos_err_t
partition_manager_write(
    uint8_t partition_id,
    uint64_t offset,
    uint64_t len,
    const void* buffer)
{
#if defined(SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    pm_partition_data_t pm_partition_data;
    seos_err_t retval = SEOS_SUCCESS;

    // checks databuffer length
    if (DATABUFFER_SIZE < len)
    {
        return SEOS_ERROR_PM_DATABUFFER_OVERLOW;
    }

    retval = api_pm_component_partition_manager_get_info_partition(partition_id,
             &pm_partition_data);
    if (retval != SEOS_SUCCESS)
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
#elif defined (SEOS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_write(partition_id, offset, len, buffer);
#endif
}


/**
 * @details %partition_manager_close close a physical partition.
 *
 * @param   partition_id:   identifier of partition
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                  if all right
 * @retval  SEOS_ERROR_PM_PARTITION_ID    if wrong partition id or partition id don't exists
 * @retval  SEOS_ERROR_PM_INTERNAL_OBJECT if invalid object internally
 * @retval  SEOS_ERROR_PM_CLOSE           if no close operation has be done
 *
 * @ingroup seos_pm_api
*/
static __attribute__((unused)) seos_err_t
partition_manager_close(
    uint8_t partition_id)
{
#if defined(SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    return api_pm_component_partition_manager_close(partition_id);
#elif defined (SEOS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_close(partition_id);
#endif
}


/**
 * @details %partition_manager_get_info_disk defines the API interface name to get disk data and calls the underlying function from the partition manager library
 *          according to the set build parameter.
 *          This function fill the given argument with disk data.
 *
 * @param   info_disk:  disk data (pointer to struct)
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                 if all right
 * @retval  SEOS_ERROR_INVALID_PARAMETER if argument is missing or wrong value
 * @retval  SEOS_ERROR_PM_GET_STRUCT     if fail to get struct
 *
 * @ingroup seos_pm_api
*/
static __attribute__((unused)) seos_err_t
partition_manager_get_info_disk(
    pm_disk_data_t* info_disk)
{
#if defined(SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    return api_pm_component_partition_manager_get_info_disk(info_disk);
#elif defined (SEOS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_get_info_disk(info_disk);
#endif
}


/**
 * @details %partition_manager_get_info_partition defines the API interface name to get partition data and calls the underlying function from the partition manager library
 *          according to the set build parameter.
 *          This function fill the given argument with partition data by partition id.
 *
 * @param   partition_id:   identifier of partition
 * @param   info_partition: partition data (pointer to struct)
 *
 * @return  an error code
 *
 * @retval  SEOS_SUCCESS                 if all right
 * @retval  SEOS_ERROR_PM_PARTITION_ID   if wrong partition id or partition id don't exists
 * @retval  SEOS_ERROR_INVALID_PARAMETER if argument is missing or wrong value
 * @retval  SEOS_ERROR_PM_GET_STRUCT     if fail to get struct
 *
 * @ingroup seos_pm_api
*/
static __attribute__((unused)) seos_err_t
partition_manager_get_info_partition(
    uint8_t partition_id,
    pm_partition_data_t* info_partition)
{
#if defined(SEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT)
    return api_pm_component_partition_manager_get_info_partition(partition_id,
            info_partition);
#elif defined (SEOS_PARTITION_MANAGER_BUILD_AS_LIB)
    return api_pm_partition_manager_get_info_partition(partition_id,
                                                       info_partition);
#endif
}
