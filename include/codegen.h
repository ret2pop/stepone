#ifndef CODEGEN_H
#define CODEGEN_H
#include "ast.h"
#include "better_string.h"
/* The code generator stores the generated code in a string struct; it will
 * generate to an IR and then that IR will generate to multiple backends */
typedef struct {
  ast_t *root;
  ast_t *cur;
  string_t *s;
} gen_t;

/* Allocates memory for new generator */
gen_t *init_gen(ast_t *root);

/* Generates all the code */
void generate(gen_t *g);

/* Frees the generator, the AST, and the generated string */
void gen_free(gen_t *g);

#endif
