#ifndef AST_H
#define AST_H
#include "better_string.h"
typedef struct AST_STRUCT {
  enum {
    AST_ROOT,
    AST_BLOCK,
    AST_FUNCDEF,
    AST_FUNCPARAMS,
    AST_FUNCCALL,
    AST_MUL,
    AST_DIV,
    AST_PLUS,
    AST_MINUS,
    AST_MOD,
    AST_AND,
    AST_OR,
    AST_VARDEC,
    AST_VARDEF,
    AST_STRING,
    AST_INT,
    AST_FLOAT,
    AST_MATHEXPR,
  } type;

  string_t *string_value;
  double num_value;

  string_t *var_name;
  string_t *type_string;

  struct AST_STRUCT **funcdef_params;
  struct AST_STRUCT **subnodes;

  struct AST_STRUCT *ast_type;

  size_t size;
} ast_t;

ast_t *init_ast(int type);

ast_t *ast_copy(ast_t *n);

void ast_free(ast_t *n);
#endif
