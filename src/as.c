#include "as.h"

ins parse_ins(asblock *bk)
{
    get_tk(bk);
    ins i = {.n_args = 0};
    strcpy(i.name, bk->tk);
    for (;;)
    {
        strip_wsp(bk);

        if (isalpha(bk->c) || bk->c == EOF)
        {
            break;
        }
        get_arg(bk, &i.args[i.n_args++]);

        strip_wsp(bk);

        if (bk->c != ',')
        {
            break;
        }
        next_c(bk);
    }
    return i;
}

void encode_ins(asblock *bk, ins i);

void as_pass(asblock *bk)
{
    char next_pass = PASS_WRIT;
    bk->off = 0;
    while (bk->c != EOF)
    {
        if (isspace(bk->c))
        {
            strip_wsp(bk);
        }
        if (bk->c == ':')
        {
            next_c(bk);
            get_tk(bk);
            lab *lb = find_lb(bk);
            if (lb->off != bk->off)
            {
                printf("No match! %hu vs %hu\n", lb->off, bk->off);
                lb->off = bk->off;
                next_pass = PASS_PLAC;
            }
        }
        else if (isalpha(bk->c))
        {
            ins i = parse_ins(bk);
            encode_ins(bk, i);
        }
        else
        {
            next_c(bk);
            get_tk(bk);
            printf("directive %s\n", bk->tk);
        }
    }
    bk->pass = next_pass;
}

void add_labels(asblock *bk)
{
    while (bk->c != EOF)
    {
        if (bk->c == ':')
        {
            next_c(bk);
            get_tk(bk);
            printf("Label '%s'\n", bk->tk);
            strcpy(bk->lb[bk->n_lb].id, bk->tk);
            ++bk->n_lb;
        }
        else
        {
            next_c(bk);
        }
    }
}

int main()
{
    asblock bk = {
        .in = fopen("C:/Users/User/Documents/c/miniprose/test.ps", "rt"),
        .n_lb = 0,
        .off = 0,
        // .pass = PASS_MARK, // reduntant
    };
    bk.c = fgetc(bk.in);

    printf("Placing labels...\n");
    add_labels(&bk);

    printf("Imaginary pass...\n");
    bk.pass = PASS_IMAG;
    rewind(bk.in);
    bk.c = fgetc(bk.in);
    as_pass(&bk);

    bk.pass = PASS_PLAC;
    while (bk.pass != PASS_WRIT)
    {
        printf("Label offset placing pass...\n");
        rewind(bk.in);
        bk.c = fgetc(bk.in);
        as_pass(&bk);
    }

    bk.out = fopen("C:/Users/User/Documents/c/miniprose/test.bin", "wb");
    printf("Encoding pass...\n");
    rewind(bk.in);
    bk.c = fgetc(bk.in);
    as_pass(&bk);
}

void strip_wsp(asblock *bk)
{
    while (isspace(bk->c))
    {
        bk->c = fgetc(bk->in);
    }
}

void get_tk(asblock *bk)
{
    bk->tk_len = 0;
    while (isalnum(bk->c))
    {
        bk->tk[bk->tk_len++] = bk->c;
        bk->c = fgetc(bk->in);
    }
    bk->tk[bk->tk_len] = 0;
}

void get_arg(asblock *bk, arg *a)
{
    while (bk->c == '(')
    {
        a->dir += BIT(4);
        next_c(bk);
        strip_wsp(bk);
    }
    a->type = bk->c;
    next_c(bk);
    get_tk(bk);
    if (isdigit(bk->tk[0]))
    {
        char *eo_tk = bk->tk + bk->tk_len;
        a->u = strtoull(bk->tk, &eo_tk, 10);
    }
    else
    {
        printf("Label arg %s\n", bk->tk);
        a->u = find_lb(bk)->off;
    }
    while (bk->c == ')')
    {
        ++a->dir;
        next_c(bk);
        strip_wsp(bk);
    }
}

lab *find_lb(asblock *bk)
{
    for (u16 i = 0; i != bk->n_lb; ++i)
    {
        printf("checking if match label '%s'\n", bk->lb[i].id);
        if (!strcmp(bk->lb[i].id, bk->tk))
        {
            return &bk->lb[i];
        }
    }
    fprintf(stderr, "No label found for '%s'\n", bk->tk);
    exit(EXIT_FAILURE);
}
