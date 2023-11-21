#ifndef LEXER_H_
#define LEXER_H_
#include "better_string.h"
#include "token.h"
typedef struct LEXER_STRUCT {
  char *source;
  int i;
  char c;
  int row;
  int col;
} lexer_t;

lexer_t *init_lexer(char *source);

void lexer_move(lexer_t *lexer);

void lexer_skip_whitespace(lexer_t *lexer);

token_t *lexer_create_id(lexer_t *lexer);

token_t *lexer_create_hex(lexer_t *lexer);

token_t *lexer_create_number(lexer_t *lexer);

token_t *lexer_create_string(lexer_t *lexer);

token_t *lexer_create_equals(lexer_t *lexer);

token_t *lexer_create_token(lexer_t *lexer, int type, string_t *value);

token_t *lexer_move_with_token(lexer_t *lexer, int type, string_t *value);

token_t *lexer_get_next(lexer_t *lexer);

void lexer_error(lexer_t *lexer);
#endif
