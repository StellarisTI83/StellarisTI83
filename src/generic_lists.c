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

GenericList *CreateGenericList() {
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

void FreeGenericList(GenericList *list) {
    GenericListElement *cell = NULL;
    while(list->firstElement != NULL) {
        cell = list->firstElement;
        list->firstElement = cell->nextElement;
        free(cell);
    }
    free(list);
}

void GenericCellAdd(GenericList *list, void *info) {
    GenericListElement *cell = calloc(1, sizeof(GenericListElement));

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

void *GenericCellGet(GenericList *list, int number) {
    GenericListElement *cell = list->firstElement;
    int  actualCell = 1;
    while(cell != NULL){
        if(actualCell == number){
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

void FreeGenericCell(GenericList *list, int number) {
    GenericListElement *temporaryCell = list->firstElement;
    if(number == 1) {
        GenericListElement *cell = list->firstElement;
        list->firstElement = temporaryCell->nextElement;
        free(cell);
        return;
    }
    else {
        int actualCell = 2;
        while(temporaryCell->nextElement != NULL) {
            if(actualCell == number) {
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

int GenericCellGetNumber(GenericList *list, void *info) {
    GenericListElement *temporaryCell = list->firstElement;
    int  actualCell = 1;
    while(temporaryCell != NULL){
        if(temporaryCell->element == info)
            return actualCell;

        temporaryCell = temporaryCell->nextElement;
        actualCell++;
    }
    return 0;
}

int GenericListArraySize(GenericList *list) {
    GenericListElement *cell = list->firstElement;
    int size = 0;
    while(cell != NULL) {
        cell = cell->nextElement;
        size++;
    }
    return size;
}