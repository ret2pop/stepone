#ifndef TOKEN_H
#define TOKEN_H
#include "better_string.h"

typedef struct TOKEN_STRUCT {
  enum {
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_PERIOD,
    TOKEN_SEMI,
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_HEX,
    TOKEN_TYPE,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_MOD,
    TOKEN_EQUALS,
    TOKEN_LEQUALS,
    TOKEN_ARROW,
    TOKEN_EXCLAM,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LTE,
    TOKEN_GTE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_CARROT,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_AT,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_SLASH,
  } type;

  string_t *value;
  int row;
  int col;
} token_t;

token_t *init_token(int type, string_t *value, int row, int col);

void token_print(token_t *token);

void token_free(token_t *token);
#endif
