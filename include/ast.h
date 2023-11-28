#ifndef AST_H
#define AST_H
#include "better_string.h"

/* The Abstract Syntax Tree (AST) is a representation of the source file in a
 * more program-readable form. It encodes the file in a recursive tree format
 * where the bottom left of the tree represents jobs that need to be done first
 * and the bottom right of the tree represents the jobs that need to be done
 * last, where the layers of the tree represent which jobs need to be done in
 * order for a more abstract job to be done. */

typedef struct AST_STRUCT {
  /* All possible AST types */
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
    AST_LIST,
    AST_GLOBAL,
  } type;

  /* Based on the type of the node, these values are used differently */
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
