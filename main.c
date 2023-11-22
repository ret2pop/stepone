#include "ast.h"
#include "better_string.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "validate.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  lexer_t *l;

  /* TEST 1: LEXER (see files lexer.h, lexer.c, token.h, token.c, macros.h,
   * macros.c, better_string.h, better_string.c) */
  char *source = "int main: int argc, string argv { "
                 "returnaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaaaaaaaa 0; }";
  l = init_lexer(source);
  token_t *t = lexer_get_next(l);
  token_print(t);
  while (t != NULL) {
    token_free(t);
    t = lexer_get_next(l);
    token_print(t);
  }
  free(l);

  /* TEST 2: PARSER */
  /* parser_t *p; */
  /* ast_t *root; */
  /* p = init_parser("main: int argc, \\\\string argv => {} { return 0 }"); */
  /* root = parse_all(p); */
  /* ast_print(root); */
  /* ast_free(root); */

  /* TEST 3: VALIDATOR */
  /* parser_t *p; */
  /* valid_t *v; */
  /* ast_t *root; */
  /* p = init_parser("main: int argc, \\\\string argv => { return 0 }"); */
  /* root = parse_all(p); */
  /* v = init_validator(root); */
  /* validate(v); */
  /* valid_free(v); */

  /* TEST 4: CODEGEN */
}
