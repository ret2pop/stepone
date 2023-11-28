#include <macros.h>
#include <parser.h>
#include <validate.h>

#include <stdio.h>
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

void validate_list(valid_t *v) {}
void validate_math_expr(valid_t *v) {}

void validate_dec(valid_t *v) {}

void validate_struct(valid_t *v) {}

void validate_dec_block(valid_t *v) {}

void validate_if_else(valid_t *v) {}

void validate_while(valid_t *v) {
  ast_t *w = v->cur;
  v->cur = w->subnodes[0];
  validate_math_expr(v);
  v->cur = w->subnodes[1];
  validate_block(v);
}

void validate_inner_expr(valid_t *v) {
  switch (v->cur->type) {
  case AST_PLUS:
  case AST_MINUS:
  case AST_MUL:
  case AST_DIV:
  case AST_LT:
  case AST_LTE:
  case AST_GT:
  case AST_GTE:
  case AST_EQ:
  case AST_MOD:
  case AST_INT:
  case AST_FLOAT:
  case AST_VAR:
    validate_math_expr(v);
  case AST_STRING:
    return;
  case AST_LIST:
    validate_list(v);
  default:
    printf("lmao\n");
  }
}

void validate_vardef(valid_t *v) {
  ast_t *vardef = v->cur;
  v->cur = vardef->subnodes[0];
  validate_inner_expr(v);
}

void validate_block_statement(valid_t *v) {
  switch (v->cur->type) {
  case AST_IF_ELSE:
    validate_if_else(v);
    break;
  case AST_WHILE:
    validate_while(v);
    break;
  case AST_VARDEF:
    validate_vardef(v);
    break;
  default:
    printf("oof\n");
  }
}

void validate_block(valid_t *v) {
  for (int i = 0; i < v->cur->size; i++) {
    validate_block_statement(v);
  }
}

void validate_funcdef(valid_t *v) {
  ast_t *funcdef = v->cur;
  hash_table_add(v->func_sig, v->cur->string_value, v->cur);
  /* add all the variable declarations to the local_sig hash table */
  v->cur = v->cur->subnodes[2];
  validate_block(v);
  v->local_sig = NULL;
}

void validate_expr(valid_t *v) {
  switch (v->cur->type) {
  case AST_FUNCDEF:
    validate_funcdef(v);
    break;
  case AST_STRUCTURE:
    validate_struct(v);
    break;
  default:
    validator_error(v);
  }
}

void validate(valid_t *v) {
  for (int i = 0; i < v->root->size; i++) {
    v->cur = v->root->subnodes[i];
    validate_expr(v);
  }
  v->cur = v->root;
}

void valid_free(valid_t *v) {
  hash_table_free(v->func_sig);
  hash_table_free(v->global_sig);
  hash_table_free(v->local_sig);
  free(v);
}

void validator_error(valid_t *v) {
  fprintf(stderr, "Current node:\n");
  ast_print(v->cur);
  fprintf(stderr, "%sVALIDATOR ERROR: semantic error on ast node%s\n", RED,
          reset);
  exit(1);
}
