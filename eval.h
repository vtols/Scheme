#ifndef EVAL_H
#define EVAL_H

#include <object.h>

object *eval_str(const char *s, env_hashtable *env);
object *eval(object *obj, env_hashtable *env);
object *apply(object *proc, object *args);

#endif /* EVAL_H */
