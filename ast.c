#include "ast.h"

#include <stdlib.h>

ast_t *init_ast(int type) {
  ast_t *a = malloc(sizeof(ast_t));
  a->type = type;
  return a;
}
