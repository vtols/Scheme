#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <object.h>

object *proc_primitive_plus(object *args);
object *proc_primitive_mul(object *args);
object *proc_primitive_minus(object *args);
object *proc_primitive_list(object *args);
object *proc_primitive_cons(object *args);
object *proc_primitive_car(object *args);
object *proc_primitive_cdr(object *args);
object *proc_primitive_apply(object *args);

#endif /* PRIMITIVE_H */
