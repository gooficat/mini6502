#include "spec.h"
#include "as.h"
#include <stdbool.h>

#define MAX_INS_BYTES 3

const opc *find_opc(asblock *bk, const ins *i);

const mnem *find_mnem(const ins *instr)
{
    for (u8 i = 0; i != n_mnems; ++i)
    {
        if (!strcmp(instr->name, mnems[i].name))
        {
            return &mnems[i];
        }
    }
    // printf("No match for mnem %s", instr->name);
    return NULL;
}

void encode_ins(asblock *bk, ins i)
{
    const opc *op = find_opc(bk, &i);
    // printf("matched %s to code 0x%hhX\n", i.name, op->code);

    u8 bytes[MAX_INS_BYTES];
    i8 n_bytes = 1;
    bytes[0] = op->code;

    switch (op->prof)
    {
    case ADDR_A:
    case ADDR_IMPL:
        break;
    case ADDR_ABS:
    case ADDR_ABS_X:
    case ADDR_ABS_Y:
        if (bk->pass == PASS_WRIT)
        {
            bytes[n_bytes] = i.args[0].u & 0xFF;
            bytes[n_bytes + 1] = (i.args[0].u >> 8) & 0xFF;
        }
        n_bytes += 2;
    case ADDR_IND:
    case ADDR_X_IND:
    case ADDR_IND_Y:
    case ADDR_IMM:
        if (bk->pass == PASS_WRIT)
        {
            bytes[n_bytes] = i.args[0].u & 0xFF;
        }
        ++n_bytes;
        break;
    case ADDR_REL:
        bytes[n_bytes] = (-(bk->off + 2) + i.args[0].i) & 0xFF;
        ++n_bytes;
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
    {"ora", 1, 8},
    {"asl", 9, 5},
    {"php", 14, 1},
    {"bpl", 15, 1},
    {"clc", 16, 1},
    {"jsr", 17, 1},
    {"and", 18, 8},
    {"bit", 26, 2},
    {"rol", 28, 5},
    {"plp", 33, 1},
    {"bmi", 34, 1},
    {"sec", 35, 1},
    {"rti", 36, 1},
    {"eor", 37, 8},
    {"lsr", 45, 5},
    {"pha", 50, 1},
    {"jmp", 51, 2},
    {"bvc", 53, 1},
    {"cli", 54, 1},
    {"rts", 55, 1},
    {"adc", 56, 8},
    {"ror", 64, 5},
    {"pla", 69, 1},
    {"bvs", 70, 1},
    {"sei", 71, 1},
    {"sta", 72, 7},
    {"sty", 79, 3},
    {"stx", 82, 3},
    {"dey", 85, 1},
    {"txa", 86, 1},
    {"bcc", 87, 1},
    {"tya", 88, 1},
    {"txs", 89, 1},
    {"ldy", 90, 5},
    {"lda", 95, 8},
    {"ldx", 103, 5},
    {"tay", 108, 1},
    {"tax", 109, 1},
    {"bcs", 110, 1},
    {"clv", 111, 1},
    {"tsx", 112, 1},
    {"cpy", 113, 3},
    {"cmp", 116, 8},
    {"dec", 124, 4},
    {"iny", 128, 1},
    {"dex", 129, 1},
    {"bne", 130, 1},
    {"cld", 131, 1},
    {"cpx", 132, 3},
    {"sbc", 135, 8},
    {"inc", 143, 4},
    {"inx", 147, 1},
    {"nop", 148, 1},
    {"beq", 149, 1},
    {"sed", 150, 1},
};
const u8 n_mnems = sizeof(mnems) / sizeof(mnem);
const opc ops[] = {
    // brk
    {0x0, ADDR_IMPL},
    // ora
    {0x1, ADDR_X_IND},
    {0x5, ADDR_ZPG},
    {0x9, ADDR_IMM},
    {0xd, ADDR_ABS},
    {0x11, ADDR_IND_Y},
    {0x15, ADDR_ZPG_X},
    {0x19, ADDR_ABS_Y},
    {0x1d, ADDR_ABS_X},
    // asl
    {0x6, ADDR_ZPG},
    {0xa, ADDR_A},
    {0xe, ADDR_ABS},
    {0x16, ADDR_ZPG_X},
    {0x1e, ADDR_ABS_X},
    // php
    {0x8, ADDR_IMPL},
    // bpl
    {0x10, ADDR_REL},
    // clc
    {0x18, ADDR_IMPL},
    // jsr
    {0x20, ADDR_ABS},
    // and
    {0x21, ADDR_X_IND},
    {0x25, ADDR_ZPG},
    {0x29, ADDR_IMM},
    {0x2d, ADDR_ABS},
    {0x31, ADDR_IND_Y},
    {0x35, ADDR_ZPG_X},
    {0x39, ADDR_ABS_Y},
    {0x3d, ADDR_ABS_X},
    // bit
    {0x24, ADDR_ZPG},
    {0x2c, ADDR_ABS},
    // rol
    {0x26, ADDR_ZPG},
    {0x2a, ADDR_A},
    {0x2e, ADDR_ABS},
    {0x36, ADDR_ZPG_X},
    {0x3e, ADDR_ABS_X},
    // plp
    {0x28, ADDR_IMPL},
    // bmi
    {0x30, ADDR_REL},
    // sec
    {0x38, ADDR_IMPL},
    // rti
    {0x40, ADDR_IMPL},
    // eor
    {0x41, ADDR_X_IND},
    {0x45, ADDR_ZPG},
    {0x49, ADDR_IMM},
    {0x4d, ADDR_ABS},
    {0x51, ADDR_IND_Y},
    {0x55, ADDR_ZPG_X},
    {0x59, ADDR_ABS_Y},
    {0x5d, ADDR_ABS_X},
    // lsr
    {0x46, ADDR_ZPG},
    {0x4a, ADDR_A},
    {0x4e, ADDR_ABS},
    {0x56, ADDR_ZPG_X},
    {0x5e, ADDR_ABS_X},
    // pha
    {0x48, ADDR_IMPL},
    // jmp
    {0x4c, ADDR_ABS},
    {0x6c, ADDR_IND},
    // bvc
    {0x50, ADDR_REL},
    // cli
    {0x58, ADDR_IMPL},
    // rts
    {0x60, ADDR_IMPL},
    // adc
    {0x61, ADDR_X_IND},
    {0x65, ADDR_ZPG},
    {0x69, ADDR_IMM},
    {0x6d, ADDR_ABS},
    {0x71, ADDR_IND_Y},
    {0x75, ADDR_ZPG_X},
    {0x79, ADDR_ABS_Y},
    {0x7d, ADDR_ABS_X},
    // ror
    {0x66, ADDR_ZPG},
    {0x6a, ADDR_A},
    {0x6e, ADDR_ABS},
    {0x76, ADDR_ZPG_X},
    {0x7e, ADDR_ABS_X},
    // pla
    {0x68, ADDR_IMPL},
    // bvs
    {0x70, ADDR_REL},
    // sei
    {0x78, ADDR_IMPL},
    // sta
    {0x81, ADDR_X_IND},
    {0x85, ADDR_ZPG},
    {0x8d, ADDR_ABS},
    {0x91, ADDR_IND_Y},
    {0x95, ADDR_ZPG_X},
    {0x99, ADDR_ABS_Y},
    {0x9d, ADDR_ABS_X},
    // sty
    {0x84, ADDR_ZPG},
    {0x8c, ADDR_ABS},
    {0x94, ADDR_ZPG_X},
    // stx
    {0x86, ADDR_ZPG},
    {0x8e, ADDR_ABS},
    {0x96, ADDR_ZPG_Y},
    // dey
    {0x88, ADDR_IMPL},
    // txa
    {0x8a, ADDR_IMPL},
    // bcc
    {0x90, ADDR_REL},
    // tya
    {0x98, ADDR_IMPL},
    // txs
    {0x9a, ADDR_IMPL},
    // ldy
    {0xa0, ADDR_IMM},
    {0xa4, ADDR_ZPG},
    {0xac, ADDR_ABS},
    {0xb4, ADDR_ZPG_X},
    {0xbc, ADDR_ABS_X},
    // lda
    {0xa1, ADDR_X_IND},
    {0xa5, ADDR_ZPG},
    {0xa9, ADDR_IMM},
    {0xad, ADDR_ABS},
    {0xb1, ADDR_IND_Y},
    {0xb5, ADDR_ZPG_X},
    {0xb9, ADDR_ABS_Y},
    {0xbd, ADDR_ABS_X},
    // ldx
    {0xa2, ADDR_IMM},
    {0xa6, ADDR_ZPG},
    {0xae, ADDR_ABS},
    {0xb6, ADDR_ZPG_Y},
    {0xbe, ADDR_ABS_Y},
    // tay
    {0xa8, ADDR_IMPL},
    // tax
    {0xaa, ADDR_IMPL},
    // bcs
    {0xb0, ADDR_REL},
    // clv
    {0xb8, ADDR_IMPL},
    // tsx
    {0xba, ADDR_IMPL},
    // cpy
    {0xc0, ADDR_IMM},
    {0xc4, ADDR_ZPG},
    {0xcc, ADDR_ABS},
    // cmp
    {0xc1, ADDR_X_IND},
    {0xc5, ADDR_ZPG},
    {0xc9, ADDR_IMM},
    {0xcd, ADDR_ABS},
    {0xd1, ADDR_IND_Y},
    {0xd5, ADDR_ZPG_X},
    {0xd9, ADDR_ABS_Y},
    {0xdd, ADDR_ABS_X},
    // dec
    {0xc6, ADDR_ZPG},
    {0xce, ADDR_ABS},
    {0xd6, ADDR_ZPG_X},
    {0xde, ADDR_ABS_X},
    // iny
    {0xc8, ADDR_IMPL},
    // dex
    {0xca, ADDR_IMPL},
    // bne
    {0xd0, ADDR_REL},
    // cld
    {0xd8, ADDR_IMPL},
    // cpx
    {0xe0, ADDR_IMM},
    {0xe4, ADDR_ZPG},
    {0xec, ADDR_ABS},
    // sbc
    {0xe1, ADDR_X_IND},
    {0xe5, ADDR_ZPG},
    {0xe9, ADDR_IMM},
    {0xed, ADDR_ABS},
    {0xf1, ADDR_IND_Y},
    {0xf5, ADDR_ZPG_X},
    {0xf9, ADDR_ABS_Y},
    {0xfd, ADDR_ABS_X},
    // inc
    {0xe6, ADDR_ZPG},
    {0xee, ADDR_ABS},
    {0xf6, ADDR_ZPG_X},
    {0xfe, ADDR_ABS_X},
    // inx
    {0xe8, ADDR_IMPL},
    // nop
    {0xea, ADDR_IMPL},
    // beq
    {0xf0, ADDR_REL},
    // sed
    {0xf8, ADDR_IMPL},

};
const u8 n_opcs = sizeof(ops) / sizeof(opc);

const opc *find_opc(asblock *bk, const ins *i)
{
    const mnem *mnem = find_mnem(i);

    for (u8 j = mnem->idx; j != mnem->idx + mnem->num; ++j)
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
        case ADDR_IND_Y:
            if (i->n_args == 2 &&
                i->args[0].type == ARG_MEM &&
                i->args[0].dir == BHL(1, 1) &&
                i->args[1].type == ARG_REG &&
                i->args[1].u == 2 &&
                i->args[1].dir == BHL(0, 0))
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
            i16 v = (-(bk->off + 2) + i->args[0].i);
            if (i->n_args == 1 && i->args[0].type == ARG_MEM)
            {
                // printf("Memory arg at %i, of %i (%i)\n", bk->off, i->args[0].i, v);
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
    fprintf(stderr, "No matching op for %s\n", i->name);
    return NULL;
}
