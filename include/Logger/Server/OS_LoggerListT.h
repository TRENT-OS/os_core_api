/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief List interface and implementation.
 */
#pragma once


/**
 * @defgroup    OS_LoggerListT List interface and implementation
 *
 * @brief       Helper to the manager list objects.
 * @details     This object provides the interface to deal with lists without
 *              the malloc space.
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_Error.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @details OS_LoggerNodeT_Handle_t contains the node's previous and next
 *          objects.
 *
 * @ingroup OS_LoggerListT
*/
typedef struct OS_LoggerNodeT_Handle
{
    void* prev; /**< (pointer) node to previous object */
    void* next; /**< (pointer) node to next object */
} OS_LoggerNodeT_Handle_t;

/**
 * @details OS_LoggerListT_hasPrevious defines the interface for the function
 *          pointer to check if the list has the previous object.
 *
 * @param   current:    pointer to a list object
 *
 * @return  a status code
 *
 * @ingroup OS_LoggerListT
*/
bool
OS_LoggerListT_hasPrevious(OS_LoggerNodeT_Handle_t* current);

/**
 * @details OS_LoggerListT_hasNext defines the interface for the function
 *          pointer to check if the list has the next object.
 *
 * @param   current:    pointer to a list object
 *
 * @return  a status code
 *
 * @ingroup OS_LoggerListT
*/
bool
OS_LoggerListT_hasNext(OS_LoggerNodeT_Handle_t* current);

/**
 * @details OS_LoggerListT_getPrevious defines the interface for the function
 *          pointer to get the previous object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
void*
OS_LoggerListT_getPrevious(OS_LoggerNodeT_Handle_t* current);

/**
 * @details OS_LoggerListT_getNext defines the interface for the function
 *          pointer to get the next object from the list.
 *
 * @param   current:    pointer to a list object
 *
 * @return  pointer to list object
 *
 * @ingroup OS_LoggerListT
*/
void*
OS_LoggerListT_getNext(OS_LoggerNodeT_Handle_t* current);

/**
 * @details OS_LoggerListT_insert defines the interface for the function
 *          pointer to insert a new object in the list.
 *
 *          The new object will be placed as next object after the current
 *          object.
 *
 * @param   current:    pointer to the list object
 * @param   new_node:   pointer to the list object
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerListT
*/
OS_Error_t
OS_LoggerListT_insert(
    OS_LoggerNodeT_Handle_t* current,
    OS_LoggerNodeT_Handle_t* new_Node);


/**
 * @details OS_LoggerListT_delete defines the interface for the function
 *          pointer to delete an object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerListT
*/
OS_Error_t
OS_LoggerListT_erase(OS_LoggerNodeT_Handle_t* current);

/**
 * @details OS_LoggerListT_getFirst defines the interface for the function
 *          pointer to get the first object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
void*
OS_LoggerListT_getFirst(OS_LoggerNodeT_Handle_t* current);

/**
 * @details OS_LoggerListT_getLast defines the interface for the function
 *          pointer to get the last object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
void*
OS_LoggerListT_getLast(OS_LoggerNodeT_Handle_t* current);

/**
 * @details OS_LoggerListT_isInside defines the interface for function pointer
 *          to check if the current object is inserted in the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  the status code
 *
 * @ingroup OS_LoggerListT
*/
bool
OS_LoggerListT_isInside(OS_LoggerNodeT_Handle_t* current);
