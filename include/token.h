#ifndef TOKEN_H
#define TOKEN_H
#include "better_string.h"

/* The token is what the lexer creates in order to group multi-character
 * statements such as integers, strings, and variables into a single value */
typedef struct TOKEN_STRUCT {
  /* All the possible types the lexer can create */
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

  /* we store a value if we see a multi-chracter token */
  string_t *value;
  int row;
  int col;
} token_t;

/* Allocates memory for new token */
token_t *init_token(int type, string_t *value, int row, int col);

/* For parser error messages */
char *token_to_str(token_t *token);

/* For debugging lexer */
void token_print_full(token_t *token);

/* Prints tokens */
void token_print(token_t *token);

/* Frees memory used up by tokens */
void token_free(token_t *token);

#endif
