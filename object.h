#ifndef OBJECT_H
#define OBJECT_H

/* env_hashtable */
#include <hashtable.h>

/* Shorthand for allocation */
#define ALLOC(T) ((T*) malloc(sizeof(T)))

typedef struct object object;

/* Type of primitive procedure */
typedef object *(*primitive_proc)(object *);

/* Object types */
enum object_type
{
    OBJ_SYMBOL,
    OBJ_NUMBER,
    OBJ_BOOLEAN,
    OBJ_PAIR,
    /* For special symbols as: lambda, let, define ... */
    /*OBJ_SPECIAL,*/
    OBJ_PRIMITIVE_PROCEDURE,
    OBJ_COMPOUND_PROCEDURE,
};

/* Object itself */
struct object
{
    enum object_type obj_type;
    union {
        /* Store numeric or boolean */
        int num;
        
        /* Store symbol */
        char *str;
        
        /* Store pair */
        struct pair {
            object *car, *cdr;
        } pr;
        
        /* Store procedures */ 
        struct compound_proc {
            object *params, *body;
            env_hashtable *env;
        } c_proc;
        
        /* Store primitive procedures
         * (Implemented in C) */
        primitive_proc proc;
    } obj;
};

/* Definitions to access object members */
#define TYPE(__x) ((__x)->obj_type)
#define NUM(__x) ((__x)->obj.num)
#define STR(__x) ((__x)->obj.str)

/* Access to pairs and lists */
#define CAR(__x) ((__x)->obj.pr.car)
#define CDR(__x) ((__x)->obj.pr.cdr)
#define CADR(__x) CAR(CDR(__x))
#define CDDR(__x) CDR(CDR(__x))
#define CADDR(__x) CAR(CDR(CDR(__x)))

/* Access to compound procedures internals */
#define CPROC(__x) ((__x)->obj.c_proc)
#define CPROC_PARAMS(__x) (CPROC(__x).params)
#define CPROC_BODY(__x) (CPROC(__x).body)
#define CPROC_ENV(__x) (CPROC(__x).env)

/* Access to primitive procedures */
#define PROC(__x) ((__x)->obj.proc)

/* Null object */
extern object *null_object;

/* Boolean objects */
extern object *true_object;
extern object *false_object;

object *symbol(const char *s);
object *number_str(const char *s);
object *number_int(int n);
object *primitive_procedure(primitive_proc p);
object *compound_procedure(object *params, object *body, 
            env_hashtable *env);
object *cons(object *first, object *rest);

void print_object(object *obj);
void print_object_newline(object *obj);

#endif /* OBJECT_H */
