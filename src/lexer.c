#include <better_string.h>
#include <lexer.h>
#include <macros.h>
#include <token.h>

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer_t *init_lexer(char *source) {
  lexer_t *lexer = malloc(sizeof(lexer_t));
  if (lexer == NULL)
    die("malloc in init_lexer");
  lexer->source = source;
  lexer->i = 0;
  lexer->c = source[0];
  lexer->row = 1;
  lexer->col = 1;
  return lexer;
}

void lexer_move(lexer_t *lexer) {
  if (lexer->c != '\0') {
    lexer->i++;
    lexer->col++;
    lexer->c = lexer->source[lexer->i];
    if (lexer->c == '\n') {
      /* When we see a newline character, we are entering a new row, and we are
       * returing back to the first column */
      lexer->row++;
      lexer->col = 1;
    }
  }
}

void lexer_skip_whitespace(lexer_t *lexer) {
  while (isspace(lexer->c)) {
    lexer_move(lexer);
  }
}

token_t *lexer_create_token(lexer_t *lexer, int type, string_t *value) {
  return init_token(type, value, lexer->row, lexer->col);
}

token_t *lexer_move_with_token(lexer_t *lexer, int type, string_t *value) {
  token_t *ret = lexer_create_token(lexer, type, value);
  lexer_move(lexer);
  return ret;
}
token_t *lexer_create_id(lexer_t *lexer) {
  string_t *str = init_string(NULL);
  char keywords[6][7] = {"if", "else", "while", "return", "struct", "func"};

  char types[16][15] = {"int8",   "int16",  "int32", "uint8",
                        "uint16", "uint32", "float", "double",
                        "string", "int",    "void"};

  while (isalnum(lexer->c)) {
    string_append(str, lexer->c);
    lexer_move(lexer);
  }

  /* We want to figure out if this id is actually a keyword or type */
  for (int i = 0; i < 6; i++) {
    if (strcmp(keywords[i], str->value) == 0) {
      return lexer_create_token(lexer, TOKEN_KEYWORD, str);
    }
  }

  for (int i = 0; i < 16; i++) {
    if (strcmp(types[i], str->value) == 0) {
      return lexer_create_token(lexer, TOKEN_TYPE, str);
    }
  }
  return lexer_create_token(lexer, TOKEN_ID, str);
}

token_t *lexer_create_hex(lexer_t *lexer) {
  string_t *str = init_string(NULL);
  while (isxdigit(lexer->c)) {
    string_append(str, lexer->c);
    lexer_move(lexer);
  }
  return lexer_create_token(lexer, TOKEN_HEX, str);
}

token_t *lexer_create_number(lexer_t *lexer) {
  string_t *str = init_string(NULL);
  bool is_float = false;
  if (lexer->c == '0') {
    lexer_move(lexer);
    if (lexer->c == 'x') {
      /* 0x prefix means it is a hex number */
      lexer_move(lexer);
      return lexer_create_hex(lexer);
    }
  }

  while (isdigit(lexer->c) || ((lexer->c == '.') && !is_float)) {
    if (lexer->c == '.')
      is_float = true;
    string_append(str, lexer->c);
    lexer_move(lexer);
  }

  if (is_float) {
    return lexer_create_token(lexer, TOKEN_FLOAT, str);
  }
  return lexer_create_token(lexer, TOKEN_INT, str);
}

token_t *lexer_create_string(lexer_t *lexer) {
  string_t *str = init_string(NULL);
  lexer_move(lexer);
  while (lexer->c != '"') {
    if (lexer->c == '\0')
      lexer_error(lexer);
    string_append(str, lexer->c);
    lexer_move(lexer);
  }
  lexer_move(lexer);
  return lexer_create_token(lexer, TOKEN_STRING, str);
}

/* We want to figure out if the next character makes it an arrow, an ==, or an =
 */
token_t *lexer_create_equals(lexer_t *lexer) {
  lexer_move(lexer);
  if (lexer->c == '>') {
    return lexer_move_with_token(lexer, TOKEN_ARROW, NULL);
  } else if (lexer->c == '=') {
    return lexer_move_with_token(lexer, TOKEN_LEQUALS, NULL);
  }
  return lexer_create_token(lexer, TOKEN_EQUALS, NULL);
}

/* helper functions to determine if something is > or >= (< or <= as well) */
static token_t *lexer_gt(lexer_t *lexer) {
  lexer_move(lexer);
  if (lexer->c == '=') {
    return lexer_move_with_token(lexer, TOKEN_GTE, NULL);
  }
  return lexer_create_token(lexer, TOKEN_GT, NULL);
}

static token_t *lexer_lt(lexer_t *lexer) {
  lexer_move(lexer);
  if (lexer->c == '=') {
    return lexer_move_with_token(lexer, TOKEN_LTE, NULL);
  }
  return lexer_create_token(lexer, TOKEN_LT, NULL);
}

token_t *lexer_get_next(lexer_t *lexer) {
  if (isspace(lexer->c)) {
    lexer_skip_whitespace(lexer);
  }
  if (isalpha(lexer->c)) {
    return lexer_create_id(lexer);
  } else if (isdigit(lexer->c)) {
    return lexer_create_number(lexer);
  }
  switch (lexer->c) {
  case '"':
    return lexer_create_string(lexer);
  case '<':
    return lexer_lt(lexer);
  case '>':
    return lexer_gt(lexer);
  case '=':
    return lexer_create_equals(lexer);

    /* One character tokens */
  case '%':
    return lexer_move_with_token(lexer, TOKEN_MOD, NULL);
  case '+':
    return lexer_move_with_token(lexer, TOKEN_PLUS, NULL);
  case '-':
    return lexer_move_with_token(lexer, TOKEN_MINUS, NULL);
  case '*':
    return lexer_move_with_token(lexer, TOKEN_MUL, NULL);
  case '!':
    return lexer_move_with_token(lexer, TOKEN_EXCLAM, NULL);
  case '/':
    return lexer_move_with_token(lexer, TOKEN_DIV, NULL);
  case '(':
    return lexer_move_with_token(lexer, TOKEN_LPAREN, NULL);
  case ')':
    return lexer_move_with_token(lexer, TOKEN_RPAREN, NULL);
  case '{':
    return lexer_move_with_token(lexer, TOKEN_LBRACE, NULL);
  case '}':
    return lexer_move_with_token(lexer, TOKEN_RBRACE, NULL);
  case '@':
    return lexer_move_with_token(lexer, TOKEN_AT, NULL);
  case ';':
    return lexer_move_with_token(lexer, TOKEN_SEMI, NULL);
  case ',':
    return lexer_move_with_token(lexer, TOKEN_COMMA, NULL);
  case '[':
    return lexer_move_with_token(lexer, TOKEN_LBRACKET, NULL);
  case ']':
    return lexer_move_with_token(lexer, TOKEN_RBRACKET, NULL);
  case ':':
    return lexer_move_with_token(lexer, TOKEN_COLON, NULL);
  case '\\':
    return lexer_move_with_token(lexer, TOKEN_SLASH, NULL);
  }
  return NULL;
}

void lexer_error(lexer_t *lexer) {
  printf("on line %d, column %d:\n", lexer->row, lexer->col);
  fprintf(stderr,
          "%sError: EOF reached in lexer before finished tokenizing "
          "stage; Perhaps you did not close a string?\n%s",
          RED, reset);

  free(lexer);
  exit(1);
}
