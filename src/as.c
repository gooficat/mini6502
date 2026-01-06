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
            strcpy(bk->lb[bk->n_lb++], bk->tk);
        }
        else if (isalpha(bk->c))
        {
            ins i = parse_ins(bk);
            encode_ins(bk, i);
        }
        else
        {
            // printf("unknown %c\n", bk->c);
        }
    }
}

void add_labels(asblock *bk)
{
    while (bk->c != EOF)
    {
        if (bk->c == ':')
        {
            next_c(bk);
            get_tk(bk);
            printf("Label %s\n", bk->tk);
            strcpy(bk->lb[bk->n_lb++], bk->tk);
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
        .in = fopen("test.ps", "rt"),
        .c = fgetc(bk.in),
        .n_lb = 0,
        .off = 0,
        // .pass = PASS_MARK, // reduntant
    };

    add_labels(&bk);
    rewind(bk.in);
    bk.c = fgetc(bk.in);
    bk.pass = PASS_IMAG;
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
        a->u = find_lb(bk);
    }
}

u16 find_lb(asblock *bk)
{
    for (u16 i = 0; i != bk->n_lb; ++i)
    {
        if (!strcmp(bk->lb[i].id, bk->tk))
        {
            return bk->lb[i].off;
        }
    }
    fprintf(stderr, "No label found for %s\n", bk->tk);
    exit(EXIT_FAILURE);
}
