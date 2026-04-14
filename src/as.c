#include "as.h"
#include <ctype.h>
#include <string.h>

bool is_op(const char c) {
  switch (c) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '|':
  case '^':
  case '&':
    return true;
  default:
    return false;
  }
}

imax char_arith(imax a, const char o, imax b) {
  switch (o) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/':
    return a / b;
  case '%':
    return a % b;
  case '|':
    return a | b;
  case '^':
    return a ^ b;
  case '&':
    return a & b;
  }
  fprintf(stderr, "Failed to perform arithmetic. %c is not an operator\n", o);
  exit(EXIT_FAILURE);
}

imax num_tok(asblock *bk) {

  umax v = strtoul(bk->tk, NULL, 0);

  return v;
}

imax comp_arith(asblock *bk);

imax parse_const(asblock *bk) {
  imax base;
  strip_wsp(bk);
  printf("Starting base at '%s' '%c', (%ld)\n", bk->tk, bk->c, ftell(bk->in));

  if (!bk->tk[0]) {
    if (bk->c == '+') {
      next_c(bk);
      strip_wsp(bk);
    }
    if (bk->c == '-') {
      strip_wsp(bk);
      get_tk(bk);
      base = -num_tok(bk); // this looks janky imo
    } else if (bk->c == '(') {
      printf("paren start %c\n", bk->tk[0]);
      next_c(bk);
      strip_wsp(bk);
      base = comp_arith(bk);
      strip_wsp(bk);
      next_c(bk);
    } else if (isdigit(bk->c)) {
      strip_wsp(bk);
      get_tk(bk);
      goto num;
    }
  } else if (isdigit(bk->tk[0])) {
  num:
    base = num_tok(bk);
    strip_wsp(bk);
  } else if (isalpha(bk->tk[0])) {
    lab *lb = find_lb(bk);
    base = lb->off;
    strip_wsp(bk);
  } else {
  err:
    printf("bk is '%c' (at %li), tk is '%s'\n", bk->tk[0], ftell(bk->in),
           bk->tk);
  }
  return base;
}

imax comp_arith(asblock *bk) {
  printf("Parsing constant...\n");
  imax base = parse_const(bk);
  while (is_op(bk->c)) {
    char op = bk->c;
    strip_wsp(bk);
    get_tk(bk);
    imax operand = parse_const(bk);
    base = char_arith(base, op, operand);
    strip_wsp(bk);
  }
  printf("Returning %hd, c is %c at %li\n", base, bk->c, ftell(bk->in));
  return base;
}

ins parse_ins(asblock *bk) {
  get_tk(bk);
  ins i = {.n_args = 0};
  strcpy(i.name, bk->tk);
  for (;;) {
    strip_wsp(bk);

    if (isalpha(bk->c) || bk->c == EOF) {
      break;
    }
    get_arg(bk, &i.args[i.n_args++]);

    strip_wsp(bk);

    if (bk->c != ',') {
      break;
    }
    next_c(bk);
  }
  // printf("Instruction ends at %c (%s)\n", bk->c, bk->tk);
  return i;
}

void encode_ins(asblock *bk, ins i);

void handle_dir(asblock *bk) {
  next_c(bk);
  get_tk(bk);

  char name[LAB_MAX];
  strcpy(name, bk->tk);

  strip_wsp(bk);
  get_tk(bk);

  if (!strcmp(name, "org")) {
    printf("Org %s\n", bk->tk);
    bk->off = comp_arith(bk);
  } else if (!strcmp(name, "align")) {
    umax n = comp_arith(bk);
    u8 v = comp_arith(bk);
    if (bk->pass == PASS_WRIT) {
      while (bk->off != n) {
        fputc(v, bk->out);
        ++bk->off;
      }
    }
  } else if (!strcmp(name, "defnum")) {
    printf("Defnum %s\n", bk->tk);
    strcpy(bk->lb[bk->n_lb].id, bk->tk);
    strip_wsp(bk);
    get_tk(bk);
    printf("Defnum %s\n", bk->tk);
    bk->lb[bk->n_lb].off = comp_arith(bk);
    bk->n_lb++;
  } else if (!strcmp(name, "byte")) {
    printf("Byte at %s\n", bk->tk);
    if (bk->c != '"') {
      u8 val = comp_arith(bk);
      ++bk->off;
      if (bk->pass == PASS_WRIT) {
        fputc(val & 0xFF, bk->out);
        printf("Put %hhu\n", val);
      }
    } else {
      next_c(bk);
      if (bk->pass == PASS_WRIT) {
        while (bk->c != '"') {
          fputc(bk->c & 0xFF, bk->out);
          printf("Put c '%c'\n", bk->c);
          next_c(bk);
          ++bk->off;
        }
        next_c(bk);
      } else {
        while (bk->c != '"') {
          next_c(bk);
          ++bk->off;
        }
        next_c(bk);
      }
    }
  } else if (!strcmp(name, "dbyte")) {
    u16 val = comp_arith(bk);
    bk->off += 2;
    if (bk->pass == PASS_WRIT) {
      fputc(val & 0xFF, bk->out);
      fputc((val >> 8) & 0xFF, bk->out);
    }
  }
}

void as_pass(asblock *bk) {
  char next_pass = PASS_WRIT;
  bk->off = 0;
  while (bk->c != EOF) {
    if (isspace(bk->c)) {
      strip_wsp(bk);
    }
    if (bk->c == ':') {
      next_c(bk);
      get_tk(bk);
      lab *lb = find_lb(bk);
      if (lb->off != bk->off) {
        // printf("No match! %hu vs %hu\n", lb->off, bk->off);
        lb->off = bk->off;
        next_pass = PASS_PLAC;
      }
    } else if (isalpha(bk->c)) {
      ins i = parse_ins(bk);
      encode_ins(bk, i);
    } else // if (bk->c == '.')
    {
      handle_dir(bk);
    }
    // else
    // {
    //     // printf("Unexpected character %c, followed by %c in %s\n", bk->c,
    //     bk->tk); exit(EXIT_FAILURE);
    // }
  }
  bk->pass = next_pass;
}

void add_labels(asblock *bk) {
  while (bk->c != EOF) {
    if (bk->c == ':') {
      next_c(bk);
      get_tk(bk);
      // printf("Label '%s'\n", bk->tk);
      strcpy(bk->lb[bk->n_lb].id, bk->tk);
      ++bk->n_lb;
    } else {
      next_c(bk);
    }
  }
}
#define in_arg argv[1]
// argv[1]
#define out_arg argv[2]
// argv[2]

int main(int argc, char *argv[]) {
  // assert(argc == 3 && "Error! Incorrect args! Correct args are
  // /path/to/mini6502 <input file> <output file>\n");

  asblock bk = {
      .in = fopen(in_arg, "rt"),
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
  while (bk.pass != PASS_WRIT) {
    // printf("Label offset placing pass...\n");
    rewind(bk.in);
    bk.c = fgetc(bk.in);
    as_pass(&bk);
  }

  bk.out = fopen(out_arg, "wb");
  // printf("Encoding pass...\n");
  rewind(bk.in);
  bk.c = fgetc(bk.in);
  as_pass(&bk);

  fclose(bk.in);
  fclose(bk.out);
}

void strip_wsp(asblock *bk) {
  while (isspace(bk->c)) {
    bk->c = fgetc(bk->in);
  }
}

void get_tk(asblock *bk) {
  bk->tk_len = 0;
  while (isalnum(bk->c)) {
    bk->tk[bk->tk_len++] = bk->c;
    bk->c = fgetc(bk->in);
  }
  bk->tk[bk->tk_len] = 0;
}

void get_arg(asblock *bk, arg *a) {
  while (bk->c == '[') {
    a->dir += BIT(4);
    next_c(bk);
    strip_wsp(bk);
  }
  a->type = bk->c;
  next_c(bk);
  get_tk(bk);
  // printf("Digit %s\n", bk->tk);
  a->i = comp_arith(bk);
  while (bk->c == ']') {
    ++a->dir;
    next_c(bk);
    strip_wsp(bk);
  }
}

lab *find_lb(asblock *bk) {
  for (u16 i = 0; i != bk->n_lb; ++i) {
    // printf("checking if match label '%s'\n", bk->lb[i].id);
    if (!strcmp(bk->lb[i].id, bk->tk)) {
      return &bk->lb[i];
    }
  }
  fprintf(stderr, "No label found for '%s'\n", bk->tk);
  exit(EXIT_FAILURE);
}
