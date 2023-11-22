#ifndef VALIDATE_H
#define VALIDATE_H
#include "ast.h"
#include "parser.h"

typedef struct {
  ast_t *root;
  hash_table_t *func_sig;
  hash_table_t *global_sig;
} valid_t;

valid_t *init_validator(ast_t *n);

void validate(valid_t *v);

void valid_free(valid_t *v);

void validator_error(valid_t *v);
#endif
