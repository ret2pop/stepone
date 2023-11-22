#ifndef SOPP_H
#define SOPP_H

typedef struct {
  char *source;
  int linenum;
  int i;
  char *newstring;
} sopp_t;

sopp_t *init_sopp(char *source);

void sopp_process(sopp_t *pp);
#endif
