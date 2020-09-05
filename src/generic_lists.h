#ifndef H_GENERIC_LIST
#define H_GENERIC_LIST

/* types =============================================================== */

typedef struct _genericList GenericList;

/* entry points ======================================================== */

GenericList *CreateGenericList();
void FreeGenericList(GenericList *list);
void GenericCellAdd(GenericList *list, void *info);
void *GenericCellGet(GenericList *list, int number);
void FreeGenericCell(GenericList *list, int number);
int GenericCellGetNumber(GenericList *list, void *info);
int GenericListArraySize(GenericList *list);

#endif