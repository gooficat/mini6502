#pragma once

#include "defs.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LAB 128
#define LAB_MAX 16
#define MAX_ARG 3
#define TOK_MAX 8

#define BIT(n) (1 << (n))
#define BHL(a, b) ((a << 4) | (b))

enum
{
    PASS_MARK,
    PASS_IMAG,
    PASS_PLAC,
    PASS_WRIT,
};

enum
{
    ARG_IMM = '#',
    ARG_MEM = '$',
    ARG_REG = '%',
};

union ui
{
    umax u;
    imax i;
};

struc(arg)
{
    char type;
    union ui;
    char op;
    arg *app;
    u8 dir;
};

struc(ins)
{
    char name[TOK_MAX];
    arg args[MAX_ARG];
    u8 n_args;
};

struc(lab)
{
    char id[LAB_MAX];
    umax off;
};

struc(asblock)
{
    FILE *in;
    i16 c;
    char tk[TOK_MAX];
    u8 tk_len;
    lab lb[MAX_LAB];
    u16 n_lb;
    umax off;
    FILE *out;
    u8 pass;
};

void strip_wsp(asblock *bk);
#define next_c(bk) (bk)->c = fgetc((bk)->in)
void get_tk(asblock *bk);

lab *find_lb(asblock *bk);

void get_arg(asblock *bk, arg *a);
