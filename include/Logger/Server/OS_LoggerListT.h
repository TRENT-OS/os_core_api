/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Generic intrusive linked list.
 */
#pragma once

/**
 * @defgroup    OS_LoggerListT Generic intrusive linked list
 *
 * @brief       Generic intrusive linked list.
 *
 * This module provides the interface to deal with the intrusive list that does
 * not require a dynamic allocation.
 *
 * @ingroup     OS_LoggerServer
*/
#include "OS_Error.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Linked list's node.
 *
 * Node object stores the necessary contex information for the linked list to be
 * useful.
 *
 * Typically it is a member of a data structure that is a part of the list i.e.:
 *
 * @code
 * typedef struct Data
 * {
 *      OS_LoggerNodeT_Handle_t node;
 *      int id;
 * } Data_t;
 *
 * Data_t firstElement  = { .id = 1 };
 * Data_t middleElement = { .id = 2 };
 * Data_t lastElement   = { .id = 3 };
 *
 * OS_LoggerListT_insert(firstElement.node, middleElement.node);
 * OS_LoggerListT_insert(middleElement.node, lastElement.node);
 *
 * for(
 *      OS_LoggerNodeT_Handle_t* iterator = firstElement.node;
 *      NULL != iterator;
 *      iterator = (OS_LoggerNodeT_Handle_t*)OS_LoggerListT_get_next(iterator);
 * )
 * {
 *      printf("Acessing element with id: %d\n", ((Data_t*)iterator)->id);
 * }
 * @endcode
 *
 * @note    Typically node will be a first member of the data structure to be
 *          placed in the list, so that the underlaying data can be accessed
 *          with casting.
 * @note    List can contain objects of different types:
 *
 * @code
 *
 * typdef struct Base
 * {
 *      OS_LoggerNodeT_Handle_t node;
 *
 *      enum Type
 *      {
 *          FOO,
 *          BAR
 *      } type;
 * } Base_t;
 *
 * typedef struct Foo
 * {
 *      Base_t base;
 *      char foo;
 * } Foo_t;
 *
 * typedef struct Bar
 * {
 *      Base_t base;
 *      int bar;
 * } Bar_t;
 *
 * Foo_t  firstElement = { .foo = 'a', .base.type = FOO };
 * Bar_t secondElement = { .bar = 2,   .base.type = BAR };
 * Foo_t  thirdElement = { .foo = 'c', .base.type = FOO };
 * Bar_t   lastElement = { .bar = 3,   .base.type = BAR };
 *
 * OS_LoggerListT_insert( firstElement.base.node, secondElement.base.node);
 * OS_LoggerListT_insert(secondElement.base.node,  thirdElement.base.node);
 * OS_LoggerListT_insert( thirdElement.base.node,   lastElement.base.node);
 *
 * for(
 *      OS_LoggerNodeT_Handle_t* iterator = firstElement.base.node;
 *      NULL != iterator;
 *      iterator = (OS_LoggerNodeT_Handle_t*)OS_LoggerListT_get_next(iterator);
 * )
 * {
 *      switch(((Base_t*)iterator)->type)
 *      {
 *          case FOO:
 *          {
 *              printf("Foo says: %c", ((Foo_t*)iterator)->foo);
 *              break;
 *          }
 *          case BAR:
 *          {
 *              printf("Bar says: %d", ((Bar_t*)iterator)->bar);
 *              break;
 *          }
 *          default:
 *          {
 *              printf("Unsupported type!");
 *              break;
 *          }
 *      }
 * }
 * @endcode
 *
 * @ingroup OS_LoggerListT
*/
typedef struct OS_LoggerNodeT_Handle
{
    void* prev; //!< Pointer to the previous list's element.
    void* next; //!< Pointer to the next list's element.
} OS_LoggerNodeT_Handle_t;

/**
 * @brief   Checks if the given list element has the reference to the previous
 *          element.
 *
 * @retval  true  - if previous element is referenced.
 * @retval  false - if previous element is NOT referenced.
 *
 * @ingroup OS_LoggerListT
*/
bool
OS_LoggerListT_hasPrevious(
    OS_LoggerNodeT_Handle_t* current //!< [in] Pointer to the current element.
);

/**
 * @brief   Checks if the given list element has the reference to the next
 *          element.
 *
 * @retval  true  - if next element is referenced.
 * @retval  false - if next element is NOT referenced.
 *
 * @ingroup OS_LoggerListT
*/
bool
OS_LoggerListT_hasNext(
    OS_LoggerNodeT_Handle_t* current //!< [in] Pointer to the current element.
);

/**
 * @brief   Gets the previous element from the list.
 *
 * @return  Pointer to the previous list's element.
 *
 * @ingroup OS_LoggerListT
*/
void*
OS_LoggerListT_getPrevious(
    OS_LoggerNodeT_Handle_t* current //!< [in] Pointer to the current element.
);

/**
 * @brief   Gets the next element from the list.
 *
 * @return  Pointer to the next list's element.
 *
 * @ingroup OS_LoggerListT
*/
void*
OS_LoggerListT_getNext(
    OS_LoggerNodeT_Handle_t* current //!< [in] Pointer to the current element.
);

/**
 * @brief   Inserts a new element into the list.
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerListT
*/
OS_Error_t
OS_LoggerListT_insert(
    OS_LoggerNodeT_Handle_t* current, //!< [in] Pointer to the current element.
    OS_LoggerNodeT_Handle_t* newNode  //!< [in] Pointer to the new element.
);

/**
 * @brief   Erases an element from the list.
 *
 * @note    It is users responsibility to delete, if necessary, the given
 *          object!
 *
 * @return  An error code.
 *
 * @ingroup OS_LoggerListT
*/
OS_Error_t
OS_LoggerListT_erase(
    OS_LoggerNodeT_Handle_t* current //!< [in] Pointer to the current element.
);

/**
 * @brief   Gets the first list's element.
 *
 * @return  Pointer to the first list's element.
 *
 * @ingroup OS_LoggerListT
*/
void*
OS_LoggerListT_getFirst(
    OS_LoggerNodeT_Handle_t* current //!< [in] Pointer to the current element.
);

/**
 * @brief   Gets the last list's element.
 *
 * @return  Pointer to the last list's element.
 *
 * @ingroup OS_LoggerListT
*/
void*
OS_LoggerListT_getLast(
    OS_LoggerNodeT_Handle_t* current //!< [in] Pointer to the current element.
);

/**
 * @brief   Checks if the given element is in the list.
 *
 * @retval  true  - if the element is in the list.
 * @retval  false - if the element is NOT in the list.
 *
 * @ingroup OS_LoggerListT
*/
bool
OS_LoggerListT_isInside(
    OS_LoggerNodeT_Handle_t* current //!< [in] Pointer to the current element.
);
