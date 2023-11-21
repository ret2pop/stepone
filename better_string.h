#ifndef BETTER_STRING_H
#define BETTER_STRING_H
#include <stdlib.h>

typedef struct STRING_STRUCT {
  size_t length;
  size_t bufsize;
  char *value;
} string_t;

string_t *init_string(char *value);

string_t *string_copy(string_t *s);

void string_concat(string_t *s1, char *s2);

void string_append(string_t *s, char c);

void string_free(string_t *s);
#endif
