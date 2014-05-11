/* eval.c - Evaluation */

#include <stdio.h>
#include <string.h>

/* parse_single() */
#include <parser.h>

/* object definition */
#include <object.h>

/* env_hashtable definition */
#include <hashtable.h>

#include <eval.h>

static object *eval_preproc(object *obj, env_hashtable *env);
static object *preprocess_syntax(object *obj);
static object *preprocess_list_syntax(object *list);

/* Parse and evaluate string */
object *eval_str(const char *s, env_hashtable *env)
{
    parser *p = parser_new();
    parser_set_str(p, s);
    object *obj = parse_single(p);
    return eval(obj, env);
}

object *eval(object *obj, env_hashtable *env)
{
    return eval_preproc(preprocess_syntax(obj), env);
}

/* Evaluate preprocessed object
 * NULL return value means Nothing
 */
static object *eval_preproc(object *obj, env_hashtable *env)
{
    object *cur, *eobj, 
        *last_pair, *t,
        *ecar, *ecdr;
    
    if (!obj)
        return NULL;
    
    /* Detect syntatic construction */
    if (TYPE(obj) == OBJ_PAIR &&
        TYPE(CAR(obj)) == OBJ_SYMBOL) {
        t = CAR(obj);
        if (strcmp("lambda", STR(t)) == 0) {
            eobj = compound_procedure(CADR(obj), CDDR(obj), env);
            return eobj;
        } else if (strcmp("define", STR(t)) == 0) {
            if (TYPE(CADR(obj)) == OBJ_SYMBOL) {
                eobj = eval_preproc(CADDR(obj), env);
                env_hashtable_insert(env, STR(CADR(obj)), eobj);
                return NULL; /* Not error, just nothing */
            } else if (TYPE(CADR(obj)) == OBJ_PAIR) {
                eobj = compound_procedure(CDADR(obj), CDDR(obj), env);
                env_hashtable_insert(env, STR(CAADR(obj)), eobj);
                return NULL;
            }
            return NULL;
        } else if (strcmp("begin", STR(t)) == 0) {
            obj = CDR(obj);
            eobj = NULL; /* Not error, just nothing */
            while (obj != null_object) {
                eobj = eval_preproc(CAR(obj), env);
                obj = CDR(obj);
            }
            return eobj;
        } else if (strcmp("apply", STR(t)) == 0) {
            eobj = eval_preproc(CADR(obj), env);
            t = eval_preproc(CADDR(obj), env);
            return apply(eobj, t);
        } else if (strcmp("quote", STR(t)) == 0) {
            return CADR(obj);
        } else if (strcmp("if", STR(t)) == 0) {
            eobj = eval_preproc(CADR(obj), env);
            if (eobj != false_object)
                return eval_preproc(CADDR(obj), env);
            if (CDDDR(obj) != null_object)
                return eval_preproc(CADDDR(obj), env);
            return NULL;
        }
    }
    
    /* Object evaluation */
    switch (TYPE(obj)) {
        case OBJ_NUMBER:
        case OBJ_BOOLEAN:
            return obj;
        case OBJ_SYMBOL:
            return env_hashtable_find(env, STR(obj));
        case OBJ_PAIR:
            cur = obj;
            eobj = null_object;
            last_pair = NULL;
            
            while (cur != null_object && 
                   TYPE(cur) == OBJ_PAIR) {
                t = cons(eval_preproc(CAR(cur), env), null_object);
                if (!last_pair)
                    eobj = t;
                else
                    CDR(last_pair) = t;
                last_pair = t;
                
                cur = CDR(cur);
            }
            
            ecar = CAR(eobj);
            ecdr = CDR(eobj);
            
            return apply(ecar, ecdr);
        default:
            return NULL;
    }
}

static object *preprocess_syntax(object *obj)
{
    object *t;

    if (TYPE(obj) == OBJ_PAIR &&
        CAR(obj) &&
        TYPE(CAR(obj)) == OBJ_SYMBOL) {
        t = CAR(obj);
        if (strcmp("let", STR(t)) == 0) {
            CDR(obj) = preprocess_list_syntax(CDR(obj));

            object *lambda_params = null_object,
                   *lambda_params_tail = NULL,
                   *lambda_args = null_object,
                   *lambda_args_tail = NULL,
                   *lambda_list = null_object,
                   *lambda_list_tail = NULL,
                   *lambda_body = CADDR(obj),
                   *bindings = CADR(obj),
                   *binding;

            while (bindings != null_object) {
                binding = CAR(bindings);
                object_list_append(&lambda_params, &lambda_params_tail,
                                   CAR(binding));
                object_list_append(&lambda_args, &lambda_args_tail,
                                   CADR(binding));

                bindings = CDR(bindings);
            }

            object_list_append(&lambda_list, &lambda_list_tail,
                               symbol("lambda"));
            object_list_append(&lambda_list, &lambda_list_tail, lambda_params);
            object_list_append(&lambda_list, &lambda_list_tail, lambda_body);

            lambda_list = cons(lambda_list, lambda_args);

            return lambda_list;
        }
    }
    if (TYPE(obj) == OBJ_PAIR) {
        return preprocess_list_syntax(obj);
    }
    return obj;
}

static object *preprocess_list_syntax(object *list)
{
    object *result_list = null_object,
           *result_list_tail = NULL;

    while (list != null_object) {
        if (TYPE(list) != OBJ_PAIR) {
            CDR(result_list_tail) = list;
            break;
        }
        object_list_append(&result_list, &result_list_tail,
                           preprocess_syntax(CAR(list)));
        list = CDR(list);
    }

    return result_list;
}

/* Apply procedure */
object *apply(object *proc, object *args)
{
    primitive_proc p;
    env_hashtable *e;
    object *params, *param, *arg;
    char *s_param;
    if (TYPE(proc) == OBJ_PRIMITIVE_PROCEDURE) {
        p = PROC(proc);
        return p(args);
    } else if (TYPE(proc) == OBJ_COMPOUND_PROCEDURE) {
        e = env_hashtable_child(CPROC_ENV(proc));
        
        params = CPROC_PARAMS(proc);
        while (params != null_object) {
            if (TYPE(params) != OBJ_PAIR) {
                param = params;
                arg = args;
            } else {
                param = CAR(params);
                arg = CAR(args);
            }
            
            s_param = STR(param);
            
            env_hashtable_insert(e, s_param, arg);
            
            if (TYPE(params) != OBJ_PAIR)
                break;
            
            params = CDR(params);
            args = CDR(args);
        }
        return eval_preproc(CPROC_BODY(proc), e);
    }
    return NULL;
}
