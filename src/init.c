/* init.c - Startup initialization */

/* sprintf */
#include <stdio.h>

#include <init.h>

/* eval_str */
#include <eval.h>

/* Primitive procedures */
#include <primitive.h>

/* Buffers */
#include <buffer.h>

/* Item of initialization table */
struct init {
    char *key;
    primitive_proc value;
};

/* Initialization table for primitive procedures */
static struct init proc_init_table[] = {
    {"+", proc_primitive_plus},
    {"*", proc_primitive_mul},
    {"-", proc_primitive_minus},
    {"list", proc_primitive_list},
    {"cons", proc_primitive_cons},
    {"car", proc_primitive_car},
    {"cdr", proc_primitive_cdr},
    {"apply", proc_primitive_apply},
    {"", NULL},
};

static void init_primitive_procedures(env_hashtable *env);
static void define_pair_procedures(env_hashtable *env);
static void define_recursive(buffer *ad_name, buffer *body, int depth,
                                env_hashtable *env);

void init_global_environment(env_hashtable *env)
{
    init_primitive_procedures(env);
    define_pair_procedures(env);
    
    /* Parser determines empty brackets as null_object */
    eval_str("(define null '())", env);
}

/* Put all primitive procedures from table to environment */
static void init_primitive_procedures(env_hashtable *env)
{
    int i;
    object *t;
    
    for (i = 0; proc_init_table[i].value; i++) {
        t = primitive_procedure(proc_init_table[i].value);
        env_hashtable_insert(env, proc_init_table[i].key, t);
    }
}

/* Define procedures cadr, caddr, etc. */
static void define_pair_procedures(env_hashtable *env)
{
    buffer *a = buffer_new(""),
           *b = buffer_new("x");
           
    define_recursive(a, b, 0, env);
    
    buffer_free(a);
    buffer_free(b);
}

static void define_recursive(buffer *ad_name, buffer *body, int depth,
                                env_hashtable *env) {
    buffer *expr_buffer,
           *new_ad_name,
           *new_body;
    char *expr_str;

    if (depth > 4)
        return;
    if (depth > 1) {
        expr_buffer = buffer_nprintf("(define c%br"
                                     " (lambda (x) %b))",
                                     ad_name,
                                     body);
        
        expr_str = buffer_to_str(expr_buffer);
        
        eval_str(expr_str, env);
        
        free(expr_str);
        free(expr_buffer);
    }

    new_ad_name = buffer_nprintf("a%b", ad_name);
    new_body = buffer_nprintf("(car %b)", body);

    define_recursive(new_ad_name, new_body, depth + 1, env);
    buffer_free(new_ad_name);
    buffer_free(new_body);

    new_ad_name = buffer_nprintf("d%b", ad_name);
    new_body = buffer_nprintf("(cdr %b)", body);

    define_recursive(new_ad_name, new_body, depth + 1, env);
    buffer_free(new_ad_name);
    buffer_free(new_body);
}
