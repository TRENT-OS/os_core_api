/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief       Log file interface and implementation.
 *
 * @details     This layer is a server site implementation. It needs a
 *              filesystem partition manager backend.
 *
 *              Functions from this layer are only necessary, if log messages
 *              shall be store in a log file. Dealing with log files needs a
 *              underlying file system.
 *
 *              So, this layer has an dependency to the filesystem backend and
 *              includes the "OS_Filesystem.h" header file.
 *              \n \n
 *              As an interface layer provides this layer functions to create
 *              and read log files on filesystem.
 *
 * @ingroup     OS_LoggerServer
*/
#pragma once

#include "OS_Error.h"
#include "Logger/Common/OS_LoggerSymbols.h"
#include "OS_FileSystem.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details OS_LoggerFile_Handle_t defines the class datatype.
*/
typedef struct OS_LoggerFile_Handle OS_LoggerFile_Handle_t;


/**
 * @details OS_LoggerFile_dtor_t defines the interface for function pointer to
 *          destructor.
 *
 * @param   self:   pointer to the class
*/
typedef void
(*OS_LoggerFile_dtor_t)(OS_LoggerFile_Handle_t* self);


/**
 * @details OS_LoggerFile_create_t defines the interface for function pointer
 *          to create a log file in filesystem backend.
 *
 * @param   self:   pointer to the class
 *
 * @return  An error code.
*/
typedef OS_Error_t
(*OS_LoggerFile_create_t)(OS_LoggerFile_Handle_t* self);


/**
 * @details OS_LoggerFile_read_t defines the interface for function pointer
 *          to read a log file from filesystem backend.
 *
 *          The parameter "len" is the buffer size, i.e. how many bytes should
 *          be read from the log file and written to the buffer when the
 *          function is called.
 *
 *          This size is particularly important because the max. buffer size of
 *          CAmkES is 4096 bytes.
 *
 * @param   self:           pointer to the class
 * @param   filename:       name of log file
 * @param   offset:         offset in log file
 * @param   len:            length of buffer
 * @param   log_file_size:  size of log file in bytes
 *
 * @return  count read bytes
*/
typedef int64_t
(*OS_LoggerFile_read_t)(
    OS_LoggerFile_Handle_t* self,
    const char* filename,
    uint64_t offset,
    uint64_t len,
    int64_t log_file_size);


/**
 * @details OS_LoggerFile_getConsumerByFilename_t defines the interface for
 *          function pointer to get a pointer of log consumer by log filename.
 *
 * @param   filename:   name of log file
 *
 * @return  pointer to log consumer
*/
typedef void*
(*OS_LoggerFile_getConsumerByFilename_t)(const char* filename);


/**
 * @details OS_LoggerFile_vtable_t contain the member functions to his class.
*/
typedef struct
{
    OS_LoggerFile_dtor_t                  dtor;
    OS_LoggerFile_create_t                create_log_file;
    OS_LoggerFile_read_t                  read_log_file;
    OS_LoggerFile_getConsumerByFilename_t get_consumer_by_filename;
} OS_LoggerFile_vtable_t;


/**
 * @details OS_LoggerFile_info_t contain information about log file and
 *          filesystem backend.
*/
typedef struct
{
    //! filesystem handle
    OS_FileSystem_Handle_t  hFs;
    //! filename (including null terminator)
    char                    filename[OS_Logger_MAX_FILENAME_LENGTH + 1];
    //! offset in file
    uint64_t                offset;
    //! size of file
    uint64_t                length;
} OS_LoggerFile_info_t;


/**
 * @details OS_LoggerFile_Handle contain the vtable to his class.
*/
struct OS_LoggerFile_Handle
{
    OS_LoggerFile_info_t          log_file_info;
    const OS_LoggerFile_vtable_t* vtable;
};


/**
 * @details %OS_LoggerFile_ctor is the constructor.
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - filename is NULL or too long.
 * @retval  OS_SUCCESS                 - Operation was successful.
*/
OS_Error_t
OS_LoggerFile_ctor(
    OS_LoggerFile_Handle_t* self, //!< [in] Logger file handle.
    OS_FileSystem_Handle_t hFs,   //!< [in] Handle to the file's file system.
    const char* filename          //!< [in] Logger file name.
);


/**
 * @details %OS_LoggerFile_dtor is the destructor.
 *
 * @param   self:   pointer to the class
*/
void
OS_LoggerFile_dtor(OS_LoggerFile_Handle_t* self);


/**
 * @details %OS_LoggerFile_create provides to create a log file.
 *
 * @param   self:   pointer to the class
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_HANDLE - Opened partition or file handle is
 *                                    invalid.
 * @retval  OS_SUCCESS              - Operation was successful.
*/
OS_Error_t
OS_LoggerFile_create(OS_LoggerFile_Handle_t* self);
