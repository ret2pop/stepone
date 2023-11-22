#include "token.h"
#include "better_string.h"
#include <stdio.h>
#include <stdlib.h>

token_t *init_token(int type, string_t *value, int row, int col) {
  token_t *t = malloc(sizeof(token_t));
  t->type = type;
  t->value = value;
  t->row = row;
  t->col = col;
  return t;
}

void token_print(token_t *token) {
  if (token == NULL)
    return;
  if (token->value == NULL)
    printf("%d\n", token->type);
  else
    printf("%d, %s\n", token->type, token->value->value);
}

void token_free(token_t *token) {
  if (token->value != NULL) {
    string_free(token->value);
  }
  free(token);
}
