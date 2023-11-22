#include "validate.h"
#include "parser.h"

#include <stdlib.h>

valid_t *init_validator(ast_t *root) {
  valid_t *v = malloc(sizeof(valid_t));
  v->root = root;
  return v;
}
