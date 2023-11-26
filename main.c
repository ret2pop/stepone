#include "ast.h"
#include "better_string.h"
#include "lexer.h"
#include "macros.h"
#include "parser.h"
#include "token.h"
#include "validate.h"
#include <stdio.h>
#include <stdlib.h>

void usage() { fprintf(stderr, "Usage: stepone [file]\n"); }

int main(int argc, char **argv) {
  /* READING A FILE: */
  char *buffer = 0;
  long length;

  if (argc < 2) {
    usage();
    exit(1);
  }

  FILE *f = fopen(argv[1], "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length + 1);
    if (buffer) {
      fread(buffer, 1, length, f);
    } else
      die("buffer");
    fclose(f);
  }
  buffer[length] = '\0';

  /* TEST 1: LEXER (see files lexer.h, lexer.c, token.h, token.c, macros.h,
   * macros.c, better_string.h, better_string.c) */
  /* lexer_t *l; */
  /* l = init_lexer(buffer); */
  /* token_t *t = lexer_get_next(l); */
  /* token_print_full(t); */
  /* while (t != NULL) { */
  /*   token_free(t); */
  /*   t = lexer_get_next(l); */
  /*   token_print_full(t); */
  /* } */
  /* free(l); */

  /* TEST 2: PARSER (see files ast.h, ast.c, parser.h, parser.c) */
  /* parser_t *p; */
  /* ast_t *root; */
  /* p = init_parser(buffer); */
  /* root = parse_all(p); */
  /* ast_print(root); */
  /* ast_free(root); */
  /* free(buffer); */

  /* TEST 3: VALIDATOR (see files validate.h, validate.c, hash_table.h,
   * hash_table.c) */

  parser_t *p;
  valid_t *v;
  ast_t *root;
  p = init_parser(buffer);
  root = parse_all(p);
  v = init_validator(root);
  validate(v);
  valid_free(v);

  /* TEST 4: CODEGEN */
}
