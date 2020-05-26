/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Log consumer flow control interface.
 */
#pragma once

/**
 * @defgroup    OS_LoggerConsumer Log consumer implementation
 *
 * @brief       This layer provides a implementation of concrete log consumer.
 * @details     On the server side, a log consumer is the object that receives
 *              log messages from a client.
 *
 *              A separate log consumer object must be created for each client.
 *              Each log consumer is given a unique name and an id.
 *              The id is assigned to the client in "*.camkes" main file.
 *              Using this ID, the client can be clearly identified when the log
 *              function is called.
 *              \n \n
 *              A log consumer receives his message by calling an RPC function.
 *              When the log is completely processed, the log consumer sends a
 *              signal that tells the clients that further logs can be received.
 *
 *              The RPC interface is blocked for other clients at this time for
 *              processing the log.
 *              \n \n
 *              Before processing the first message, the log server copy on each
 *              given data buffer his server log level.
 *              \n \n
 *              Only one log consumer can be operated at the same time.
 *              The log consumer object is the central object that offers
 *              interfaces for:
 *              \code
 *                  log filter
 *                  log backend
 *                  log file
 *                  flow control functions
 *              \endcode
 *
 *              The latter is the class \link log_consumer_callback \endlink .
 *
 * @see         log_consumer_callback
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_Error.h"
#include "Logger/Common/OS_LoggerFilter.h"
#include "Logger/Server/OS_LoggerConsumerCallback.h"
#include "Logger/Server/OS_LoggerSubject.h"
#include "Logger/Common/OS_LoggerEntry.h"
#include "Logger/Server/OS_LoggerListT.h"


/**
 * @details OS_LoggerConsumer_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerConsumer
*/
typedef struct OS_LoggerConsumer_Handle OS_LoggerConsumer_Handle_t;


/**
 * @details OS_LoggerConsumer_dtor_t defines the interface for the function
 *          pointer to destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerConsumer
*/
typedef void
(*OS_LoggerConsumer_dtor_t)(OS_LoggerConsumer_Handle_t* self);


/**
 * @details OS_LoggerConsumer_process_t defines the interface for function
 *          pointer to process the logging call.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerConsumer
*/
typedef void
(*OS_LoggerConsumer_process_t)(OS_LoggerConsumer_Handle_t* self);


/**
 * @details OS_LoggerConsumer_emit_t defines the interface for function pointer
 *          to emit a signal, that the interface is ready for new logs.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerConsumer
*/
typedef void
(*OS_LoggerConsumer_emit_t)(OS_LoggerConsumer_Handle_t* self);


/**
 * @details OS_LoggerConsumer_getTimestamp_t defines the interface for function
 *          pointer to get a timestamp from timeserver.
 *
 * @param   self:   pointer to the class
 *
 * @return  timestamp
 *
 * @ingroup OS_LoggerConsumer
*/
typedef uint64_t
(*OS_LoggerConsumer_getTimestamp_t)(OS_LoggerConsumer_Handle_t* self);


/**
 * @details OS_LoggerConsumer_vtable_t contain the member functions to his
 *          class.
 *
 * @ingroup OS_LoggerConsumer
*/
typedef struct
{
    OS_LoggerConsumer_dtor_t         dtor;
    OS_LoggerConsumer_process_t      process;
    OS_LoggerConsumer_getTimestamp_t get_timestamp;
} OS_LoggerConsumer_vtable_t;


/**
 * @details OS_LoggerConsumer_Handle contain the vtable to his class.
 *
 * @ingroup OS_LoggerConsumer
*/
struct OS_LoggerConsumer_Handle
{
    OS_LoggerNodeT_Handle_t           node;
    OS_LoggerEntry_t*                 entry;
    OS_LoggerFilter_Handle_t*         log_filter;
    OS_LoggerSubject_Handle_t*        log_subject;
    void*                             log_file;
    OS_LoggerConsumerCallback_t*      callback_vtable;
    const OS_LoggerConsumer_vtable_t* vtable;
};


/**
 * @details %OS_LoggerConsumer_dtor is the constructor.
 *          \n
 *          The parameters "log_filter", "log_file" and "name" are optional.
 *          "log_file" can be NULL, if no log file is needed.
 *          "log_filter" can be NULL, if no log filter is installed.
 *          \n
 *          All layer functions are interchangeable due to their special
 *          implementation.
 *
 * @param   self:               pointer to the class
 * @param   buffer:             data buffer
 * @param   log_filter:         layer for log filter (optional)
 * @param   callback_vtable:    layer for callback vtable
 * @param   log_subject:        layer for log subject
 * @param   log_file:           layer for file logging (optional)
 * @param   id:                 log consumer id
 * @param   name:               name (optional)
 *
 * @return  An error code.
 *
 * @retval  OS_ERROR_INVALID_PARAMETER - One of the parameters is a NULL
 *                                       pointer.
 * @retval  OS_SUCCESS                 - Operation was successful.
 *
 * @ingroup OS_LoggerConsumer
*/
OS_Error_t
OS_LoggerConsumer_ctor(
    OS_LoggerConsumer_Handle_t* self,
    void* buffer,
    OS_LoggerFilter_Handle_t* log_filter,
    OS_LoggerConsumerCallback_t* callback_vtable,
    OS_LoggerSubject_Handle_t* log_subject,
    void* log_file,
    uint32_t id,
    const char* name);


/**
 * @details %OS_LoggerConsumer_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerConsumer
*/
void
OS_LoggerConsumer_dtor(OS_LoggerConsumer_Handle_t* self);
