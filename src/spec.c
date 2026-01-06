#include "spec.h"
#include "as.h"
#include <stdbool.h>

const opc *find_opc(const ins *i);

const mnem *find_mnem(asblock *bk)
{
    for (u8 i = 0; i != n_mnems; ++i)
    {
        if (!strcmp(bk->tk, mnems[i].name))
        {
            return &mnems[i];
        }
    }
    return NULL;
}

void encode_ins(asblock *bk, ins i)
{
    //
}

const mnem mnems[] = {
    {"brk", 0, 1},
    {"lda", 1, 4},
};
const u8 n_mnems = sizeof(mnems) / sizeof(mnem);
const opc ops[] = {
    // brk
    {0x00, ADDR_IMPL},
    // lda
    {0xA1, ADDR_X_IND},
    {0xB1, ADDR_IND_Y},
    {0xA5, ADDR_ZPG},
    {0xB5, ADDR_ZPG_X},

};
const u8 n_opcs = sizeof(ops) / sizeof(opc);

const opc *find_opc(const ins *i)
{
    for (u8 j = 0; j != n_opcs; ++j)
    {
        bool match = false;
        switch (ops[j].prof)
        {
        case ADDR_A:
            if (i->n_args == 1 &&
                i->args[0].type == ARG_REG &&
                i->args[0].u == 0) // reg a
            {
                match = true;
            }
            break;
        case ADDR_ABS:
            if (i->n_args == 1 && i->args[0].type == ARG_MEM)
            {
                match = true;
            }
            break;
        case ADDR_ABS_X:
            if (i->n_args == 2 &&
                i->args[0].type == ARG_MEM &&
                i->args[0].u == 1) // reg x
            {
                match = true;
            }
            break;
        case ADDR_ABS_Y:
            if (i->n_args == 2 &&
                i->args[0].type == ARG_MEM &&
                i->args[0].u == 2) // reg y
            {
                match = true;
            }
            break;
        case ADDR_IMM:
            if (i->n_args == 1 && i->args[0].type == ARG_IMM)
            {
                match = true;
            }
            break;
        case ADDR_IMPL:
            if (!i->n_args)
            {
                match = true;
            }
            break;
        case ADDR_IND:
            if (i->n_args == 1 &&
                i->args[0].type == ARG_MEM &&
                i->args[0].dir == BHL(1, 1))
            {
                match = true;
            }
            break;
        case ADDR_X_IND:
            if (i->n_args == 2 &&
                i->args[0].type == ARG_MEM &&
                i->args[0].dir == BHL(1, 0) &&
                i->args[1].type == ARG_REG &&
                i->args[1].u == 1 &&
                i->args[1].dir == BHL(0, 1))
            {
                match = true;
            }
            break;
        default:
            break;
        }
        if (match)
        {
            return &ops[j];
        }
    }
    return NULL;
}
