#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "hash_table.h"
#include "lexer.h"

typedef struct PARSER_STRUCT {
  lexer_t *l;
  token_t *t;
  hash_table_t *stable;
  hash_table_t *ftable;
  hash_table_t *ttable;
} parser_t;

parser_t *init_parser(char *source);

void parser_move(parser_t *p);

ast_t *parse_int(parser_t *p);

ast_t *parse_var_dec(parser_t *p);

ast_t *parse_var(parser_t *p);

ast_t *parse_function(parser_t *p);

ast_t *parse_function_dec(parser_t *p);

ast_t *parse_block(parser_t *p);

ast_t *parse_string(parser_t *p);

ast_t *parse_expr(parser_t *p);

ast_t *parse_all(parser_t *p);

void parser_error(parser_t *p);
#endif
