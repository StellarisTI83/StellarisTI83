#ifndef H_GENERIC_LIST
#define H_GENERIC_LIST

/* types =============================================================== */
/**
 * @brief A generic list
 * 
 */
typedef struct _genericList GenericList;

/* entry points ======================================================== */

/**
 * @brief Create a Generic List object
 * 
 * @return GenericList* 
 */
GenericList *GenericList_Create();

/**
 * @brief Free a Generic List object. Free every Generic Cell inside the Generic List
 * 
 * @param list  The list to free
 */
void GenericList_Free(GenericList *list);

/**
 * @brief Return the number of element in a generic list
 * 
 * @param list  The generic list to size
 * @return int 
 */
int GenericList_ArraySize(GenericList *list);



/**
 * @brief Add a generic cell to a Generic List. It can hold any pointer
 * 
 * @param list  Generic list to add cell to
 * @param info  Generic Cell Content
 */
void GenericCell_Add(GenericList *list, void *info);

/**
 * @brief Get the pointer of
 * 
 * @param list  The generic list
 * @param index 
 * @return void* GenericCell content
 */
void *GenericCell_Get(GenericList *list, int index);

/**
 * @brief Free the generic cell. Don't free the content
 * 
 * @param list  The generic list
 * @param index The index of the cell
 */
void GenericCell_Free(GenericList *list, int index);

/**
 * @brief Free the generic cell. Don't free the content
 * 
 * @param list  The generic list
 * @param index The index of the cell
 */
int GenericCell_GetNumber(GenericList *list, void *info);

#endif