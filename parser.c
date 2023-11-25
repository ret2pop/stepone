#include "parser.h"
#include "ast.h"
#include "hash_table.h"
#include "lexer.h"
#include "macros.h"
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
  if (p->t->value != NULL) {
    if (strcmp(p->t->value->value, "if") == 0) {
      return parse_if_else(p);
    } else if (strcmp(p->t->value->value, "while") == 0) {
      return parse_while(p);
    } else if (strcmp(p->t->value->value, "return") == 0) {
      return parse_return(p);
    }
  }

  switch (p->t->type) {
  case TOKEN_LBRACE:
    return parse_math_expr(p);
  case TOKEN_LPAREN:
  case TOKEN_INT:
    return parse_math_expr(p);
  case TOKEN_FLOAT:
    return parse_math_expr(p);
  case TOKEN_SLASH:
  case TOKEN_ID:
  case TOKEN_AT:
    return parse_var(p);
  case TOKEN_STRING:
    return parse_string(p);
  case TOKEN_LBRACKET:
    return parse_list(p);
  case TOKEN_SEMI:
    parser_move(p);
    return parse_inside_block(p);
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
  int pcount;
  while (p->t->type == TOKEN_AT) {
    atcount++;
    parser_move(p);
  }
  if (atcount > 0 && p->t->type == TOKEN_SLASH) {
    parser_error(p);
  }
  while (p->t->type == TOKEN_SLASH) {
    pcount++;
    parser_move(p);
  }
  if (pcount > 0 && p->t->type == TOKEN_AT) {
    parser_error(p);
  }

  str = string_copy(p->t->value);
  parser_move(p);
  if (p->t->type == TOKEN_EQUALS && atcount == 0) {
    n = init_ast(AST_VARDEF);
    n->string_value = str;
    n->subnodes = malloc(sizeof(ast_t *));
    if (n->subnodes == NULL)
      die("malloc on subnodes");
    n->size = 1;
    n->subnodes[0] = parse_inner_expr(p);
    if (n->subnodes[0]->type == AST_VARDEF) {
      parser_error(p);
    }

    n->pcount = pcount;
  } else if (p->t->type == TOKEN_LPAREN) {
    if (atcount != 0) {
      parser_error(p);
    }
    parser_move(p);
    n = init_ast(AST_FUNCCALL);
    n->string_value = str;
    n->subnodes = malloc(sizeof(ast_t *));
    if (n->subnodes == NULL)
      die("malloc on subnodes");
    n->size = 0;
    while (p->t->type != TOKEN_RPAREN) {
      n2 = parse_inner_expr(p);
      if (n2->type == AST_VARDEF)
        parser_error(p);

      if (p->t->type != TOKEN_COMMA && p->t->type != TOKEN_RPAREN) {
        parser_error(p);
      }
      if (p->t->type != TOKEN_RPAREN)
        parser_move(p);
      n->subnodes[n->size] = n2;
      n->size++;
      n->subnodes = realloc(n->subnodes, (n->size + 1) * sizeof(ast_t *));
    }
    parser_move(p);
  } else {
    n = init_ast(AST_VAR);
    n->string_value = str;
    n->acount = atcount;
    n->pcount = pcount;
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
  if (p->t->type != TOKEN_ID && p->t->type != TOKEN_TYPE) {
    parser_error(p);
  }
  n->type_name = string_copy(p->t->value);
  parser_move(p);
  if (p->t->type != TOKEN_ID) {
    parser_error(p);
  }
  n->string_value = string_copy(p->t->value);
  n->pcount = pcount;
  parser_move(p);
  return n;
}

ast_t *parse_block(parser_t *p) {
  ast_t *b = init_ast(AST_BLOCK);
  b->subnodes = malloc(sizeof(ast_t *));
  if (b->subnodes == NULL)
    die("malloc on subnodes");
  b->size = 0;
  parser_move(p);
  while (p->t->type != TOKEN_RBRACE) {
    b->subnodes[b->size] = parse_inside_block(p);
    b->size++;
    b->subnodes = realloc(b->subnodes, (b->size + 1) * sizeof(ast_t *));
  }
  parser_move(p);
  return b;
}

ast_t *parse_local(parser_t *p) {
  ast_t *n = init_ast(AST_LOCAL);
  ast_t *v;
  n->subnodes = malloc(sizeof(ast_t *));
  if (n->subnodes == NULL)
    die("malloc on subnodes");
  n->size = 0;
  parser_move(p);
  while (p->t->type != TOKEN_RBRACE) {
    v = parse_var_dec(p);
    n->subnodes[n->size] = v;
    n->size++;
    n->subnodes = realloc(n->subnodes, (n->size + 1) * sizeof(ast_t *));
  }
  parser_move(p);
  return n;
}

ast_t *parse_function_dec(parser_t *p) {
  parser_move(p);
  string_t *name = string_copy(p->t->value);
  ast_t *func = init_ast(AST_FUNCDEF);
  ast_t *varblock;
  ast_t *funcblock;
  ast_t *n;
  ast_t *n2;
  int pcount = 0;
  parser_move(p);
  if (p->t->type != TOKEN_COLON) {
    parser_error(p);
  }

  parser_move(p);
  func->funcdef_params = malloc(sizeof(ast_t *));
  if (func->funcdef_params == NULL)
    die("malloc on subnodes");
  func->size = 0;

  while (p->t->type != TOKEN_ARROW && p->t->type != TOKEN_LBRACE) {
    n = parse_var_dec(p);
    func->funcdef_params[func->size] = n;
    func->size++;
    func->funcdef_params =
        realloc(func->funcdef_params, (1 + func->size) * sizeof(ast_t *));
  }

  if (p->t->type == TOKEN_ARROW) {
    parser_move(p);
    if (p->t->type != TOKEN_TYPE && p->t->type != TOKEN_SLASH &&
        p->t->type != TOKEN_ID) {
      printf("entering error\n");
      parser_error(p);
    }
    while (p->t->type == TOKEN_SLASH) {
      parser_move(p);
      pcount++;
    }
    if (p->t->type != TOKEN_TYPE && p->t->type != TOKEN_ID) {
      parser_error(p);
    }
    n2 = init_ast(AST_TYPE);
    n2->string_value = string_copy(p->t->value);
    n->ast_type = n;
    parser_move(p);
  }
  if (p->t->type != TOKEN_LBRACE) {
    parser_error(p);
  }
  func->subnodes = malloc(2 * sizeof(ast_t *));
  if (func->subnodes == NULL)
    die("malloc on subnodes");
  varblock = parse_local(p);
  funcblock = parse_block(p);
  func->subnodes[0] = varblock;
  func->subnodes[1] = funcblock;
  func->string_value = name;
  return func;
}

bool is_op(parser_t *p) {
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
    return true;
  default:
    return false;
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

/* This constructs the final operator tree from parse_math_expr */
static ast_t *final_optree(ast_t **nodes, size_t size) {
  ast_t *tree;
  size_t leftsize = 0;
  size_t rightsize = 0;
  int priority = 0;
  int pindex = 0;
  if (size == 1) {
    tree = nodes[0];
    free(nodes);
    return tree;
  }

  for (int i = 0; i < size; i++) {
    if (priority <= nodes[i]->priority) {
      priority = nodes[i]->priority;
      pindex = i;
    }
  }

  tree = nodes[pindex];
  leftsize = pindex;
  rightsize = size - pindex - 1;

  tree->subnodes = calloc(2, sizeof(ast_t *));
  if (tree->subnodes == NULL)
    die("calloc on subnodes");
  ast_t **left = calloc(leftsize, sizeof(ast_t *));
  if (left == NULL)
    die("calloc on left");
  for (int i = 0; i < leftsize; i++) {
    left[i] = nodes[i];
  }
  tree->subnodes[0] = final_optree(left, leftsize);

  ast_t **right = calloc(rightsize, sizeof(ast_t *));
  if (right == NULL)
    die("calloc on right");

  for (int i = 0; i < rightsize; i++) {
    right[i] = nodes[pindex + i + 1];
  }

  tree->subnodes[1] = final_optree(right, rightsize);
  free(nodes);
  return tree;
}

ast_t *parse_math_expr(parser_t *p) {
  /* number of possible operators and then the expressions */
  ast_t **exprs = calloc(1, sizeof(ast_t *));
  if (exprs == NULL)
    die("calloc on exprs");
  size_t num_exprs = 0;

  ast_t *n;
  ast_t *o;

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
  }

  n = final_optree(exprs, num_exprs);
  return n;
}

/* TODO: return string ast node that properly handles escapes */
ast_t *parse_string(parser_t *p) {
  ast_t *n = init_ast(AST_STRING);
  n->string_value = string_copy(p->t->value);
  parser_move(p);
  return n;
}

/* TODO: Implement this */
ast_t *parse_list(parser_t *p) { return NULL; }

ast_t *parse_if_else(parser_t *p) {
  ast_t *n = init_ast(AST_IF_ELSE);
  ast_t *n2;
  ast_t *b;
  ast_t *l;
  parser_move(p);
  l = parse_math_expr(p);
  b = parse_block(p);
  if (p->t->value != NULL) {
    if (strcmp(p->t->value->value, "else") == 0) {
      parser_move(p);
      if (strcmp(p->t->value->value, "if") == 0) {
        n2 = parse_if_else(p);
      } else if (p->t->type == TOKEN_LBRACE) {
        n2 = parse_block(p);
      } else {
        parser_error(p);
      }
    } else
      n2 = NULL;
  } else
    n2 = NULL;

  n->subnodes = malloc(3 * sizeof(ast_t *));
  if (n->subnodes == NULL)
    die("malloc on subnodes");
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
  if (n->subnodes == NULL)
    die("malloc on subnodes");
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
  ret->size = 1;
  if (ret->subnodes == NULL)
    die("malloc on subnodes");
  ret->subnodes[0] = expr;
  return ret;
}

ast_t *parse_struct(parser_t *p) {
  parser_move(p);
  ast_t *n = init_ast(AST_STRUCTURE);
  n->subnodes = malloc(sizeof(ast_t *));
  if (n->subnodes == NULL)
    die("malloc on subnodes");
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
    } else if (strcmp(p->t->value->value, "global") == 0) {
      parser_move(p);
      return parse_var_dec(p);
    } else {
      parser_error(p);
    }
  }
  switch (p->t->type) {
  case TOKEN_SLASH:
    return parse_var_dec(p);
  case TOKEN_LBRACE:
    return parse_math_expr(p);
  case TOKEN_INT:
    return parse_math_expr(p);
  case TOKEN_FLOAT:
    return parse_math_expr(p);
  case TOKEN_ID:
    return parse_var(
        p); /* TODO: make it so that this can also be a var_dec in the end */
  case TOKEN_STRING:
    return parse_string(p);
  case TOKEN_LBRACKET:
    return parse_list(p);
  case TOKEN_AT:
    return parse_var(p);
  case TOKEN_SEMI:
    parser_move(p);
    return parse_expr(p);
  default:
    parser_error(p);
    return NULL;
  }
}

ast_t *parse_all(parser_t *p) {
  ast_t *root = init_ast(AST_ROOT);
  root->subnodes = malloc(sizeof(ast_t *));
  if (root->subnodes == NULL)
    die("malloc on subnodes");
  root->size = 0;
  while (p->t != NULL) {
    root->subnodes[root->size] = parse_expr(p);
    root->size++;
    root->subnodes =
        realloc(root->subnodes, (root->size + 1) * sizeof(ast_t *));
  }
  free(p->l);
  free(p);
  return root;
}

void parser_error(parser_t *p) {
  printf("On line %d, column %d:\n", p->t->row, p->t->col);
  if (p->t->value != NULL)
    fprintf(stderr, "%sPARSER ERROR: Unexpected Token of type `%s`: %s%s\n",
            RED, token_to_str(p->t), p->t->value->value, reset);
  else
    fprintf(stderr, "%sPARSER ERROR: Unexpected Token of type `%s`%s\n", RED,
            token_to_str(p->t), reset);
  token_free(p->t);
  free(p->l);
  free(p);
  exit(1);
}
