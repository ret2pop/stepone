#include "ast.h"

#include <stdlib.h>

ast_t *init_ast(int type) {
  ast_t *a = malloc(sizeof(ast_t));
  a->type = type;
  return a;
}

ast_t *ast_copy(ast_t *n) { return n; }

void ast_print(ast_t *n) {}

void ast_free(ast_t *n) {}
