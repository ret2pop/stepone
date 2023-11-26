#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "ast.h"
#include "better_string.h"
#include <stdlib.h>

/* This is the node for the singly linked list that is being used as the buckets
 * for the hash table */
typedef struct SL_NODE {
  ast_t *value;
  string_t *name;
  struct SL_NODE *next;
} sl_node_t;

/* Each singly linked list is a bucket in the hash table */
typedef struct {
  sl_node_t *head;
  size_t size;
} sl_list_t;

/* The hash table will store a list of signatures in the hash table that will
 * allow the validator to check for type and parameter consistency. */
typedef struct {
  sl_list_t **buckets;
  size_t size;
} hash_table_t;

/* Allocates memory for new singly linked list node */
sl_node_t *init_sl_node(string_t *name, ast_t *value);

/* Allocates memory for new singly linked list node */
sl_list_t *init_sl_list();

/* Adds value to back of singly linked list */
void sl_list_add(sl_list_t *l, string_t *name, ast_t *n);

/* Gets value by name from singly linked list */
ast_t *sl_list_get(sl_list_t *l, string_t *name);

/* Frees singly linked list but does not free the ast node */
void sl_list_free(sl_list_t *l);

/* Allocates memory for new hash table */
hash_table_t *init_hash_table(size_t size);

/* Adds value by key to hash table */
void hash_table_add(hash_table_t *h, string_t *name, ast_t *value);

/* Gets value from hash table by key */
ast_t *hash_table_get(hash_table_t *h, string_t *name);

/* Frees hash table and buckets */
void hash_table_free(hash_table_t *h);

/* DJB2 hash function taken from online */
unsigned long hash(char *str);
#endif
