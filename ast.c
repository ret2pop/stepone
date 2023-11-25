#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

ast_t *init_ast(int type) {
  ast_t *a = calloc(1, sizeof(ast_t));
  a->type = type;
  return a;
}

/* TODO: implement this, right now it is obviously not deep copying; I might not
 * need this function at all either, who knows */
ast_t *ast_copy(ast_t *n) { return n; }

/* Recursively prints AST nodes for debug purposes */
void ast_print(ast_t *n) {
  switch (n->type) {
  case AST_ROOT:
  case AST_BLOCK:
  case AST_LOCAL:
  case AST_FUNCCALL:
    if (n->type == AST_FUNCCALL)
      printf("func name: %s\n", n->string_value->value);
    printf("Compound node:\n");
    for (int i = 0; i < n->size; i++) {
      ast_print(n->subnodes[i]);
    }
    break;
  case AST_FUNCDEF:
    for (int i = 0; i < n->size; i++) {
      ast_print(n->funcdef_params[i]);
    }
    ast_print(n->subnodes[0]);
    ast_print(n->subnodes[1]);
    break;
  case AST_VARDEC:
    printf("Variable decaration: %s, %s\n", n->type_name->value,
           n->string_value->value);
    break;
  case AST_VARDEF:
    printf("Vardef: %s\n", n->string_value->value);
    ast_print(n->subnodes[0]);
    break;
  case AST_MUL:
  case AST_DIV:
  case AST_PLUS:
  case AST_MINUS:
  case AST_MOD:
  case AST_AND:
  case AST_OR:
  case AST_LT:
  case AST_EQ:
  case AST_LTE:
  case AST_GT:
  case AST_GTE:
    printf("Operator, operating on:\n");
    ast_print(n->subnodes[0]);
    ast_print(n->subnodes[1]);
    break;
  case AST_INT:
  case AST_FLOAT:
    printf("Number: %f\n", n->num_value);
    break;
  case AST_VAR:
  case AST_STRING:
  case AST_TYPE:
    printf("Var, string, or type node: %s\n", n->string_value->value);
    break;
  case AST_IF_ELSE:
    printf("if or if-else statement\n");
    ast_print(n->subnodes[0]);
    ast_print(n->subnodes[1]);
    if (n->subnodes[2] != NULL)
      ast_print(n->subnodes[2]);

    break;
  case AST_WHILE:
    printf("while loop:\n");
    ast_print(n->subnodes[0]);
    ast_print(n->subnodes[1]);
    break;
  case AST_RETURN:
    printf("Return\n");
    ast_print(n->subnodes[0]);
  case AST_STRUCTURE:
    printf("Struct %s:\n", n->string_value->value);
    ast_print(n->subnodes[0]);
    break;
  default:
    printf("something is probably NULL\n");
  }
}

void ast_free(ast_t *n) {
  switch (n->type) {
  case AST_ROOT:
  case AST_BLOCK:
  case AST_LOCAL:
  case AST_FUNCCALL:
    for (int i = 0; i < n->size; i++) {
      printf("iteration: %d\n", i);
      ast_free(n->subnodes[i]);
    }
    free(n->subnodes);
    free(n);
    break;
  case AST_FUNCDEF:
    for (int i = 0; i < n->size; i++) {
      ast_free(n->funcdef_params[i]);
    }
    free(n->funcdef_params);
    ast_free(n->subnodes[0]);
    ast_free(n->subnodes[1]);
    free(n->subnodes);
    free(n);
    break;
  case AST_VAR:
  case AST_STRING:
    string_free(n->string_value);
    free(n);
    break;
  case AST_MUL:
  case AST_DIV:
  case AST_PLUS:
  case AST_MINUS:
  case AST_MOD:
  case AST_AND:
  case AST_OR:
  case AST_LT:
  case AST_EQ:
  case AST_LTE:
  case AST_GT:
  case AST_GTE:
    ast_free(n->subnodes[0]);
    ast_free(n->subnodes[1]);
    free(n->subnodes);
    free(n);
    break;
  case AST_VARDEC:
    string_free(n->string_value);
    string_free(n->type_name);
    free(n);
    break;
  case AST_VARDEF:
    string_free(n->string_value);
    ast_free(n->subnodes[0]);
    free(n->subnodes);
    free(n);
    break;
  case AST_INT:
  case AST_FLOAT:
    free(n);
    break;
  case AST_IF_ELSE:
    printf("if or if-else statement\n");
    ast_free(n->subnodes[0]);
    ast_free(n->subnodes[1]);
    if (n->subnodes[2] != NULL)
      ast_free(n->subnodes[2]);
    free(n->subnodes);
    free(n);
    break;
  case AST_WHILE:
    printf("while loop:\n");
    ast_free(n->subnodes[0]);
    ast_free(n->subnodes[1]);
    free(n->subnodes);
    free(n);
    break;
  case AST_STRUCTURE:
  case AST_RETURN:
    ast_free(n->subnodes[0]);
    free(n->subnodes);
    free(n);
    break;
  case AST_TYPE:
    free(n);
    break;
  default:
    free(n);
  }
}
