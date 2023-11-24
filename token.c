#include "token.h"
#include "better_string.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

token_t *init_token(int type, string_t *value, int row, int col) {
  token_t *t = malloc(sizeof(token_t));
  if (t == NULL)
    die("malloc on init_token");
  t->type = type;
  t->value = value;
  t->row = row;
  t->col = col;
  return t;
}

void token_print(token_t *token) {
  if (token == NULL)
    return;
  /* Single character tokens do not have a value */
  if (token->value == NULL)
    fprintf(stderr, "%d\n", token->type);
  else
    fprintf(stderr, "%d, %s\n", token->type, token->value->value);
}

void token_free(token_t *token) {
  if (token->value != NULL) {
    string_free(token->value);
  }
  free(token);
}
