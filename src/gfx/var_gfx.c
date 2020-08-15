// convpng v7.3
#include <stdint.h>
#include "var_gfx.h"

#include <fileioc.h>
uint8_t *var_gfx[79] = {
 (uint8_t*)0,
 (uint8_t*)44,
 (uint8_t*)66,
 (uint8_t*)93,
 (uint8_t*)125,
 (uint8_t*)152,
 (uint8_t*)179,
 (uint8_t*)206,
 (uint8_t*)233,
 (uint8_t*)316,
 (uint8_t*)395,
 (uint8_t*)425,
 (uint8_t*)469,
 (uint8_t*)1312,
 (uint8_t*)2155,
 (uint8_t*)2998,
 (uint8_t*)3841,
 (uint8_t*)4684,
 (uint8_t*)5527,
 (uint8_t*)6370,
 (uint8_t*)7213,
 (uint8_t*)8056,
 (uint8_t*)8899,
 (uint8_t*)9742,
 (uint8_t*)10585,
 (uint8_t*)11428,
 (uint8_t*)12271,
 (uint8_t*)13114,
 (uint8_t*)13957,
 (uint8_t*)14800,
 (uint8_t*)15643,
 (uint8_t*)16486,
 (uint8_t*)16608,
 (uint8_t*)16630,
 (uint8_t*)16731,
 (uint8_t*)16742,
 (uint8_t*)16753,
 (uint8_t*)16764,
 (uint8_t*)16775,
 (uint8_t*)16813,
 (uint8_t*)16843,
 (uint8_t*)16894,
 (uint8_t*)17013,
 (uint8_t*)17158,
 (uint8_t*)17286,
 (uint8_t*)17344,
 (uint8_t*)17442,
 (uint8_t*)17564,
 (uint8_t*)17638,
 (uint8_t*)17920,
 (uint8_t*)17947,
 (uint8_t*)17998,
 (uint8_t*)18049,
 (uint8_t*)18100,
 (uint8_t*)18223,
 (uint8_t*)18267,
 (uint8_t*)18311,
 (uint8_t*)18343,
 (uint8_t*)18369,
 (uint8_t*)18399,
 (uint8_t*)18491,
 (uint8_t*)18518,
 (uint8_t*)18545,
 (uint8_t*)18572,
 (uint8_t*)18599,
 (uint8_t*)18650,
 (uint8_t*)21291,
 (uint8_t*)24922,
 (uint8_t*)28407,
 (uint8_t*)32077,
 (uint8_t*)34352,
 (uint8_t*)37597,
 (uint8_t*)40939,
 (uint8_t*)44519,
 (uint8_t*)46909,
 (uint8_t*)49848,
 (uint8_t*)52394,
 (uint8_t*)55466,
 (uint8_t*)56170,
};

bool var_gfx_init(void) {
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("var_gfx", "r");
    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)var_gfx[0];
    for (i = 0; i < var_gfx_num; i++) {
        var_gfx[i] += data;
    }

    ti_CloseAll();

    return (bool)appvar;
}
