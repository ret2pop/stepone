#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "ast.h"
#include "better_string.h"
#include <stdlib.h>

typedef struct SL_NODE {
  ast_t *value;
  string_t *name;
  struct SL_NODE *next;
} sl_node_t;

typedef struct {
  sl_node_t *head;
  size_t size;
} sl_list_t;

typedef struct {
  sl_list_t **buckets;
  size_t size;
} hash_table_t;

sl_node_t *init_sl_node(string_t *name, ast_t *value);

sl_list_t *init_sl_list();

void sl_list_add(sl_list_t *l, string_t *name, ast_t *n);

ast_t *sl_list_get(sl_list_t *l, string_t *name);

void sl_list_free(sl_list_t *l);

hash_table_t *init_hash_table(size_t size);

void hash_table_add(hash_table_t *h, string_t *name, ast_t *value);

ast_t *hash_table_get(hash_table_t *h, string_t *name);

void hash_table_free(hash_table_t *h);

unsigned long hash(char *str);
#endif
