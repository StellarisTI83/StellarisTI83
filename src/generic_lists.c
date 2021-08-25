#include <stdlib.h>

#include <debug.h>

#include "main.h"
#include "generic_lists.h"

/* structures ========================================================== */

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
    GenericList *list = malloc(sizeof(GenericList));
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
    if(!list)
        return;

    while(list->firstElement != NULL) {
        cell = list->firstElement;
        list->firstElement = cell->nextElement;
        free(cell);
    }
    free(list);
}

int GenericList_ArraySize(const GenericList *list) {
    GenericListElement *cell;
    int size = 0;
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
    
    if(!list)
        return;

    cell = calloc(1, sizeof(GenericListElement));
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

    if(!list)
        return;
    
    temporaryCell = list->firstElement;
    if(index == 0) {
        GenericListElement *cell = list->firstElement;
        list->firstElement = temporaryCell->nextElement;
        free(cell);
        return;
    }
    else {
        int actualCell = 1;
        while(temporaryCell->nextElement != NULL) {
            if(actualCell == index) {
                GenericListElement *cell = temporaryCell->nextElement;
                temporaryCell->nextElement = temporaryCell->nextElement->nextElement;
                free(cell);
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