#include "validate.h"
#include "parser.h"

#include <stdlib.h>

valid_t *init_validator(ast_t *root) {
  valid_t *v = malloc(sizeof(valid_t));
  v->root = root;
  v->cur = root;
  v->func_sig = init_hash_table(500);
  v->global_sig = init_hash_table(500);
  v->local_sig = init_hash_table(500);
  return v;
}

void validate_expr(valid_t *v) {}

void validate(valid_t *v) {
  for (int i = 0; i < v->root->size; i++) {
    v->cur = v->root->subnodes[i];
    validate_expr(v);
  }
}

void valid_free(valid_t *v) {
  hash_table_free(v->func_sig);
  hash_table_free(v->global_sig);
  hash_table_free(v->local_sig);
  ast_free(v->root);
  free(v);
}
