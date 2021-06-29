#include "gfx.h"
#include <fileioc.h>

#define var_gfx_HEADER_SIZE 0

unsigned char *var_gfx_appvar[84] =
{
    (unsigned char*)0,
    (unsigned char*)140,
    (unsigned char*)652,
    (unsigned char*)696,
    (unsigned char*)740,
    (unsigned char*)784,
    (unsigned char*)828,
    (unsigned char*)879,
    (unsigned char*)930,
    (unsigned char*)981,
    (unsigned char*)1824,
    (unsigned char*)2667,
    (unsigned char*)3510,
    (unsigned char*)4353,
    (unsigned char*)5196,
    (unsigned char*)6039,
    (unsigned char*)6882,
    (unsigned char*)7725,
    (unsigned char*)8568,
    (unsigned char*)9411,
    (unsigned char*)10254,
    (unsigned char*)11097,
    (unsigned char*)11940,
    (unsigned char*)12783,
    (unsigned char*)13626,
    (unsigned char*)14469,
    (unsigned char*)15312,
    (unsigned char*)16155,
    (unsigned char*)16998,
    (unsigned char*)17077,
    (unsigned char*)17200,
    (unsigned char*)17301,
    (unsigned char*)17423,
    (unsigned char*)17434,
    (unsigned char*)17445,
    (unsigned char*)17456,
    (unsigned char*)17467,
    (unsigned char*)17550,
    (unsigned char*)17642,
    (unsigned char*)17693,
    (unsigned char*)17720,
    (unsigned char*)17747,
    (unsigned char*)17774,
    (unsigned char*)17801,
    (unsigned char*)17828,
    (unsigned char*)17855,
    (unsigned char*)17882,
    (unsigned char*)17909,
    (unsigned char*)17939,
    (unsigned char*)17969,
    (unsigned char*)17999,
    (unsigned char*)18037,
    (unsigned char*)18059,
    (unsigned char*)18110,
    (unsigned char*)18212,
    (unsigned char*)18331,
    (unsigned char*)18476,
    (unsigned char*)18604,
    (unsigned char*)18662,
    (unsigned char*)18760,
    (unsigned char*)18882,
    (unsigned char*)18956,
    (unsigned char*)18983,
    (unsigned char*)19015,
    (unsigned char*)19042,
    (unsigned char*)19064,
    (unsigned char*)19262,
    (unsigned char*)19364,
    (unsigned char*)19406,
    (unsigned char*)19448,
    (unsigned char*)19540,
    (unsigned char*)19577,
    (unsigned char*)20247,
    (unsigned char*)22161,
    (unsigned char*)25104,
    (unsigned char*)28702,
    (unsigned char*)31470,
    (unsigned char*)33778,
    (unsigned char*)36633,
    (unsigned char*)38899,
    (unsigned char*)41755,
    (unsigned char*)43575,
    (unsigned char*)45383,
    (unsigned char*)46732,
};

unsigned char *background0_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background1_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background2_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background3_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background4_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background5_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background6_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background7_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background8_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background9_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background10_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char *background11_tiles_compressed[1] =
{
    (unsigned char*)0,
};

unsigned char var_gfx_init(void)
{
    unsigned int data, i;
    ti_var_t appvar;

    ti_CloseAll();

    appvar = ti_Open("var_gfx", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)var_gfx_appvar[0] + var_gfx_HEADER_SIZE;
    for (i = 0; i < 84; i++)
    {
        var_gfx_appvar[i] += data;
    }

    ti_CloseAll();

    data = (unsigned int)var_gfx_appvar[72] - (unsigned int)background0_tiles_compressed[0];
    for (i = 0; i < background0_tiles_num; i++)
    {
        background0_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[73] - (unsigned int)background1_tiles_compressed[0];
    for (i = 0; i < background1_tiles_num; i++)
    {
        background1_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[74] - (unsigned int)background2_tiles_compressed[0];
    for (i = 0; i < background2_tiles_num; i++)
    {
        background2_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[75] - (unsigned int)background3_tiles_compressed[0];
    for (i = 0; i < background3_tiles_num; i++)
    {
        background3_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[76] - (unsigned int)background4_tiles_compressed[0];
    for (i = 0; i < background4_tiles_num; i++)
    {
        background4_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[77] - (unsigned int)background5_tiles_compressed[0];
    for (i = 0; i < background5_tiles_num; i++)
    {
        background5_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[78] - (unsigned int)background6_tiles_compressed[0];
    for (i = 0; i < background6_tiles_num; i++)
    {
        background6_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[79] - (unsigned int)background7_tiles_compressed[0];
    for (i = 0; i < background7_tiles_num; i++)
    {
        background7_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[80] - (unsigned int)background8_tiles_compressed[0];
    for (i = 0; i < background8_tiles_num; i++)
    {
        background8_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[81] - (unsigned int)background9_tiles_compressed[0];
    for (i = 0; i < background9_tiles_num; i++)
    {
        background9_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[82] - (unsigned int)background10_tiles_compressed[0];
    for (i = 0; i < background10_tiles_num; i++)
    {
        background10_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[83] - (unsigned int)background11_tiles_compressed[0];
    for (i = 0; i < background11_tiles_num; i++)
    {
        background11_tiles_compressed[i] += data;
    }

    return 1;
}

