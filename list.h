/* =============================================================================
 *
 * list.h
 * -- Sorted singly linked list
 * -- Options: -DLIST_NO_DUPLICATES (default: allow duplicates)
 *
 * =============================================================================
*/


#ifndef LIST_H
#define LIST_H 1

#include "types.h"


typedef struct list_node list_node_t;

typedef list_node_t* list_iter_t;

typedef struct list list_t;

typedef long (*cmp_f) (const void *, const void *);
typedef void (*rel_f) (void *);
/* =============================================================================
 * list_iter_reset
 * =============================================================================
 */
void
list_iter_reset (list_iter_t* itPtr, list_t* listPtr);


/* =============================================================================
 * list_iter_hasNext
 * =============================================================================
 */
bool_t
list_iter_hasNext (list_iter_t* itPtr, list_t* listPtr);


/* =============================================================================
 * list_iter_next
 * =============================================================================
 */
void*
list_iter_next (list_iter_t* itPtr, list_t* listPtr);


/* =============================================================================
 * list_alloc
 * -- If NULL passed for 'compare' function, will compare data pointer addresses
 * -- Returns NULL on failure
 * =============================================================================
 */
list_t*
list_alloc (cmp_f compare, rel_f rel);


/* =============================================================================
 * list_free
 * =============================================================================
 */
void
list_free (list_t* listPtr);


/* =============================================================================
 * list_isEmpty
 * -- Return TRUE if list is empty, else FALSE
 * =============================================================================
 */
bool_t
list_isEmpty (list_t* listPtr);


/* =============================================================================
 * list_getSize
 * -- Returns size of list
 * =============================================================================
 */
long
list_getSize (list_t* listPtr);


/* =============================================================================
 * list_front
 * -- Returns NULL if list is empty, else returns pointer to data to first 
 *    element
 * =============================================================================
 */
void* 
list_front(list_t* listPtr);


/* =============================================================================
 * list_back
 * -- Returns NULL if list is empty, else returns pointer to data to last 
 *    element
 * =============================================================================
 */
void* 
list_back(list_t* listPtr);


/* =============================================================================
 * list_find
 * -- Returns NULL if not found, else returns pointer to data
 * =============================================================================
 */
void*
list_find (list_t* listPtr, void* dataPtr);


/* =============================================================================
 * list_push_front
 * -- Inserts an element to the beginning 
 * -- Return TRUE on success, else FALSE
 * =============================================================================
 */
bool_t
list_push_front (list_t* listPtr, void* dataPtr);


/* =============================================================================
 * list_push_back
 * -- Adds an element to the end 
 * -- Return TRUE on success, else FALSE
 * =============================================================================
 */
bool_t
list_push_back (list_t* listPtr, void* dataPtr);


/* =============================================================================
 * list_insert
 * -- Return TRUE on success, else FALSE
 * =============================================================================
 */
bool_t
list_insert (list_t* listPtr, void* dataPtr);


/* =============================================================================
 * list_remove
 * -- Returns TRUE if successful, else FALSE
 * =============================================================================
 */
bool_t
list_remove (list_t* listPtr, void* dataPtr);


/* =============================================================================
 * list_pop_back
 * -- Removes last element from the list
 * -- Return TRUE if successful, else FALSE
 * =============================================================================
 */
bool_t
list_pop_back(list_t* listPtr);


/* =============================================================================
 * list_clear
 * -- Removes all elements
 * =============================================================================
 */
void
list_clear (list_t* listPtr);

#endif /* LIST_H */


/* =============================================================================
 *
 * End of list.h
 *
 * =============================================================================
 */
