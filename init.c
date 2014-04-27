#include <stdio.h>

#include <init.h>
#include <eval.h>
#include <primitive.h>

struct init {
    char *key;
    primitive_proc value;
};

static struct init proc_init_table[] = {
    {"+", proc_primitive_plus},
    {"*", proc_primitive_mul},
    {"-", proc_primitive_minus},
    {"list", proc_primitive_list},
    {"cons", proc_primitive_cons},
    {"car", proc_primitive_car},
    {"cdr", proc_primitive_cdr},
    {"", NULL},
};

static void init_primitive_procedures(env_hashtable *env);
static void define_pair_procedures(env_hashtable *env);

void init_global_environment(env_hashtable *env)
{
    init_primitive_procedures(env);
    define_pair_procedures(env);
    
    eval_str("(define null '())", env);
}

static void init_primitive_procedures(env_hashtable *env)
{
    int i;
    object *t;
    
    for (i = 0; proc_init_table[i].value; i++) {
        t = primitive_procedure(proc_init_table[i].value);
        env_hashtable_insert(env, proc_init_table[i].key, t);
    }
}

static void define_pair_procedures(env_hashtable *env)
{
    void define_recursive(char *ad_name, char *body, int depth) {
        char bufa[1000], bufb[1000];
        if (depth > 4)
            return;
        if (depth > 1) {
            sprintf(bufa,
                    "(define c%sr (lambda (x) %s))",
                    ad_name,
                    body);
            eval_str(bufa, env);
        }
        
        sprintf(bufa, "a%s", ad_name);
        sprintf(bufb, "(car %s)", body);
        define_recursive(bufa, bufb, depth + 1);
        
        sprintf(bufa, "d%s", ad_name);
        sprintf(bufb, "(cdr %s)", body);
        define_recursive(bufa, bufb, depth + 1);
    }
    define_recursive("", "x", 0);
}
