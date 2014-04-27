#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>

typedef struct node node;
typedef struct list list;
typedef struct hashtable hashtable;
typedef struct env_hashtable env_hashtable;

struct hashtable
{
    list **items;
    size_t m, z;
};

struct env_hashtable
{
    env_hashtable *root, *parent;
    hashtable *t;
    size_t z, c;
};

hashtable *hashtable_new();
void hashtable_insert(hashtable *s, char *k, void *v);
void *hashtable_find(hashtable *s, char *k);
void hashtable_remove(hashtable *s, char *k);

env_hashtable *env_hashtable_new();
void env_hashtable_insert(env_hashtable *s, char *k, void *v);
void *env_hashtable_find(env_hashtable *s, char *k);
void env_hashtable_remove(env_hashtable *s, char *k);
env_hashtable *env_hashtable_child(env_hashtable *s);

#endif /* HASHTABLE_H */
