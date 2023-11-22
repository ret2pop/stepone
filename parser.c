#include "parser.h"
#include "ast.h"
#include "hash_table.h"
#include "lexer.h"
#include "token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

parser_t *init_parser(char *source) {
  parser_t *p = malloc(sizeof(parser_t));
  p->l = init_lexer(source);
  p->t = lexer_get_next(p->l);
  return p;
}

void parser_move(parser_t *p) {
  if (p->t != NULL) {
    token_free(p->t);
    p->t = lexer_get_next(p->l);
  }
}

ast_t *parse_list(parser_t *p) { return NULL; }

static ast_t *parse_inner_expr(parser_t *p) {
  switch (p->t->type) {
  case TOKEN_LBRACE:
    return parse_math_expr(p);
  case TOKEN_INT:
    return parse_math_expr(p);
  case TOKEN_FLOAT:
    return parse_math_expr(p);
  case TOKEN_ID:
    return parse_var(p);
  case TOKEN_STRING:
    return parse_string(p);
  case TOKEN_LBRACKET:
    return parse_list(p);
  case TOKEN_AT:
    return parse_var(p);
  default:
    parser_error(p);
    return NULL;
  }
}

ast_t *parse_inside_block(parser_t *p) {
  if (strcmp(p->t->value->value, "if") == 0) {
    return parse_if_else(p);
  } else if (strcmp(p->t->value->value, "while") == 0) {
    return parse_while(p);
  } else if (strcmp(p->t->value->value, "return") == 0) {
    return parse_return(p);
  }

  switch (p->t->type) {
  case TOKEN_LBRACE:
    return parse_math_expr(p);
  case TOKEN_INT:
    return parse_math_expr(p);
  case TOKEN_FLOAT:
    return parse_math_expr(p);
  case TOKEN_ID:
    return parse_var(p);
  case TOKEN_STRING:
    return parse_string(p);
  case TOKEN_LBRACKET:
    return parse_list(p);
  case TOKEN_AT:
    return parse_var(p);
  default:
    parser_error(p);
    return NULL;
  }
}

ast_t *parse_var(parser_t *p) {
  ast_t *n;
  ast_t *n2;
  string_t *str;
  int atcount;
  while (p->t->type == TOKEN_AT) {
    atcount++;
    parser_move(p);
  }
  str = string_copy(p->t->value);
  parser_move(p);
  if (p->t->type == TOKEN_EQUALS) {
    n = init_ast(AST_VARDEF);
    n->string_value = str;
    n->subnodes = malloc(sizeof(ast_t *));
    n->size = 1;
    n->subnodes[0] = parse_inner_expr(p);
    n->acount = atcount;
  } else if (p->t->type == TOKEN_LPAREN) {
    if (atcount != 0) {
      parser_error(p);
    }
    parser_move(p);
    n = init_ast(AST_FUNCCALL);
    n->string_value = str;
    n->subnodes = malloc(sizeof(ast_t *));
    n->size = 0;
    while (p->t->type != TOKEN_RPAREN) {
      n2 = parse_inner_expr(p);
      if (p->t->type != TOKEN_COMMA) {
        parser_error(p);
      }
      parser_move(p);
      n->subnodes[n->size] = n2;
      n->size++;
      n->subnodes = realloc(n->subnodes, (n->size + 1) * sizeof(ast_t *));
    }
    return n;
  } else {
    n = init_ast(AST_VAR);
    n->string_value = str;
  }
  return n;
}

ast_t *parse_var_dec(parser_t *p) {
  ast_t *n = init_ast(AST_VARDEC);
  int pcount = 0;
  while (p->t->type == TOKEN_SLASH) {
    pcount++;
    parser_move(p);
  }
  if (p->t->type != TOKEN_ID && p->t->type != TOKEN_KEYWORD) {
    parser_error(p);
  }
  n->string_value = string_copy(p->t->value);
  parser_move(p);
  if (p->t->type != TOKEN_ID && p->t->type != TOKEN_KEYWORD) {
    parser_error(p);
  }
  n->type_name = string_copy(p->t->value);
  n->pcount = pcount;
  parser_move(p);
  return n;
}

ast_t *parse_block(parser_t *p) {
  ast_t *b = init_ast(AST_BLOCK);
  b->subnodes = malloc(sizeof(ast_t *));
  b->size = 0;
  while (p->t->type != TOKEN_RBRACE) {
    b->subnodes[b->size] = parse_inside_block(p);
    b->size++;
    b->subnodes = realloc(b->subnodes, (b->size + 1) * sizeof(ast_t *));
  }
  return b;
}

ast_t *parse_local(parser_t *p) {
  ast_t *n = init_ast(AST_LOCAL);
  ast_t *v;
  n->subnodes = malloc(sizeof(ast_t *));
  n->size = 0;
  parser_move(p);
  while (p->t->type != TOKEN_LBRACE) {
    v = parse_var_dec(p);
    n->subnodes[n->size] = v;
    n->size++;
    n->subnodes = realloc(n->subnodes, (n->size + 1) * sizeof(ast_t *));
  }
  parser_move(p);
  return n;
}

/* TODO: add in actual parsing */
ast_t *parse_function_dec(parser_t *p) {
  string_t *name = string_copy(p->t->value);
  ast_t *func = init_ast(AST_FUNCDEF);
  ast_t *varblock;
  ast_t *funcblock;
  int pcount = 0;
  int retpcount = 0;
  parser_move(p);
  if (p->t->type != TOKEN_COLON) {
    parser_error(p);
  }
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
  varblock = parse_local(p);
  funcblock = parse_block(p);
  return func;
}

int is_op(parser_t *p) {
  switch (p->t->type) {
  case TOKEN_PLUS:
  case TOKEN_MINUS:
  case TOKEN_MUL:
  case TOKEN_DIV:
  case TOKEN_MOD:
  case TOKEN_GTE:
  case TOKEN_LTE:
  case TOKEN_LT:
  case TOKEN_GT:
  case TOKEN_LEQUALS:
  case TOKEN_AND:
  case TOKEN_OR:
    return 1;
  default:
    return 0;
  }
}

static ast_t *op_get(parser_t *p) {
  ast_t *e;
  if (p->t->type == TOKEN_PLUS) {
    e = init_ast(AST_PLUS);
    e->priority = 2;
  } else if (p->t->type == TOKEN_MINUS) {
    e = init_ast(AST_MINUS);
    e->priority = 2;
  } else if (p->t->type == TOKEN_MUL) {
    e = init_ast(AST_MUL);
    e->priority = 1;
  } else if (p->t->type == TOKEN_DIV) {
    e = init_ast(AST_DIV);
    e->priority = 1;
  } else if (p->t->type == TOKEN_MOD) {
    e = init_ast(AST_MOD);
    e->priority = 1;
  } else if (p->t->type == TOKEN_GTE) {
    e = init_ast(AST_GTE);
    e->priority = 3;
  } else if (p->t->type == TOKEN_LTE) {
    e = init_ast(AST_GTE);
    e->priority = 3;
  } else if (p->t->type == TOKEN_LT) {
    e = init_ast(AST_LT);
    e->priority = 3;
  } else if (p->t->type == TOKEN_GT) {
    e = init_ast(AST_GT);
    e->priority = 3;
  } else if (p->t->type == TOKEN_LEQUALS) {
    e = init_ast(AST_EQ);
    e->priority = 3;
  } else if (p->t->type == TOKEN_AND) {
    e = init_ast(AST_AND);
    e->priority = 5;
  } else if (p->t->type == TOKEN_OR) {
    e = init_ast(AST_OR);
    e->priority = 4;
  } else {
    return NULL;
  }
  return e;
}

static ast_t *final_optree(ast_t **nodes, size_t size) {
  ast_t *tree;
  size_t leftsize;
  size_t rightsize;
  int priority;
  int pindex;
  if (size == 1) {
    free(nodes);
    return nodes[0];
  }

  for (int i = 0; i < size; i++) {
    if (priority < nodes[i]->priority) {
      priority = nodes[i]->priority;
      pindex = i;
    }
  }

  tree = nodes[pindex];
  leftsize = pindex;
  rightsize = size - pindex - 1;

  tree->subnodes = malloc(2 * sizeof(ast_t *));
  ast_t **left = malloc(leftsize * sizeof(ast_t *));
  for (int i = 0; i < leftsize; i++) {
    left[i] = nodes[i];
  }
  tree->subnodes[0] = final_optree(left, leftsize);

  ast_t **right = malloc(rightsize * sizeof(ast_t *));
  for (int i = 0; i < rightsize; i++) {
    right[i] = nodes[pindex + i + 1];
  }
  tree->subnodes[1] = final_optree(right, rightsize);

  free(nodes);
  return tree;
}

ast_t *parse_math_expr(parser_t *p) {
  /* number of possible operators and then the expressions */
  ast_t **exprs = malloc(sizeof(ast_t *));
  size_t num_exprs;

  double d;
  ast_t *n;
  ast_t *o;

  if (p->t->type == TOKEN_LPAREN) {
    parser_move(p);
    n = parse_math_expr(p);
    if (p->t->type != TOKEN_RPAREN) {
      parser_error(p);
    }
  } else if (p->t->type == TOKEN_INT || p->t->type == TOKEN_FLOAT) {
    if (p->t->type == TOKEN_INT)
      n = init_ast(AST_INT);
    else
      n = init_ast(AST_FLOAT);
    n->num_value = atof(p->t->value->value);
    n->priority = 0;
  } else if (p->t->type == TOKEN_ID) {
    n = init_ast(AST_VAR);
    n->priority = 0;
  } else {
    parser_error(p);
  }

  parser_move(p);
  exprs[0] = n;
  num_exprs++;

  while (is_op(p)) {
    o = op_get(p);
    exprs = realloc(exprs, (num_exprs + 2) * sizeof(ast_t *));
    exprs[num_exprs] = o;
    num_exprs++;
    parser_move(p);

    if (p->t->type == TOKEN_LPAREN) {
      parser_move(p);
      n = parse_math_expr(p);
      n->priority = 0;
      if (p->t->type != TOKEN_RPAREN) {
        parser_error(p);
      }
    } else if (p->t->type == TOKEN_INT || p->t->type == TOKEN_FLOAT) {
      if (p->t->type == TOKEN_INT)
        n = init_ast(AST_INT);
      else
        n = init_ast(AST_FLOAT);
      n->num_value = atof(p->t->value->value);
      n->priority = 0;
    } else if (p->t->type == TOKEN_ID) {
      n = init_ast(AST_VAR);
      n->priority = 0;
    } else {
      parser_error(p);
    }

    parser_move(p);
    exprs = realloc(exprs, (num_exprs + 2) * sizeof(ast_t *));
    exprs[num_exprs] = n;
    num_exprs++;

    parser_move(p);
  }

  n = final_optree(exprs, num_exprs);
  free(exprs);
  return n;
}

/* TODO: return string ast node that properly handles escapes */
ast_t *parse_string(parser_t *p) { return NULL; }

ast_t *parse_if_else(parser_t *p) {
  ast_t *n = init_ast(AST_IF_ELSE);
  ast_t *n2;
  ast_t *b;
  ast_t *l;
  parser_move(p);
  if (p->t->type != TOKEN_LPAREN) {
    parser_error(p);
  }
  l = parse_math_expr(p);
  if (p->t->type != TOKEN_RPAREN) {
    parser_error(p);
  }
  b = parse_block(p);
  if (strcmp(p->t->value->value, "else") == 0) {
    parser_move(p);
    if (strcmp(p->t->value->value, "if") == 0) {
      n2 = parse_if_else(p);
    } else if (p->t->type == TOKEN_LBRACE) {
      n2 = parse_block(p);
    }
    n2 = NULL;
  }
  n->subnodes = malloc(3 * sizeof(ast_t *));
  n->subnodes[0] = l;
  n->subnodes[1] = b;
  n->subnodes[2] = n2;
  return n;
}

ast_t *parse_while(parser_t *p) {
  ast_t *n = init_ast(AST_WHILE);
  ast_t *b;
  parser_move(p);
  if (p->t->type != TOKEN_LPAREN) {
    parser_error(p);
  }
  ast_t *logic = parse_math_expr(p);
  if (p->t->type != TOKEN_RPAREN) {
    parser_error(p);
  }
  parser_move(p);
  b = parse_block(p);
  n->subnodes = malloc(2 * sizeof(ast_t *));
  n->subnodes[0] = logic;
  n->subnodes[1] = b;
  n->size = 1;
  return n;
}

ast_t *parse_return(parser_t *p) {
  ast_t *ret = init_ast(AST_RETURN);
  parser_move(p);
  ast_t *expr = parse_inner_expr(p);
  ret->subnodes = malloc(sizeof(ast_t *));
  ret->subnodes[0] = expr;
  parser_move(p);
  return ret;
}

ast_t *parse_struct(parser_t *p) {
  parser_move(p);
  ast_t *n = init_ast(AST_STRUCTURE);
  n->subnodes = malloc(sizeof(ast_t *));
  n->string_value = string_copy(p->t->value);
  parser_move(p);
  n->subnodes[0] = parse_local(p);
  return n;
}

ast_t *parse_expr(parser_t *p) {
  if (p->t == NULL)
    return NULL;
  if (p->t->type == TOKEN_KEYWORD) {
    if (strcmp(p->t->value->value, "struct") == 0) {
      return parse_struct(p);
    } else if (strcmp(p->t->value->value, "func") == 0) {
      return parse_function_dec(p);
    } else {
      parser_error(p);
    }
  }
  switch (p->t->type) {
  case TOKEN_TYPE:
    return parse_var_dec(p);
  case TOKEN_SLASH:
    return parse_var_dec(p);
  case TOKEN_LBRACE:
    return parse_math_expr(p);
  case TOKEN_INT:
    return parse_math_expr(p);
  case TOKEN_FLOAT:
    return parse_math_expr(p);
  case TOKEN_ID:
    return parse_var(p);
  case TOKEN_STRING:
    return parse_string(p);
  case TOKEN_LBRACKET:
    return parse_list(p);
  case TOKEN_AT:
    return parse_var(p);
  default:
    parser_error(p);
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

void parser_error(parser_t *p) {
  fprintf(stderr, "lmao what the fuck did you do\n");
}
