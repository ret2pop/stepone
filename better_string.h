#ifndef BETTER_STRING_H
#define BETTER_STRING_H
#include <stdlib.h>
/* We want better strings to realloc less and keep track of the length of the
 * string so we don't have to call strlen every time */
typedef struct STRING_STRUCT {
  /* length of string */
  size_t length;
  /* Size of current value buffer */
  size_t bufsize;
  char *value;
} string_t;

/* Allocates memory for new string */
string_t *init_string(char *value);

/* Copies string to another string */
string_t *string_copy(string_t *s);

/* Concatenates a string_t type and a char * type */
void string_concat(string_t *s1, char *s2);

/* Appends single characters */
void string_append(string_t *s, char c);

/* Frees space for string */
void string_free(string_t *s);
#endif
