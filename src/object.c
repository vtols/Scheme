/* object.c - Work with objects */

/* sscanf, printf */
#include <stdio.h>

/* strdup */
#include <string.h>

#include <object.h>

object null_value = {
    .obj_type = OBJ_PAIR,
    .obj.pr = {
        .car = NULL,
        .cdr = NULL,
    },
};

object true_value = {
    .obj_type = OBJ_BOOLEAN,
    .obj.num = 1,
};

object false_value = {
    .obj_type = OBJ_BOOLEAN,
    .obj.num = 0,
};

object *null_object = &null_value;
object *true_object = &true_value;
object *false_object = &false_value;

/* Internal cons implementation */
object *cons(object *first, object *rest)
{
    object *p = ALLOC(object);
    TYPE(p) = OBJ_PAIR;
    CAR(p) = first;
    CDR(p) = rest;
    return p;
}

object *symbol(const char *s)
{
    object *p = ALLOC(object);
    TYPE(p) = OBJ_SYMBOL;
    STR(p) = strdup(s);
    return p;
}

object *number_str(const char *s)
{
    object *p = ALLOC(object);
    TYPE(p) = OBJ_NUMBER;
    sscanf(s, "%d", &NUM(p));
    return p;
}

object *number_int(int n)
{
    object *p = ALLOC(object);
    TYPE(p) = OBJ_NUMBER;
    NUM(p) = n;
    return p;
}

object *primitive_procedure(primitive_proc proc)
{
    object *p = ALLOC(object);
    TYPE(p) = OBJ_PRIMITIVE_PROCEDURE;
    PROC(p) = proc;
    return p;
}

object *compound_procedure(object *params, object *body, 
            env_hashtable *env)
{
    object *p = ALLOC(object);
    TYPE(p) = OBJ_COMPOUND_PROCEDURE;
    CPROC_PARAMS(p) = params;
    CPROC_BODY(p) = cons(symbol("begin"), body);
    CPROC_ENV(p) = env;
    return p;
}

void object_list_append(object **list, object **tail, object *element)
{
    object *t;

    t = cons(element, null_object);

    if (!*tail)
        *list = t;
    else
        CDR(*tail) = t;

    *tail = t;
}

void print_object(object *obj)
{
    int first = 1;
    if (!obj)
        return;
    switch (TYPE(obj)) {
        case OBJ_SYMBOL:
            printf("%s", STR(obj));
            break;
        case OBJ_NUMBER:
            printf("%d", NUM(obj));
            break;
        case OBJ_BOOLEAN:
            printf("%s", (NUM(obj) ? "#t" : "#f"));
            break;
        case OBJ_PAIR:
            printf("(");
            while (TYPE(obj) == OBJ_PAIR &&
                   obj != null_object) {
                if (!first)
                    putchar(' ');
                print_object(CAR(obj));
                obj = CDR(obj);
                first = 0;
            }
            if (obj != null_object) {
                printf(" . ");
                print_object(obj);
            }
            printf(")");
            break;
        case OBJ_PRIMITIVE_PROCEDURE:
        case OBJ_COMPOUND_PROCEDURE:
            printf("#<procedure>");
            break;
    }
}

void print_object_newline(object *obj)
{
    print_object(obj);
    if (obj)
        putchar('\n');
}
