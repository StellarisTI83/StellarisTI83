#include "gfx.h"
#include <fileioc.h>

#define var_gfx_HEADER_SIZE 0

unsigned char *var_gfx_appvar[88] =
{
    (unsigned char*)0,
    (unsigned char*)148,
    (unsigned char*)660,
    (unsigned char*)1503,
    (unsigned char*)2346,
    (unsigned char*)3189,
    (unsigned char*)4032,
    (unsigned char*)4875,
    (unsigned char*)5718,
    (unsigned char*)6561,
    (unsigned char*)7404,
    (unsigned char*)8247,
    (unsigned char*)9090,
    (unsigned char*)9933,
    (unsigned char*)10776,
    (unsigned char*)11619,
    (unsigned char*)12462,
    (unsigned char*)13305,
    (unsigned char*)14148,
    (unsigned char*)14991,
    (unsigned char*)15834,
    (unsigned char*)16677,
    (unsigned char*)16769,
    (unsigned char*)16891,
    (unsigned char*)16913,
    (unsigned char*)17015,
    (unsigned char*)17098,
    (unsigned char*)17177,
    (unsigned char*)17219,
    (unsigned char*)17241,
    (unsigned char*)17285,
    (unsigned char*)17329,
    (unsigned char*)17373,
    (unsigned char*)17417,
    (unsigned char*)17518,
    (unsigned char*)17556,
    (unsigned char*)17586,
    (unsigned char*)17613,
    (unsigned char*)17650,
    (unsigned char*)17677,
    (unsigned char*)17875,
    (unsigned char*)17926,
    (unsigned char*)18049,
    (unsigned char*)18121,
    (unsigned char*)18223,
    (unsigned char*)18255,
    (unsigned char*)18306,
    (unsigned char*)18332,
    (unsigned char*)18383,
    (unsigned char*)18434,
    (unsigned char*)18464,
    (unsigned char*)18494,
    (unsigned char*)18545,
    (unsigned char*)18637,
    (unsigned char*)18669,
    (unsigned char*)18696,
    (unsigned char*)18723,
    (unsigned char*)18750,
    (unsigned char*)18777,
    (unsigned char*)18788,
    (unsigned char*)18799,
    (unsigned char*)18810,
    (unsigned char*)18821,
    (unsigned char*)18848,
    (unsigned char*)18875,
    (unsigned char*)18902,
    (unsigned char*)18929,
    (unsigned char*)18971,
    (unsigned char*)19090,
    (unsigned char*)19235,
    (unsigned char*)19363,
    (unsigned char*)19421,
    (unsigned char*)19519,
    (unsigned char*)19641,
    (unsigned char*)19715,
    (unsigned char*)19997,
    (unsigned char*)20664,
    (unsigned char*)22578,
    (unsigned char*)25534,
    (unsigned char*)29151,
    (unsigned char*)31909,
    (unsigned char*)34232,
    (unsigned char*)37075,
    (unsigned char*)39349,
    (unsigned char*)42229,
    (unsigned char*)44049,
    (unsigned char*)45857,
    (unsigned char*)47206,
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

    appvar = ti_Open("var_gfx", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)var_gfx_appvar[0] + var_gfx_HEADER_SIZE;
    for (i = 0; i < 88; i++)
    {
        var_gfx_appvar[i] += data;
    }

    ti_Close(appvar);

    data = (unsigned int)var_gfx_appvar[76] - (unsigned int)background0_tiles_compressed[0];
    for (i = 0; i < background0_tiles_num; i++)
    {
        background0_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[77] - (unsigned int)background1_tiles_compressed[0];
    for (i = 0; i < background1_tiles_num; i++)
    {
        background1_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[78] - (unsigned int)background2_tiles_compressed[0];
    for (i = 0; i < background2_tiles_num; i++)
    {
        background2_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[79] - (unsigned int)background3_tiles_compressed[0];
    for (i = 0; i < background3_tiles_num; i++)
    {
        background3_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[80] - (unsigned int)background4_tiles_compressed[0];
    for (i = 0; i < background4_tiles_num; i++)
    {
        background4_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[81] - (unsigned int)background5_tiles_compressed[0];
    for (i = 0; i < background5_tiles_num; i++)
    {
        background5_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[82] - (unsigned int)background6_tiles_compressed[0];
    for (i = 0; i < background6_tiles_num; i++)
    {
        background6_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[83] - (unsigned int)background7_tiles_compressed[0];
    for (i = 0; i < background7_tiles_num; i++)
    {
        background7_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[84] - (unsigned int)background8_tiles_compressed[0];
    for (i = 0; i < background8_tiles_num; i++)
    {
        background8_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[85] - (unsigned int)background9_tiles_compressed[0];
    for (i = 0; i < background9_tiles_num; i++)
    {
        background9_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[86] - (unsigned int)background10_tiles_compressed[0];
    for (i = 0; i < background10_tiles_num; i++)
    {
        background10_tiles_compressed[i] += data;
    }

    data = (unsigned int)var_gfx_appvar[87] - (unsigned int)background11_tiles_compressed[0];
    for (i = 0; i < background11_tiles_num; i++)
    {
        background11_tiles_compressed[i] += data;
    }

    return 1;
}

