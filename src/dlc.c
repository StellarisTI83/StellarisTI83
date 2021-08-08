#include <fileioc.h>

#include "dlc.h"

/* types =============================================================== */



/* private functions =================================================== */

void dlc_Parse(const ti_var_t dlc_var) {

}


/* entry points ======================================================== */
/**
 * Load a dlc/data set with his name
 */
void dlc_Load(const char *name) {
    ti_var_t dlc_var;

	ti_CloseAll();

    dlc_var = ti_Open(name, "r");

    dlc_Parse(dlc_var);

    ti_Close(dlc_var);
}
