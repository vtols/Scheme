#include <stdlib.h>
#include <string.h>

#include <hashtable.h>

#define M 10001
#define MAX 2000000000
#define HASH(A, B) (A % B)

#ifndef strdup
char *strdup(const char *s)
{
    char *t = malloc(strlen(s) + 1);
    strcpy(t, s);
    return t;
}
#endif

struct node
{
    node *n, *p;
    char *k;
    void *v;
    size_t z;
};

struct list
{
    node *h, *t;
};

node *node_new(char *k, void *v, size_t z)
{
    node *n = (node *) malloc(sizeof(node));
    n->k = strdup(k);
    n->v = v;
    n->n = n->p = NULL;
    n->z = z;
    return n;
}

void node_assign_value(node *n, void *v)
{
    n->v = v;
}

list *list_new()
{
    list *l = (list *) malloc(sizeof(list));
    l->h = l->t = NULL;
    return l;
}

void list_append(list *l, char *k, void *v, size_t z)
{
    node *n = node_new(k, v, z);
    if (l->h == NULL)
        l->h = l->t = n;
    else {
        n->p = l->t;
        l->t->n = n;
        l->t = n;
    }
}

void list_remove(list *l, node *n)
{
    if (n->n == NULL && n->p == NULL)
        l->h = l->t = NULL;
    else if (n->n == NULL) {
        l->t = n->p;
        n->p->n = NULL;
    } else if (n->p == NULL) {
        l->h = n->n;
        n->n->p = NULL;
    } else {
        n->n->p = n->p;
        n->p->n = n->n;
    }
    free(n->k);
    free(n->v);
    free(n);
}

node *list_find(list *l, char *k, size_t z)
{
    node *h = l->h;
    while (h) {
        if (strcmp(h->k, k) == 0 && h->z == z)
            return h;
        h = h->n;
    }
    return NULL;
}

int str_hash(const char *s)
{
    int h = 0;
    while (*s) {
        h = ((long long) h * 269 + *s) % MAX;
        s++;
    }
    return h;
}

hashtable *hashtable_new()
{
    hashtable *s = malloc(sizeof(hashtable));
    s->m = M;
    s->z = 0;
    s->items = calloc(s->m, sizeof(list));
    return s;
}

void hashtable_insert(hashtable *s, char *k, void *v)
{
    node *n;
    int hash = HASH(str_hash(k), s->m);
    if (!s->items[hash])
        s->items[hash] = list_new();
    n = list_find(s->items[hash], k, s->z);
    if (!n)
        list_append(s->items[hash], k, v, s->z);
    else
        node_assign_value(n, v);
}

void *hashtable_find(hashtable *s, char *k)
{
    node *t;
    int hash = HASH(str_hash(k), s->m);
    if (!s->items[hash])
        return NULL;
    t = list_find(s->items[hash], k, s->z);
    if (t)
        return t->v;
    return NULL;
}

void hashtable_remove(hashtable *s, char *k)
{
    node *t;
    int hash = HASH(str_hash(k), s->m);
    if (s->items[hash]) {
        t = list_find(s->items[hash], k, s->z);
        if (t)
            list_remove(s->items[hash], t);
    }
}

env_hashtable *env_hashtable_new()
{
    env_hashtable *e = malloc(sizeof(env_hashtable));
    e->root = e;
    e->parent = NULL;
    e->t = hashtable_new();
    e->z = e->c = 0;
    return e;
}

void env_hashtable_insert(env_hashtable *s, char *k, void *v)
{
    s->t->z = s->z;
    hashtable_insert(s->t, k, v);
}

void *env_hashtable_find(env_hashtable *s, char *k)
{
    void *ret = NULL;
    s->t->z = s->z;
    ret = hashtable_find(s->t, k);
    if (!ret && s->parent)
        ret = env_hashtable_find(s->parent, k);
    return ret;
}

void env_hashtable_remove(env_hashtable *s, char *k)
{
    s->t->z = s->z;
    hashtable_remove(s->t, k);
}

env_hashtable *env_hashtable_child(env_hashtable *s)
{
    env_hashtable *e = malloc(sizeof(env_hashtable));
    e->root = s->root;
    e->parent = s;
    e->t = s->root->t;
    e->z = ++s->root->c;
    return e;
}
