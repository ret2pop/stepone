#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

ast_t *init_ast(int type) {
  ast_t *a = malloc(sizeof(ast_t));
  a->type = type;
  return a;
}

/* TODO: implement these */
ast_t *ast_copy(ast_t *n) { return n; }

void ast_print(ast_t *n) {
  switch (n->type) {
  default:
    printf("lol\n");
  }
}

void ast_free(ast_t *n) {
  if (n->string_value != NULL) {
    string_free(n->string_value);
  }
  switch (n->type) {
  case AST_ROOT:
  case AST_BLOCK:
    for (int i = 0; i < n->size; i++) {
      ast_free(n->subnodes[i]);
    }
    free(n);
  default:
    free(n);
  }
}
