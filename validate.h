#ifndef VALIDATE_H
#define VALIDATE_H
#include "ast.h"
#include "parser.h"

typedef struct {
  ast_t *root;
  ast_t *cur_node;
  hash_table_t *func_sig;
  hash_table_t *global_sig;
} valid_t;

/* Allocates memory for new validator */
valid_t *init_validator(ast_t *n);

/* Validates the root node */
void validate(valid_t *v);

/* free validator and associated hash tables */
void valid_free(valid_t *v);

/* Semantic errors */
void validator_error(valid_t *v);
#endif
