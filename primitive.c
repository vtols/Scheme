#include <primitive.h>

object *proc_primitive_plus(object *args)
{
    object *arg;
    int sum = 0;
    while (args != null_object) {
        arg = CAR(args);
        sum += NUM(arg);
        args = CDR(args);
    }
    return number_int(sum);
}

object *proc_primitive_mul(object *args)
{
    object *arg;
    int sum = 1;
    while (args != null_object) {
        arg = CAR(args);
        sum *= NUM(arg);
        args = CDR(args);
    }
    return number_int(sum);
}

object *proc_primitive_minus(object *args)
{
    object *arg;
    int sum = 0;
    
    if (CDR(args) == null_object)
        return number_int(-NUM(CAR(args)));
    
    sum = NUM(CAR(args));
    args = CDR(args);
    
    while (args != null_object) {
        arg = CAR(args);
        sum -= NUM(arg);
        args = CDR(args);
    }
    return number_int(sum);
}

object *proc_primitive_list(object *args)
{
    return args;
}

object *proc_primitive_cons(object *args)
{
    return cons(CAR(args), CADR(args));
}

object *proc_primitive_car(object *args)
{
    return CAR(CAR(args));
}

object *proc_primitive_cdr(object *args)
{
    return CDR(CAR(args));
}
