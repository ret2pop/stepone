#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "hash_table.h"
#include "lexer.h"

/* The parser generates the AST node continuously requesting tokens and making
 * correlations */
typedef struct PARSER_STRUCT {
  lexer_t *l;
  token_t *t;
} parser_t;

/* Allocates space for new parser */
parser_t *init_parser(char *source);

/* Moves the parser */
void parser_move(parser_t *p);

/* Function to parse expressions that appear inside blocks */
ast_t *parse_inside_block(parser_t *p);

/* Parse variable declaration */
ast_t *parse_var_dec(parser_t *p);

/* Parse what looks like a variable -- might be a function call */
ast_t *parse_var(parser_t *p);

ast_t *parse_var_assign(parser_t *p);

/* Parse function declaration -- defining the parameters, return value, and
 * other stuff */
ast_t *parse_function_dec(parser_t *p);

/* Parses if statement */
ast_t *parse_if_else(parser_t *p);

/* Parses while loop */
ast_t *parse_while(parser_t *p);

/* Parses struct */
ast_t *parse_struct(parser_t *p);

/* Parses return statement */
ast_t *parse_return(parser_t *p);

/* parses blocks after if statements, while statements, functions, etc... */
ast_t *parse_block(parser_t *p);

/* Parses expressions allowed in blocks */
ast_t *parse_block_expr(parser_t *p);

/* parses local variables for functions and definitions for structs */
ast_t *parse_local(parser_t *p);

/* Parse string and do the correct escape sequence */
ast_t *parse_string(parser_t *p);

/* parses infix mathematics, logic, etc... */
ast_t *parse_math_expr(parser_t *p);

/* Parses an expression -- this then parses something more specific */
ast_t *parse_expr(parser_t *p);

/* Parses array/list */
ast_t *parse_list(parser_t *p);

/* Returns the root AST node */
ast_t *parse_all(parser_t *p);

/* Error out if parser encounters token that doesn't make sense */
void parser_error(parser_t *p);

#endif
