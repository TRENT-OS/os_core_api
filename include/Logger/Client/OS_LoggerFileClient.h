/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log file client implementation.
 */
#pragma once

/**
 * @defgroup    OS_LoggerFileClient Log file client implementation
 *
 * @brief       This layer provides the implementation of log file client
 *              functions.
 *
 * @details     This is a client library, so this library doesn't know anything
 *              about the file handling on the log server i.e. it doesn't
 *              matter, which filesystem and so on is installed on server site.
 *              \n \n
 *              This layer is a consumer layer and provides client site
 *              functionality to read content from a log file or a complete log
 *              file.
 *
 * @ingroup     OS_LoggerClient
*/
#include "Logger/Client/OS_LoggerFileClientCallback.h"
#include <stdint.h>
#include <stdbool.h>


/**
 * @details OS_LoggerFileClient_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerFileClient
*/
typedef struct OS_LoggerFileClient_Handle OS_LoggerFileClient_Handle_t;


/**
 * @details OS_LoggerFileClient_dtor_t defines the interface for function
 *          pointer to destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerFileClient
*/
typedef void
(*OS_LoggerFileClient_dtor_t)(OS_LoggerFileClient_Handle_t* self);


/**
 * @details OS_LoggerFileClient_read_t defines the interface for function
 *          pointer to read a log file from log server.
 *
 *          The parameter "len" is the buffer size, i.e. how many bytes should
 *          be read from the log file and written to the buffer when the
 *          function is called.
 *
 *          This size is particularly important because the max. buffer size of
 *          CamkES is 4096 bytes.
 *
 * @param   self:       pointer to the class
 * @param   filename:   name of log file
 * @param   offset:     offset in log file
 * @param   len:        length of buffer
 *
 * @return  an error code
 *
 * @ingroup OS_LoggerFileClient
*/
typedef bool
(*OS_LoggerFileClient_read_t)(
    OS_LoggerFileClient_Handle_t* self,
    const char* filename,
    uint64_t offset,
    uint64_t len);


/**
 * @details OS_LoggerFileClient_vtable_t contain the member functions to his
 *          class.
 *
 * @ingroup OS_LoggerFileClient
*/
typedef struct
{
    OS_LoggerFileClient_dtor_t dtor;
    OS_LoggerFileClient_read_t read_log_file;
} OS_LoggerFileClient_vtable_t;


/**
 * @details OS_LoggerFileClient_Handle contain the vtable to his class.
 *
 * @ingroup OS_LoggerFileClient
*/
struct OS_LoggerFileClient_Handle
{
    void* src_buf;
    void* dest_buf;
    OS_LoggerFileClientCallback_Handle_t* callback_vtable;
    const OS_LoggerFileClient_vtable_t* vtable;
};


/**
 * @details %OS_LoggerFileClient_ctor is the constructor. \n
 *          All layer functions are interchangeable due to their special
 *          implementation.
 *
 * @param   self:                       pointer to the class
 * @param   src_buf:                    pointer to source data buffer
 * @param   dest_buf:                   pointer to destination data buffer
 * @param   log_file_client_callback:   layer for callback vtable
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup OS_LoggerFileClient
*/
bool
OS_LoggerFileClient_ctor(
    OS_LoggerFileClient_Handle_t* self,
    void* src_buf,
    void* dest_buf,
    OS_LoggerFileClientCallback_Handle_t* log_file_client_callback);


/**
 * @details %OS_LoggerFileClient_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerFileClient
*/
void
OS_LoggerFileClient_dtor(OS_LoggerFileClient_Handle_t* self);


/**
 * @details %OS_LoggerFileClient_read provides to read a log file from log
 *          server.
 *
 * @param   self:       pointer to the class
 * @param   filename:   name of log file
 * @param   offset:     offset in log file
 * @param   len:        buffer size, i.e. how many bytes should be read from the
 *                      log file and written to the buffer when the function is
 *                      called. This size is particularly important because the
 *                      max. buffer size of camkes is 4096 bytes.
 *
 * @return  an error code
 *
 * @retval  true,  if all allright
 * @retval  false, if an error has been occurred
 *
 * @ingroup OS_LoggerFileClient
*/
bool
OS_LoggerFileClient_read(
    OS_LoggerFileClient_Handle_t* self,
    const char* filename,
    uint64_t offset,
    uint64_t len);

