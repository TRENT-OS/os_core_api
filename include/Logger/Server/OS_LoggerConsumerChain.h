/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Consumer chain implementation
 */
#pragma once

/**
 * @defgroup    OS_LoggerConsumerChain Consumer chain implementation
 *
 * @brief       This layer provides to manage consumer log objects.
 * @details     All log consumer objects, which will be getting logged, has to
 *              append in a logger chain.
 *              This layer provides functions to append or remove consumer log
 *              objects into or from this consumer chain.
 *
 *              Using the "get sender id" function, a log consumer, i.e. a log
 *              client, can be clearly identified when a log message arrives.
 *              This implementation works as a singleton. That means that only
 *              one consumer log chain can exist at a time.
 *              \n \n
 *              This layer is implemented as singleton.
 *
 * @ingroup     OS_LoggerServer
*/
#include "Logger/Server/OS_LoggerConsumer.h"
#include "Logger/Server/OS_LoggerListT.h"
#include <stdbool.h>


/**
 * @details OS_LoggerConsumerChain_Handle_t defines the class datatype.
 *
 * @ingroup OS_LoggerConsumerChain
*/
typedef struct OS_LoggerConsumerChain_Handle OS_LoggerConsumerChain_Handle_t;


/**
 * @details OS_LoggerConsumerChain_dtor_t defines the interface for function
 *          pointer to the destructor.
 *
 * @ingroup OS_LoggerConsumerChain
*/
typedef void
(*OS_LoggerConsumerChain_dtor_t)(void);


/**
 * @details OS_LoggerConsumerChain_append_t defines the interface for function
 *          pointer to append a consumer log object.
 *
 * @param   consumer:   pointer to consumer log object
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerConsumerChain
*/
typedef bool
(*OS_LoggerConsumerChain_append_t)(OS_LoggerConsumer_Handle_t* consumer);


/**
 * @details OS_LoggerConsumerChain_remove_t defines the interface for function
 *          pointer to remove a consumer log object.
 *
 * @param   consumer:   pointer to consumer log object
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerConsumerChain
*/
typedef bool
(*OS_LoggerConsumerChain_remove_t)(OS_LoggerConsumer_Handle_t* consumer);


/**
 * @details OS_LoggerConsumerChain_getSender_t defines the interface for
 *          function pointer to determine a calling consumer log object by
 *          sender id.
 *
 * @return  pointer to consumer log object
 *
 * @ingroup OS_LoggerConsumerChain
*/
typedef OS_LoggerConsumer_Handle_t*
(*OS_LoggerConsumerChain_getSender_t)(void);


/**
 * @details OS_LoggerConsumerChain_vtable_t contain the member functions to his
 *          class.
 *
 * @ingroup OS_LoggerConsumerChain
*/
typedef struct
{
    OS_LoggerConsumerChain_dtor_t      dtor;
    OS_LoggerConsumerChain_append_t    append;
    OS_LoggerConsumerChain_remove_t    remove;
    OS_LoggerConsumerChain_getSender_t get_sender;
} OS_LoggerConsumerChain_vtable_t;


/**
 * @details OS_LoggerConsumerChain_node_t contain informations about the
 *          consumer log object and is realized as singleton.
 *
 * @ingroup OS_LoggerConsumerChain
*/
typedef struct
{
    void* first; /**< pointer to the first element */
} OS_LoggerConsumerChain_node_t;


/**
 * @details OS_LoggerConsumerChain_Handle contain the vtable to his class.
 *
 * @ingroup OS_LoggerConsumerChain
*/
struct OS_LoggerConsumerChain_Handle
{
    OS_LoggerListT_t_Handle_t              listT;
    OS_LoggerConsumerChain_node_t          node;
    const OS_LoggerConsumerChain_vtable_t* vtable;
};


/**
 * @details %OS_LoggerConsumerChain_getInstance is a getter to instantiate this
 *          object.
 *
 *          This is an singleton implementation. It will internally create a
 *          consumer chain object.
 *
 *          If the object has been successfully created, the pointer to this
 *          object will returned, in the other case this functions returns a
 *          NULL pointer.
 *
 * @return  pointer to OS_LoggerConsumerChain_Handle_t object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup OS_LoggerConsumerChain
*/
OS_LoggerConsumerChain_Handle_t*
OS_LoggerConsumerChain_getInstance(void);


/**
 * @details %OS_LoggerConsumerChain_dtor is the destructor.
 *
 * @ingroup OS_LoggerConsumerChain
*/
void
OS_LoggerConsumerChain_dtor(void);


/**
 * @details %OS_LoggerConsumerChain_append provides to append a consumer log
 *          object.
 *
 * @param   consumer:   pointer to consumer log object
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if an error has been occurred
 *
 * @ingroup OS_LoggerConsumerChain
*/
bool
OS_LoggerConsumerChain_append(OS_LoggerConsumer_Handle_t* consumer);


/**
 * @details %OS_LoggerConsumerChain_remove provides to remove a consumer log
 *          object.
 *
 * @param   consumer:   pointer to consumer log object
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if an error has been occurred
 *
 * @ingroup OS_LoggerConsumerChain
*/
bool
OS_LoggerConsumerChain_remove(OS_LoggerConsumer_Handle_t* consumer);


/**
 * @details %OS_LoggerConsumerChain_getSender provides to determine a calling
 *          consumer log object by sender id.
 *
 * @return  pointer to log consumer object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup OS_LoggerConsumerChain
*/
OS_LoggerConsumer_Handle_t*
OS_LoggerConsumerChain_getSender(void);
