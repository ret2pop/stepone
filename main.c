#include "better_string.h"
#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  /* TEST 1: LEXER */
  char *source = "int main: int argc, string argv { return 0; }";
  lexer_t *l = init_lexer(source);
  token_t *t = lexer_get_next(l);
  token_print(t);
  while (t != NULL) {
    token_free(t);
    t = lexer_get_next(l);
    token_print(t);
  }
  free(l);
}
