#pragma once

#include "defs.h"

#define MNEM_MAX 3

struc(mnem)
{
    char name[MNEM_MAX];
    u8 idx;
    u8 num;
};

enum
{
    ADDR_A,
    ADDR_ABS,
    ADDR_ABS_X,
    ADDR_ABS_Y,
    ADDR_IMM,
    ADDR_IMPL,
    ADDR_IND,
    ADDR_X_IND,
    ADDR_IND_Y,
    ADDR_REL,
    ADDR_ZPG,
    ADDR_ZPG_X,
    ADDR_ZPG_Y,
};

struc(opc)
{
    u8 code;
    i8 prof;
};

extern const mnem mnems[];
extern const u8 n_mnems;
extern const opc ops[];
extern const u8 n_opcs;
