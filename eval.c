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


/* Parse and evaluate string */
object *eval_str(const char *s, env_hashtable *env)
{
    object *obj = parse_single(s);
    return eval(obj, env);
}

/* Evaluate object 
 * NULL return value means Nothing
 */
object *eval(object *obj, env_hashtable *env)
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
            t = CDDR(obj);
            t = cons(symbol("begin"), t);
            eobj = compound_procedure(CADR(obj), t, env);
            return eobj;
        } else if (strcmp("define", STR(t)) == 0) {
            eobj = eval(CADDR(obj), env);
            env_hashtable_insert(env, STR(CADR(obj)), eobj);
            return NULL; /* Not error, just nothing */
        } else if (strcmp("begin", STR(t)) == 0) {
            obj = CDR(obj);
            eobj = NULL; /* Not error, just nothing */
            while (obj != null_object) {
                eobj = eval(CAR(obj), env);
                obj = CDR(obj);
            }
            return eobj;
        } else if (strcmp("apply", STR(t)) == 0) {
            eobj = eval(CADR(obj), env);
            t = eval(CADDR(obj), env);
            return apply(eobj, t);
        } else if (strcmp("quote", STR(t)) == 0) {
            return CADR(obj);
        }
    }
    
    /* Object evaluation */
    switch (obj->obj_type) {
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
                   cur->obj_type == OBJ_PAIR) {
                t = cons(eval(CAR(cur), env), null_object);
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

/* Apply procedure */
object *apply(object *proc, object *args)
{
    primitive_proc p;
    env_hashtable *e;
    object *params, *param, *arg;
    char *s_param;
    if (proc->obj_type == OBJ_PRIMITIVE_PROCEDURE) {
        p = proc->obj.proc;
        return p(args);
    } else if (TYPE(proc) == OBJ_COMPOUND_PROCEDURE) {
        e = env_hashtable_child(proc->obj.c_proc.env);
        
        params = proc->obj.c_proc.params;
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
        return eval(proc->obj.c_proc.body, e);
    }
    return NULL;
}
