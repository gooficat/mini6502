#include "as.h"

union ui parse_arith(asblock *bk)
{
    char *n = bk->tk;
    union ui out;
    if (bk->tk[0] == '(')
    {
        next_c(bk);
        strip_wsp(bk);
        //
    }
    if (isalpha(bk->tk[0]))
    {
        out.u = find_lb(bk)->off;
    }
    else
    {
        i8 radix;
        char *eo_n = bk->tk + bk->tk_len;

        if (bk->tk_len > 2)
        {
            switch (bk->tk[1])
            {
            case 'x':
                radix = 16;
                break;
            case 'b':
                radix = 2;
                break;
            case 'o':
                radix = 8;
                break;
            default:
                goto deci;
            }

            n += 2;
        }
        else
        {
        deci:;
            radix = 10;
        }

        out.i = strtol(n, &eo_n, radix);
        // printf("Val %hi\n", out.i);
    }
    strip_wsp(bk);
    if (bk->c != EOF)
    {
        char sign = bk->c;
        switch (bk->c)
        {
        case '+':
            next_c(bk);
            strip_wsp(bk);
            out.i += parse_arith(bk).i;
            break;
        case '-':
            next_c(bk);
            strip_wsp(bk);
            out.i -= parse_arith(bk).i;
            break;
        case '*':
            next_c(bk);
            strip_wsp(bk);
            out.i *= parse_arith(bk).i;
            break;
        case '/':
            next_c(bk);
            strip_wsp(bk);
            out.i /= parse_arith(bk).i;
            break;
        case '%':
            next_c(bk);
            strip_wsp(bk);
            out.i %= parse_arith(bk).i;
            break;
        case '^':
            next_c(bk);
            strip_wsp(bk);
            out.i ^= parse_arith(bk).i;
            break;
        case '&':
            next_c(bk);
            strip_wsp(bk);
            out.i &= parse_arith(bk).i;
            break;
        case '|':
            next_c(bk);
            strip_wsp(bk);
            out.i |= parse_arith(bk).i;
            break;
        default:
            break;
        }
    }

    return out;
}

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
    // printf("Instruction ends at %c (%s)\n", bk->c, bk->tk);
    return i;
}

void encode_ins(asblock *bk, ins i);

void handle_dir(asblock *bk)
{
    next_c(bk);
    get_tk(bk);

    char name[LAB_MAX];
    strcpy(name, bk->tk);

    strip_wsp(bk);
    get_tk(bk);

    if (!strcmp(name, "org"))
    {
        bk->off += parse_arith(bk).u;
        // printf("Org %hu\n", bk->off);
    }
    else if (!strcmp(name, "align"))
    {
        umax n = parse_arith(bk).u;
        u8 v = parse_arith(bk).u;
        if (bk->pass == PASS_WRIT)
        {
            while (bk->off != n)
            {
                fputc(v, bk->out);
                ++bk->off;
            }
        }
    }
    else if (!strcmp(name, "defnum"))
    {
        strcpy(bk->lb[bk->n_lb], bk->tk);
        get_tk(bk);
        bk->lb[bk->n_lb].off = parse_arith(bk).u;
        bk->n_lb++;
    }
    else if (!strcmp(name, "byte"))
    {
        u8 val = parse_arith(bk).u;
        ++bk->off;
        if (bk->pass == PASS_WRIT)
        {
            fputc(val & 0xFF, bk->out);
        }
    }
    else if (!strcmp(name, "dbyte"))
    {
        u16 val = parse_arith(bk).u;
        bk->off += 2;
        if (bk->pass == PASS_WRIT)
        {
            fputc(val & 0xFF, bk->out);
            fputc((val >> 8) & 0xFF, bk->out);
        }
    }
}

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
                // printf("No match! %hu vs %hu\n", lb->off, bk->off);
                lb->off = bk->off;
                next_pass = PASS_PLAC;
            }
        }
        else if (isalpha(bk->c))
        {
            ins i = parse_ins(bk);
            encode_ins(bk, i);
        }
        else // if (bk->c == '.')
        {
            handle_dir(bk);
        }
        // else
        // {
        //     // printf("Unexpected character %c, followed by %c in %s\n", bk->c, bk->tk);
        //     exit(EXIT_FAILURE);
        // }
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
            // printf("Label '%s'\n", bk->tk);
            strcpy(bk->lb[bk->n_lb].id, bk->tk);
            ++bk->n_lb;
        }
        else
        {
            next_c(bk);
        }
    }
}

int main(int argc, char *argv[])
{
    assert(argc == 3 && "Error! Incorrect args! Correct args are /path/to/mini6502 <input file> <output file>\n");

    asblock bk = {
        .in = fopen(argv[1], "rt"),
        .n_lb = 0,
        .off = 0,
        // .pass = PASS_MARK, // reduntant
    };
    bk.c = fgetc(bk.in);

    // printf("Placing labels...\n");
    add_labels(&bk);

    // printf("Imaginary pass...\n");
    bk.pass = PASS_IMAG;
    rewind(bk.in);
    bk.c = fgetc(bk.in);
    as_pass(&bk);

    bk.pass = PASS_PLAC;
    while (bk.pass != PASS_WRIT)
    {
        // printf("Label offset placing pass...\n");
        rewind(bk.in);
        bk.c = fgetc(bk.in);
        as_pass(&bk);
    }

    bk.out = fopen(argv[2], "wb");
    // printf("Encoding pass...\n");
    rewind(bk.in);
    bk.c = fgetc(bk.in);
    as_pass(&bk);

    fclose(bk.in);
    fclose(bk.out);
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
    while (bk->c == '[')
    {
        a->dir += BIT(4);
        next_c(bk);
        strip_wsp(bk);
    }
    a->type = bk->c;
    next_c(bk);
    get_tk(bk);
    // printf("Digit %s\n", bk->tk);
    a->i = parse_arith(bk).i;
    while (bk->c == ']')
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
        // printf("checking if match label '%s'\n", bk->lb[i].id);
        if (!strcmp(bk->lb[i].id, bk->tk))
        {
            return &bk->lb[i];
        }
    }
    fprintf(stderr, "No label found for '%s'\n", bk->tk);
    exit(EXIT_FAILURE);
}
