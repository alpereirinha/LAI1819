/* =============================================================================
 *
 * list.c
 * -- Sorted doubly linked list
 * -- Options: -DLIST_NO_DUPLICATES (default: allow duplicates)
 *
 * ============================================================================= 
 */


#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "types.h"

struct list_node {
	void* dataPtr;
	struct list_node* prevPtr;
	struct list_node* nextPtr;
};

struct list {
	list_node_t head;
	list_node_t tail;
	long size;
	rel_f release;
	cmp_f compare;
};

/* =============================================================================
 * compareDataPtrAddresses
 * -- Default compare function
 * =============================================================================
 */
static long 
compareDataPtrAddresses (const void* a, const void* b) {
	return ((long)a - (long)b);
}


/* =============================================================================
 * list_iter_reset
 * =============================================================================
 */
void
list_iter_reset (list_iter_t* itPtr, list_t* listPtr) {
	*itPtr = &(listPtr->head);
}


/* =============================================================================
 * list_iter_hasNext
 * =============================================================================
 */
bool_t
list_iter_hasNext (list_iter_t* itPtr, list_t* list) {
	return (((*itPtr)->nextPtr != NULL) ? TRUE : FALSE);
}


/* =============================================================================
 * list_iter_Next
 * =============================================================================
 */
void*
list_iter_next (list_iter_t* itPtr, list_t* list) {
	*itPtr = (*itPtr)->nextPtr;
	
	return (*itPtr)->dataPtr;
}


/* =============================================================================
 * allocNode
 * -- Returns NULL on failure
 * =============================================================================
 */
static list_node_t*
allocNode (void* dataPtr) {
	list_node_t* nodePtr = (list_node_t*) malloc (sizeof(list_node_t));
	
	if(nodePtr == NULL) {
		return NULL;
	}

	nodePtr->dataPtr = dataPtr;
	nodePtr->prevPtr = NULL;
	nodePtr->nextPtr = NULL; 

	return nodePtr;
}


/* =============================================================================
 * list_alloc
 * -- If NULL passed for 'compare' funtion, will compare data pointer addresses 
 * -- Returns NULL on failure
 * =============================================================================
 */
list_t* 
list_alloc (cmp_f compare, rel_f release) {
	list_t* listPtr = (list_t*) malloc (sizeof(list_t));

	if (listPtr == NULL) {
		return NULL;
	}

	listPtr->head.nextPtr = NULL;
	listPtr->tail.prevPtr = NULL;
	listPtr->head.dataPtr = NULL;

	listPtr->tail.nextPtr = NULL;
	listPtr->tail.prevPtr = NULL;
	listPtr->tail.dataPtr = NULL;

	listPtr->size = 0;

	if (compare == NULL) {
		listPtr->compare = &compareDataPtrAddresses; /* default */
	} else {
		listPtr->compare = compare;
	}
 
	listPtr->release = release;

	return listPtr;
}	


/* =============================================================================
 * freeNode
 * =============================================================================
 */
static void 
freeNode (list_t* listPtr, list_node_t* nodePtr) {
	if (listPtr->release) {
		listPtr->release(nodePtr->dataPtr);
	}
	free(nodePtr);
} 


/* =============================================================================
 * freeList
 * =============================================================================
 */
static void
freeList (list_t* listPtr, list_node_t* nodePtr) {
	if (nodePtr != NULL) {
		freeList(listPtr, nodePtr->nextPtr);
		freeNode(listPtr, nodePtr);
	}
}


/* =============================================================================
 * list_free
 * =============================================================================
 */
void 
list_free (list_t* listPtr) {
	freeList(listPtr, listPtr->head.nextPtr);
	free(listPtr);
}


/* =============================================================================
 * list_isEmpty
 * -- Return TRUE if list is empty, else False
 * =============================================================================
 */
bool_t
list_isEmpty (list_t* listPtr) {
	return listPtr->head.nextPtr == NULL;
}


/* =============================================================================
 * list_getSize
 * -- Returns the size of the list
 * =============================================================================
 */
long 
list_getSize (list_t* listPtr) {
	return listPtr->size;
}


/* =============================================================================
 * list_front
 * -- Returns NULL if list is empty, else returns pointer to data to first 
 *    element
 * =============================================================================
 */
void* 
list_front(list_t* listPtr) {
	return listPtr->head.nextPtr == NULL ? NULL : listPtr->head.nextPtr->dataPtr;
}


/* =============================================================================
 * list_back
 * -- Returns NULL if list is empty, else returns pointer to data to last 
 *    element
 * =============================================================================
 */
void* 
list_back(list_t* listPtr) {
	return listPtr->tail.prevPtr == NULL ? NULL : listPtr->tail.prevPtr->dataPtr;
}


/* =============================================================================
 * findPrevious
 * =============================================================================
 */
static list_node_t*
findPrevious (list_t* listPtr, void* dataPtr) {
	list_node_t* prevPtr = &(listPtr->head);
	list_node_t* nodePtr = prevPtr->nextPtr;

	for( ; nodePtr != NULL; nodePtr = nodePtr->nextPtr) {
		if (listPtr->compare(nodePtr->dataPtr, dataPtr) >= 0) {
			return prevPtr;
		}
		prevPtr = nodePtr;
	}

	return prevPtr;
}


/* =============================================================================
 * list_find
 * -- Returns NULL if not found, else returns pointer to data
 * =============================================================================
 */
void*
list_find (list_t* listPtr, void* dataPtr) {
	list_node_t* nodePtr;
	list_node_t* prevPtr = findPrevious(listPtr, dataPtr);

	nodePtr = prevPtr->nextPtr;

	if (nodePtr == NULL || listPtr->compare(nodePtr->dataPtr, dataPtr) != 0) { 
		return NULL;
	}

	return nodePtr->dataPtr;
} 

/* =============================================================================
 * list_push_front
 * -- Inserts an element to the beginning 
 * -- Return TRUE on success, else FALSE
 * =============================================================================
 */
bool_t
list_push_front (list_t* listPtr, void* dataPtr) {
	list_node_t* nodePtr;

#ifdef LIST_NO_DUPLICATES
	list_node_t* currPtr;
	list_node_t* prevPtr;

	prevPtr = findPrevious(listPtr, dataPtr);
	currPtr = prevPtr->nextPtr;

	if((currPtr != NULL) && (listPtr->compare(currPtr->dataPtr, dataPtr) == 0)) {
		return FALSE;
	}
#endif

	nodePtr = allocNode(dataPtr);

	if (nodePtr == NULL) {
		return FALSE;
	}

	if (listPtr->head.nextPtr == NULL) {
		listPtr->head.nextPtr = nodePtr;
		listPtr->tail.prevPtr = nodePtr;
	}
	else {
		nodePtr->nextPtr = listPtr->head.nextPtr;
		listPtr->head.nextPtr->prevPtr = nodePtr;
		listPtr->head.nextPtr = nodePtr;
	}

	listPtr->size++;

	return TRUE;
}

/* =============================================================================
 * list_push_back
 * -- Adds an element to the end 
 * -- Return TRUE on success, else FALSE
 * =============================================================================
 */
bool_t
list_push_back (list_t* listPtr, void* dataPtr) {
	list_node_t* nodePtr;

#ifdef LIST_NO_DUPLICATES
	list_node_t* currPtr;
	list_node_t* prevPtr;
	
	prevPtr = findPrevious(listPtr, dataPtr);
	currPtr = prevPtr->nextPtr;

	if((currPtr != NULL) && (listPtr->compare(currPtr->dataPtr, dataPtr) == 0)) {
		return FALSE;
	}
#endif

	nodePtr = allocNode(dataPtr);

	if (nodePtr == NULL) {
		return FALSE;
	}

	if (listPtr->tail.prevPtr == NULL) {
		listPtr->head.nextPtr = nodePtr;
		listPtr->tail.prevPtr = nodePtr;
	}
	else {
		nodePtr->prevPtr = listPtr->tail.prevPtr;
		listPtr->tail.prevPtr->nextPtr = nodePtr;
		listPtr->tail.prevPtr = nodePtr;
	}

	listPtr->size++;
	
	return TRUE;
}

/* =============================================================================
 * list_insert
 * =============================================================================
 */
bool_t
list_insert(list_t* listPtr, void* dataPtr) {
	list_node_t* nodePtr;
	list_node_t* prevPtr;
	list_node_t* currPtr;

	prevPtr = findPrevious(listPtr, dataPtr);
	currPtr = prevPtr->nextPtr;

#ifdef LIST_NO_DUPLICATES
	if((currPtr != NULL) && (listPtr->compare(currPtr->dataPtr, dataPtr) == 0)) {
		return FALSE;
	}
#endif

	nodePtr = allocNode(dataPtr);
	if (nodePtr == NULL) {
		return FALSE;
	}

	if (prevPtr == &(listPtr->head)) {
		nodePtr->nextPtr = prevPtr->nextPtr;
		prevPtr->nextPtr = nodePtr;
		nodePtr->prevPtr = prevPtr; 
	} else {
		prevPtr->nextPtr = nodePtr;
		nodePtr->prevPtr = prevPtr;
		nodePtr->nextPtr = currPtr;
	}

	if (currPtr != NULL) {
		currPtr->prevPtr = nodePtr;
	}
	else {
		listPtr->tail.prevPtr = nodePtr; 
	}

	listPtr->size++;

	return TRUE;
}


/* =============================================================================
 * list_remove
 * -- Returns TRUE if sucessfull, else FALSE
 * =============================================================================
 */
bool_t
list_remove(list_t* listPtr, void* dataPtr) {
	list_node_t* nodePtr;
	list_node_t* prevPtr;

	prevPtr = findPrevious(listPtr, dataPtr);
	nodePtr = prevPtr->nextPtr;

	if((nodePtr != NULL) && (listPtr->compare(nodePtr->dataPtr, dataPtr) == 0)) {
		prevPtr->nextPtr = nodePtr->nextPtr;
		nodePtr->prevPtr = NULL;
		nodePtr->nextPtr = NULL;
		if(prevPtr->nextPtr != NULL) {
			prevPtr->nextPtr->prevPtr = prevPtr;
		}
		else {
			listPtr->tail.prevPtr = prevPtr;
		}
		freeNode(listPtr, nodePtr);
		listPtr->size--;
		assert(listPtr->size >= 0);
		return TRUE;
	} 

	return FALSE;
}


/* =============================================================================
 * list_pop_back
 * =============================================================================
 */
bool_t
list_pop_back(list_t* listPtr) {
	list_node_t* nodePtr;
	if (listPtr->tail.prevPtr != NULL) {

		nodePtr = listPtr->tail.prevPtr;

		if (listPtr->head.nextPtr != nodePtr) {
			listPtr->tail.prevPtr = nodePtr->prevPtr;
			listPtr->tail.prevPtr->nextPtr = NULL;
		}
		else {
			listPtr->tail.prevPtr = NULL;
			listPtr->head.nextPtr = NULL;
		}

		nodePtr->nextPtr = NULL;
		nodePtr->prevPtr = NULL;

		freeNode(listPtr, nodePtr);
		listPtr->size--;
		assert(listPtr->size >= 0);
		return TRUE;
	}

	return FALSE;
}

/* =============================================================================
 * list_clear
 * =============================================================================
 */
void 
list_clear(list_t* listPtr) {
	freeList(listPtr, listPtr->head.nextPtr);
	listPtr->head.nextPtr = NULL;
	listPtr->tail.prevPtr = NULL;
	listPtr->size = 0;
}


/* =============================================================================
 * TEST_LIST
 * =============================================================================
 */
#ifdef TEST_LIST


#include <assert.h>
#include <stdio.h>


static long
compare (const void* a, const void* b)
{
    return (*((const long*)a) - *((const long*)b));
}


static void
printList (list_t* listPtr)
{
    list_iter_t it;
    printf("[");
    list_iter_reset(&it, listPtr);
    while (list_iter_hasNext(&it, listPtr)) {
        printf("%li ", *((long*)(list_iter_next(&it, listPtr))));
    }
    puts("]");
}

/*
static void
insertInt (list_t* listPtr, long* data)
{
    printf("Inserting: %li\n", *data);
    list_insert(listPtr, (void*)data);
    printList(listPtr);
}
*/


static void
insertInt (list_t* listPtr, long* data)
{
    printf("Inserting: %li\n", *data);
    list_push_back(listPtr, (void*)data);
    printList(listPtr);
}

static void
removeInt (list_t* listPtr, long* data)
{
    printf("Removing: %li\n", *data);
    list_remove(listPtr, (void*)data);
    printList(listPtr);
}


static void
remove_back (list_t* listPtr)
{
    printf("Removing: %li\n", *(long *)(list_back(listPtr)));
    list_pop_back(listPtr);
    printList(listPtr);
}

int
main ()
{
    list_t* listPtr;
#ifdef LIST_NO_DUPLICATES
    long data1[] = {-1, 3, 1, 4, 1, 5, -1};
#else
    long data1[] = {-1, 3, 1, 4, 5, -1};
#endif
    long data2[] = {-1, 3, 1, 4, 1, 5, -1};
    long i;

    puts("Starting...");

    puts("List sorted by values:");

    /* listPtr = list_alloc(&compare); */
    listPtr = list_alloc(NULL, NULL);

    for (i = 1; data1[i] >= 0; i++) {
        insertInt(listPtr, &data1[i]);
        assert(*((long*)list_find(listPtr, &data1[i])) == data1[i]);
    }

    /*
    for (i = 0; data1[i] >= 0; i++) {
        removeInt(listPtr, &data1[i]);
        assert(list_find(listPtr, &data1[i]) == NULL);
    }
	*/
    for (i = 4; data1[i] >= 0; i--) {
    	remove_back(listPtr);
        assert(list_find(listPtr, &data1[i]) == NULL);
    }

    list_free(listPtr);

    puts("List sorted by addresses:");

    listPtr = list_alloc(NULL, NULL);

    for (i = 1; data2[i] >= 0; i++) {
        insertInt(listPtr, &data2[i]);
        assert(*((long*)list_find(listPtr, &data2[i])) == data2[i]);
    }

    for (i = 1; data2[i] >= 0; i++) {
        removeInt(listPtr, &data2[i]);
        assert(list_find(listPtr, &data2[i]) == NULL);
    }

    list_free(listPtr);

    puts("Done.");

    return 0;
}


#endif /* TEST_LIST */


/* =============================================================================
 *
 * End of list.c
 *
 * =============================================================================
 */