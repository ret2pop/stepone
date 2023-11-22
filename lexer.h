#ifndef LEXER_H_
#define LEXER_H_
#include "better_string.h"
#include "token.h"

/* The purpose of the lexer is to loop through the source file in order to group
 * together certain multi-character symbols into single tokens. For consistency,
 * one-character tokens are also allowed. */
typedef struct LEXER_STRUCT {
  /* The source file that is written in our language */
  char *source;
  /* The current character and current index */
  int i;
  char c;
  /* We want to keep track of the row and column for giving error reports to the
   * using during parsing stage */
  int row;
  int col;
} lexer_t;

/* Allocates memory for new lexer */
lexer_t *init_lexer(char *source);

/* Increments the current character position */
void lexer_move(lexer_t *lexer);

/* Whitespace doesn't matter in our language; we want to skip it when we see it
 */
void lexer_skip_whitespace(lexer_t *lexer);

/* This collects any word and stores it in a single token */
token_t *lexer_create_id(lexer_t *lexer);

/* Collect hex number */
token_t *lexer_create_hex(lexer_t *lexer);

/* Collect number */
token_t *lexer_create_number(lexer_t *lexer);

/* Collects string */
token_t *lexer_create_string(lexer_t *lexer);

/* Collects either an arrow, a == operator, or an = */
token_t *lexer_create_equals(lexer_t *lexer);

/* Creates token with lexer row and col parameters */
token_t *lexer_create_token(lexer_t *lexer, int type, string_t *value);

/* We don't want to get stuck in a loop of seeing the same token over and over
 * again */
token_t *lexer_move_with_token(lexer_t *lexer, int type, string_t *value);

/* The main function that we call; it simply gets the next token */
token_t *lexer_get_next(lexer_t *lexer);

/* the lexer can see something that doesn't make sense */
void lexer_error(lexer_t *lexer);

#endif
