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
#include <stdint.h>
#include <stdbool.h>


/**
 * @details OS_LoggerListT_t_Handle_t defines the list datatype.
 *
 * @ingroup OS_LoggerListT
*/
typedef struct OS_LoggerListT_Handle OS_LoggerListT_t_Handle_t;
/**
 * @details OS_LoggerNodeT_Handle_t defines the node datatype.
 *
 * @ingroup OS_LoggerListT
*/
typedef struct OS_LoggerNodeT_Handle OS_LoggerNodeT_Handle_t;


/**
 * @details OS_LoggerListT_dtor_t defines the interface for the function pointer
 *          to destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerListT
*/
typedef void
(*OS_LoggerListT_dtor_t)(OS_LoggerListT_t_Handle_t* self);


/**
 * @details OS_LoggerListT_hasPrev_t defines the interface for the function
 *          pointer to check if the list has the previous object.
 *
 * @param   current:    pointer to a list object
 *
 * @return  a status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*OS_LoggerListT_hasPrev_t)(OS_LoggerNodeT_Handle_t* current);


/**
 * @details OS_LoggerListT_hasNext_t defines the interface for the function
 *          pointer to check if the list has the next object.
 *
 * @param   current:    pointer to a list object
 *
 * @return  a status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*OS_LoggerListT_hasNext_t)(OS_LoggerNodeT_Handle_t* current);


/**
 * @details OS_LoggerListT_getPrev_t defines the interface for the function
 *          pointer to get the previous object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
typedef void*
(*OS_LoggerListT_getPrev_t)(OS_LoggerNodeT_Handle_t* current);


/**
 * @details OS_LoggerListT_getNext_t defines the interface for the function
 *          pointer to get the next object from the list.
 *
 * @param   current:    pointer to a list object
 *
 * @return  pointer to list object
 *
 * @ingroup OS_LoggerListT
*/
typedef void*
(*OS_LoggerListT_getNext_t)(OS_LoggerNodeT_Handle_t* current);


/**
 * @details OS_LoggerListT_insert_t defines the interface for the function
 *          pointer to insert a new object in the list.
 *
 *          The new object will be placed as next object after the current
 *          object.
 *
 * @param   current:    pointer to the list object
 * @param   new_node:   pointer to the list object
 *
 * @return  an status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*OS_LoggerListT_insert_t)(
    OS_LoggerNodeT_Handle_t* current,
    OS_LoggerNodeT_Handle_t* new_Node);


/**
 * @details OS_LoggerListT_delete_t defines the interface for the function
 *          pointer to delete an object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  a status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*OS_LoggerListT_delete_t)(OS_LoggerNodeT_Handle_t* current);


/**
 * @details OS_LoggerListT_getFirst_t defines the interface for the function
 *          pointer to get the first object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
typedef void*
(*OS_LoggerListT_getFirst_t)(OS_LoggerNodeT_Handle_t* current);


/**
 * @details OS_LoggerListT_getLast_t defines the interface for the function
 *          pointer to get the last object from the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  pointer to the list object
 *
 * @ingroup OS_LoggerListT
*/
typedef void*
(*OS_LoggerListT_getLast_t)(OS_LoggerNodeT_Handle_t* current);


/**
 * @details OS_LoggerListT_isInside_t defines the interface for function pointer
 *          to check if the current object is inserted in the list.
 *
 * @param   current:    pointer to the list object
 *
 * @return  the status code
 *
 * @ingroup OS_LoggerListT
*/
typedef bool
(*OS_LoggerListT_isInside_t)(OS_LoggerNodeT_Handle_t* current);


/**
 * @details OS_LoggerListT_vtable_t contain the member functions to his class.
 *
 * @ingroup OS_LoggerListT
*/
typedef struct
{
    OS_LoggerListT_dtor_t     dtor;
    OS_LoggerListT_hasPrev_t  has_prev;
    OS_LoggerListT_hasNext_t  has_next;
    OS_LoggerListT_getPrev_t  get_prev;
    OS_LoggerListT_getNext_t  get_next;
    OS_LoggerListT_insert_t   insert;
    OS_LoggerListT_delete_t   delete;
    OS_LoggerListT_getFirst_t get_first;
    OS_LoggerListT_getLast_t  get_last;
    OS_LoggerListT_isInside_t isInside;
} OS_LoggerListT_vtable_t;


/**
 * @details OS_LoggerListT_t_Handle_t contains the class' vtable.
 *
 * @ingroup OS_LoggerListT
*/
struct OS_LoggerListT_Handle
{
    const OS_LoggerListT_vtable_t* vtable; /**< virtual table */
};


/**
 * @details OS_LoggerNodeT_Handle_t contains the node's previous and next
 *          objects.
 *
 * @ingroup OS_LoggerListT
*/
struct OS_LoggerNodeT_Handle
{
    void* prev; /**< (pointer) node to previous object */
    void* next; /**< (pointer) node to next object */
};


/**
 * @details %OS_LoggerListT_ctor is the constructor.
 *
 * @param   self:   pointer to the class
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if pointer is NULL
 *
 * @ingroup OS_LoggerListT
*/
bool
OS_LoggerListT_ctor(OS_LoggerListT_t_Handle_t* self);


/**
 * @details %OS_LoggerListT_dtor is the destructor.
 *
 * @param   self:   pointer to the class
 *
 * @ingroup OS_LoggerListT
*/
void
OS_LoggerListT_dtor(OS_LoggerListT_t_Handle_t* self);
