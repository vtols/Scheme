#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <object.h>

object null_value = {
    .obj_type = OBJ_PAIR,
    .obj.pr = {
        .car = NULL,
        .cdr = NULL,
    },
};

object *null_object = &null_value;

/* Internal cons implementation */
object *cons(object *first, object *rest)
{
    object *p = ALLOC(object);
    p->obj_type = OBJ_PAIR;
    p->obj.pr.car = first;
    p->obj.pr.cdr = rest;
    return p;
}

object *symbol(const char *s)
{
    object *p = ALLOC(object);
    p->obj_type = OBJ_SYMBOL;
    p->obj.str = strdup(s);
    return p;
}

object *number_str(const char *s)
{
    object *p = ALLOC(object);
    p->obj_type = OBJ_NUMBER;
    sscanf(s, "%d", &p->obj.num);
    return p;
}

object *number_int(int n)
{
    object *p = ALLOC(object);
    p->obj_type = OBJ_NUMBER;
    p->obj.num = n;
    return p;
}

object *primitive_procedure(primitive_proc proc)
{
    object *p = ALLOC(object);
    p->obj_type = OBJ_PRIMITIVE_PROCEDURE;
    p->obj.proc = proc;
    return p;
}

object *compound_procedure(object *params, object *body, 
            env_hashtable *env)
{
    object *p = ALLOC(object);
    p->obj_type = OBJ_COMPOUND_PROCEDURE;
    p->obj.c_proc.params = params;
    p->obj.c_proc.body = body;
    p->obj.c_proc.env = env;
    return p;
}

void print_object(object *obj)
{
    int first = 1;
    if (!obj)
        return;
    switch (obj->obj_type) {
        case OBJ_SYMBOL:
            printf("%s", obj->obj.str);
            break;
        case OBJ_NUMBER:
            printf("%d", obj->obj.num);
            break;
        case OBJ_PAIR:
            printf("(");
            while (obj->obj_type == OBJ_PAIR &&
                   obj != null_object) {
                if (!first)
                    putchar(' ');
                print_object(obj->obj.pr.car);
                obj = obj->obj.pr.cdr;
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
