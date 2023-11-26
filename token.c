#include "token.h"
#include "better_string.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

token_t *init_token(int type, string_t *value, int row, int col) {
  token_t *t = malloc(sizeof(token_t));
  if (t == NULL)
    die("malloc on init_token");
  t->type = type;
  t->value = value;
  t->row = row;
  t->col = col;
  return t;
}

char *token_to_str(token_t *token) {
  switch (token->type) {
  case TOKEN_LPAREN:
    return "(";
  case TOKEN_RPAREN:
    return ")";
  case TOKEN_LBRACE:
    return "{";
  case TOKEN_RBRACE:
    return "}";
  case TOKEN_PERIOD:
    return ".";
  case TOKEN_SEMI:
    return ";";
  case TOKEN_ID:
    return "ID";
  case TOKEN_INT:
    return "INT";
  case TOKEN_FLOAT:
    return "FLOAT";
  case TOKEN_HEX:
    return "HEX INT";
  case TOKEN_TYPE:
    return "TYPE";
  case TOKEN_PLUS:
    return "+";
  case TOKEN_MINUS:
    return "-";
  case TOKEN_MUL:
    return "*";
  case TOKEN_DIV:
    return "/";
  case TOKEN_MOD:
    return "%";
  case TOKEN_EQUALS:
    return "=";
  case TOKEN_LEQUALS:
    return "==";
  case TOKEN_ARROW:
    return "=>";
  case TOKEN_EXCLAM:
    return "!";
  case TOKEN_LT:
    return "<";
  case TOKEN_GT:
    return ">";
  case TOKEN_LTE:
    return "<=";
  case TOKEN_GTE:
    return ">=";
  case TOKEN_AND:
    return "&&";
  case TOKEN_OR:
    return "||";
  case TOKEN_CARROT:
    return "^";
  case TOKEN_KEYWORD:
    return "KEYWORD";
  case TOKEN_STRING:
    return "STRING";
  case TOKEN_AT:
    return "@";
  case TOKEN_COMMA:
    return ",";
  case TOKEN_COLON:
    return ":";
  case TOKEN_LBRACKET:
    return "[";
  case TOKEN_RBRACKET:
    return "]";
  case TOKEN_SLASH:
    return "\\";
  }
  return NULL;
}

void token_print_full(token_t *token) {
  if (token == NULL)
    return;
  if (token->value == NULL)
    fprintf(stderr, "%s\n", token_to_str(token));
  else
    fprintf(stderr, "%s: %s\n", token_to_str(token), token->value->value);
}
void token_print(token_t *token) {
  if (token == NULL)
    return;
  /* Single character tokens do not have a value */
  if (token->value != NULL)
    fprintf(stderr, "%s\n", token->value->value);
}

void token_free(token_t *token) {
  if (token->value != NULL) {
    string_free(token->value);
  }
  free(token);
}
