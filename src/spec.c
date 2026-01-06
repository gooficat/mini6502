#include "spec.h"
#include "as.h"
#include <stdbool.h>

#define MAX_INS_BYTES 3

const opc *find_opc(asblock *bk, const ins *i);

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
    const opc *op = find_opc(bk, &i);
    printf("matched to code 0x%hhX\n", op->code);

    u8 bytes[MAX_INS_BYTES];
    i8 n_bytes = 1;
    bytes[0] = op->code;

    switch (op->prof)
    {
    case ADDR_A:
        break;
    case ADDR_ABS:
        break;
    case ADDR_ABS_X:
        break;
    case ADDR_ABS_Y:
        break;
    case ADDR_IMM:
        //
        break;
    case ADDR_IMPL:
        break;
    case ADDR_IND:
        break;
    case ADDR_X_IND:
        break;
    default:
        break;
    }

    bk->off += n_bytes;

    if (bk->pass == PASS_WRIT)
    {
        fwrite(bytes, 1, n_bytes, bk->out);
    }
}

const mnem mnems[] = {
    {"brk", 0, 1},
    {"lda", 1, 8},
    {"jmp", 9, 2},
    {"adc", 11, 8},
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
    {0xA9, ADDR_IMM},
    {0xB9, ADDR_ABS_Y},
    {0xAD, ADDR_ABS},
    {0xBD, ADDR_ABS_X},
    // jmp
    {0x4C, ADDR_ABS},
    {0x6C, ADDR_IND},
    // adc
    {0x61, ADDR_X_IND},
    {0x71, ADDR_IND_Y},
    {0x65, ADDR_ZPG},
    {0x75, ADDR_ZPG_X},
    {0x69, ADDR_IMM},
    {0x79, ADDR_ABS_Y},
    {0x6D, ADDR_ABS},
    {0x7D, ADDR_ABS_X},
};
const u8 n_opcs = sizeof(ops) / sizeof(opc);

const opc *find_opc(asblock *bk, const ins *i)
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
        case ADDR_ZPG:
            if (i->n_args == 1 &&
                i->args[0].type == ARG_MEM &&
                !i->args[0].dir)
            {
                if (bk->pass == PASS_IMAG ||
                    i->args[0].u < 0xFF)
                {
                    match = true;
                }
            }
            break;
        case ADDR_ZPG_X:
            if (i->n_args == 2 &&
                i->args[0].type == ARG_MEM &&
                i->args[1].type == ARG_REG &&
                i->args[1].u == 1 && // reg x
                !i->args[0].dir &&
                !i->args[1].dir)
            {
                if (bk->pass == PASS_IMAG ||
                    i->args[0].u < 0xFF)
                {
                    match = true;
                }
            }
            break;
        case ADDR_ZPG_Y:
            if (i->n_args == 2 &&
                i->args[0].type == ARG_MEM &&
                i->args[1].type == ARG_REG &&
                i->args[1].u == 2 && // reg y
                !i->args[0].dir &&
                !i->args[1].dir)
            {
                if (bk->pass == PASS_IMAG ||
                    i->args[0].u < 0xFF)
                {
                    match = true;
                }
            }
            break;
        case ADDR_REL:
        {
            i16 v = bk->off + 2 - i->args[0].i;
            if (i->n_args == 1 && i->args[0].type == ARG_MEM)
            {
                if (bk->pass == PASS_IMAG || (v < INT8_MAX && v > INT8_MIN))
                {
                    match = true;
                }
            }
            break;
        }
        }
        if (match)
        {
            return &ops[j];
        }
    }
    return NULL;
}
