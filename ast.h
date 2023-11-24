#ifndef AST_H
#define AST_H
#include "better_string.h"

typedef struct AST_STRUCT {
  enum {
    AST_ROOT,
    AST_BLOCK,
    AST_FUNCDEF,
    AST_VAR,
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
    AST_LT,
    AST_EQ,
    AST_LTE,
    AST_GT,
    AST_GTE,
    AST_IF_ELSE,
    AST_WHILE,
    AST_RETURN,
    AST_STRUCTURE,
    AST_LOCAL,
    AST_TYPE,
  } type;

  string_t *string_value;
  string_t *type_name;
  double num_value;

  string_t *var_name;
  string_t *type_string;

  struct AST_STRUCT **funcdef_params;
  struct AST_STRUCT **subnodes;

  struct AST_STRUCT *ast_type;

  size_t size;
  int priority;

  int pcount; /* pointer count */
  int acount; /* @ count */
} ast_t;

/* Allocates space for new Abstract Syntax Tree (AST) */
ast_t *init_ast(int type);

/* Deep copy of AST */
ast_t *ast_copy(ast_t *n);

/* Prints AST node recursively */
void ast_print(ast_t *n);

/* Recursive free of AST */
void ast_free(ast_t *n);
#endif
