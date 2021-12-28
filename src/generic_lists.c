#include <stdlib.h>

#include <debug.h>

#include "main.h"
#include "generic_lists.h"

/* struct ============================================================== */

typedef struct _genericListElement GenericListElement;

struct _genericListElement {
    void *element;
    GenericListElement *nextElement;
};

struct _genericList {
    GenericListElement *firstElement;
};

/* entry points ======================================================== */

GenericList *GenericList_Create() {
    GenericList *list = malloc_count(sizeof(GenericList));
    #ifdef LOG_VERSION
	dbg_sprintf(dbgerr, "Create generic list %p\n", list);
    #endif
	if(!list){
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when creating generic list");
		#endif
        exit(EXIT_FAILURE);
    }
    list->firstElement = NULL;
    return list;
}

void GenericList_Free(GenericList *list) {
    GenericListElement *cell = NULL;
    #ifdef LOG_VERSION
	dbg_sprintf(dbgerr, "Free generic list\n");
    #endif
    if(!list)
        return;

    while(list->firstElement != NULL) {
        cell = list->firstElement;
        list->firstElement = cell->nextElement;
        free_count(cell);
    }
    free_count(list);
}

int GenericList_ArraySize(const GenericList *list) {
    GenericListElement *cell;
    int size = 0;
    #ifdef LOG_VERSION
	dbg_sprintf(dbgerr, "GenericList_ArraySize\n");
    #endif
    if(!list)
        return size;
    
    cell = list->firstElement;
    while(cell != NULL) {
        cell = cell->nextElement;
        size++;
    }
    return size;
}

void GenericCell_Add(GenericList *list, void *info) {
    GenericListElement *cell;
    #ifdef LOG_VERSION
	dbg_sprintf(dbgerr, "GenericCell_Add\n");
    #endif
    if(!list)
        return;

    cell = calloc_count(1, sizeof(GenericListElement));
	if(!cell){
		#ifdef DEBUG_VERSION
		dbg_sprintf(dbgerr, "Malloc returned NULL when adding generic cell");
		#endif
        exit(EXIT_FAILURE);
    }
    
    cell->element = info;
    cell->nextElement = NULL;

    if(list->firstElement == NULL) {
        list->firstElement = cell;
    }
    else {
        GenericListElement *tempCell = list->firstElement;
        while(tempCell->nextElement != NULL){
            tempCell = tempCell->nextElement;
        }
        tempCell->nextElement = cell;
    }
}

void *GenericCell_Get(const GenericList *list, const int index) {
    GenericListElement *cell = list->firstElement;
    int  actualCell = 0;
    #ifdef LOG_VERSION
	dbg_sprintf(dbgerr, "GenericCell_Get\n");
    #endif
    while(cell != NULL){
        if(actualCell == index){
            return cell->element;
            #ifdef DEBUG_VERSION
                dbg_sprintf(dbgout, "Cell: %p, Next: %p\n", cell, cell->nextElement);
            #endif
        }
        
        cell = cell->nextElement;
        actualCell++;
    }
    return NULL;
}

void GenericCell_Free(GenericList *list, int index) {
    GenericListElement *temporaryCell;
    #ifdef LOG_VERSION
	dbg_sprintf(dbgerr, "GenericCell_Free\n");
    #endif

    if(!list)
        return;
    
    temporaryCell = list->firstElement;
    if(index == 0) {
        GenericListElement *cell = list->firstElement;
        list->firstElement = temporaryCell->nextElement;
        free_count(cell);
        return;
    }
    else {
        int actualCell = 1;
        while(temporaryCell->nextElement != NULL) {
            if(actualCell == index) {
                GenericListElement *cell = temporaryCell->nextElement;
                temporaryCell->nextElement = temporaryCell->nextElement->nextElement;
                free_count(cell);
                return;
            }
            temporaryCell = temporaryCell->nextElement;
            actualCell++;
        }
    }
}

int GenericCell_GetNumber(const GenericList *list, const void *info) {
    GenericListElement *temporaryCell;
    int  actualCell = 0;
    #ifdef LOG_VERSION
	dbg_sprintf(dbgerr, "GenericCell_GetNumber\n");
    #endif
    
    if(!list)
        return 0;
    
    temporaryCell = list->firstElement;
    while(temporaryCell != NULL){
        if(temporaryCell->element == info)
            return actualCell;

        temporaryCell = temporaryCell->nextElement;
        actualCell++;
    }
    return 0;
}