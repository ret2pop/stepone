#include "parser.h"
#include "ast.h"
#include "hash_table.h"
#include "lexer.h"
#include "token.h"
#include <stdbool.h>
#include <stdlib.h>

parser_t *init_parser(char *source) {
  parser_t *p = malloc(sizeof(parser_t));
  p->l = init_lexer(source);
  p->t = lexer_get_next(p->l);
  p->stable = init_hash_table(500);
  p->ftable = init_hash_table(500);
  return p;
}

void parser_move(parser_t *p) {
  if (p->t != NULL) {
    token_free(p->t);
    p->t = lexer_get_next(p->l);
  }
}

ast_t *parse_var_dec(parser_t *p) {}

ast_t *parse_function_dec(parser_t *p) {
  string_t *name = string_copy(p->t->value);
  ast_t *func = init_ast(AST_FUNCDEF);
  ast_t *funcblock;
  int pcount = 0;
  int retpcount = 0;
  parser_move(p);
  while (p->t->type != TOKEN_ARROW && p->t->type != TOKEN_LBRACE) {
    while (p->t->type == TOKEN_SLASH) {
      pcount++;
      parser_move(p);
    }
    if (p->t->type != TOKEN_TYPE) {
      parser_error(p);
    }
    parser_move(p);
    if (p->t->type != TOKEN_ID) {
      parser_error(p);
    }
    parser_move(p);
    if (p->t->type != TOKEN_COMMA) {
      parser_error(p);
    }
    parser_move(p);
  }
  if (p->t->type == TOKEN_ARROW) {
    parser_move(p);
    while (p->t->type == TOKEN_SLASH) {
      retpcount++;
      parser_move(p);
    }
    if (p->t->type != TOKEN_TYPE) {
      parser_error(p);
    }
    parser_move(p);
  }
  if (p->t->type != TOKEN_LBRACE) {
    parser_error(p);
  }
  parser_move(p);
  while (p->t->type != TOKEN_RBRACE) {
  }
  parser_move(p);
  if (p->t->type != TOKEN_LBRACE) {
    parser_error(p);
  }
  funcblock = parse_block(p);
  return func;
}

ast_t *parse_math_expr(parser_t *p) {
  ast_t *e1;
  ast_t *e2;
  ast_t *e3;
  ast_t *e4;
  ast_t *e5;
  int p1;
  int p2;
  if (p->t->type == TOKEN_LPAREN) {
    parser_move(p);
    e1 = parse_math_expr(p);
    if (p->t->type != TOKEN_RPAREN) {
      parser_error(p);
    }
    parser_move(p);
    return e1;
  } else if (p->t->type == TOKEN_INT || p->t->type == TOKEN_FLOAT) {
    double f = atof(p->t->value->value);
    if (p->t->type == TOKEN_INT) {
      e1 = init_ast(AST_INT);
    } else {
      e1 = init_ast(AST_FLOAT);
    }
    e1->num_value = f;
    parser_move(p);

    if (p->t->type == TOKEN_PLUS) {
      e2 = init_ast(AST_PLUS);
      p1 = 2;
    } else if (p->t->type == TOKEN_MINUS) {
      e2 = init_ast(AST_MINUS);
      p1 = 2;
    } else if (p->t->type == TOKEN_MUL) {
      e2 = init_ast(AST_MUL);
      p1 = 1;
    } else if (p->t->type == TOKEN_DIV) {
      e2 = init_ast(AST_DIV);
      p1 = 1;
    } else if (p->t->type == TOKEN_MOD) {
      e2 = init_ast(AST_MOD);
      p1 = 1;
    } else {
      return e1;
    }
    parser_move(p);
    e3 = parse_math_expr(p);
    if (p->t->type == TOKEN_PLUS) {
      e4 = init_ast(AST_PLUS);
      p2 = 2;
    } else if (p->t->type == TOKEN_MINUS) {
      e4 = init_ast(AST_MINUS);
      p2 = 2;
    } else if (p->t->type == TOKEN_MUL) {
      e4 = init_ast(AST_MUL);
      p2 = 1;
    } else if (p->t->type == TOKEN_DIV) {
      e4 = init_ast(AST_DIV);
      p2 = 1;
    } else if (p->t->type == TOKEN_MOD) {
      e4 = init_ast(AST_MOD);
      p2 = 1;
    } else {
      e2->subnodes = malloc(2 * sizeof(ast_t *));
      e2->size = 2;
      e2->subnodes[0] = e1;
      e2->subnodes[1] = e3;
      return e2;
    }
    parser_move(p);
    e5 = parse_math_expr(p);
    if (p1 == 2) {
      e2->subnodes = malloc(2 * sizeof(ast_t *));
      e2->subnodes[0] = e1;
      e4->subnodes = malloc(2 * sizeof(ast_t *));
      e4->subnodes[0] = e3;
      e4->subnodes[1] = e5;
      e2->subnodes[1] = e4;
      return e2;
    } else {
      e2->subnodes = malloc(2 * sizeof(ast_t *));
      e2->subnodes[0] = e1;
      e2->subnodes[1] = e3;
      e4->subnodes = malloc(2 * sizeof(ast_t *));
      e4->subnodes[0] = e2;
      e4->subnodes[1] = e5;
      return e4;
    }
  } else {
    parser_error(p);
  }
  return NULL;
}

ast_t *parse_string(parser_t *p) { return NULL; }

ast_t *parse_expr(parser_t *p) {
  switch (p->t->type) {
  case TOKEN_TYPE:
    return parse_var(p);
  case TOKEN_LBRACE:
    return parse_math_expr(p);
  case TOKEN_INT:
    return parse_math_expr(p);
  case TOKEN_FLOAT:
    return parse_math_expr(p);
  case TOKEN_ID:
    return parse_function_dec(p);
  case TOKEN_STRING:
    return parse_string(p);
  default:
    return NULL;
  }
}

ast_t *parse_all(parser_t *p) {
  ast_t *root = init_ast(AST_ROOT);
  root->subnodes = malloc(sizeof(ast_t *));
  int i = 0;
  while (p->t != NULL) {
    root->subnodes[i] = parse_expr(p);
    i++;
    root->subnodes = realloc(root->subnodes, (i + 1) * sizeof(ast_t *));
  }
  root->subnodes[i] = NULL;
  return root;
}

void parser_error(parser_t *p) {}
