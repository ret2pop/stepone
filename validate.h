#ifndef VALIDATE_H
#define VALIDATE_H
#include "ast.h"
#include "parser.h"

/* The validator ensures that the correct types are used and the correct amount
 * of parameters are used for functions */
typedef struct {
  ast_t *root;
  ast_t *cur;
  hash_table_t *func_sig;
  hash_table_t *global_sig;
  hash_table_t *local_sig;
} valid_t;

/* Allocates memory for new validator */
valid_t *init_validator(ast_t *n);

/* Validate expression */
void validate_expr(valid_t *v);

/* Validates the root node */
void validate(valid_t *v);

/* free validator and associated hash tables */
void valid_free(valid_t *v);

/* Semantic errors */
void validator_error(valid_t *v);
#endif
