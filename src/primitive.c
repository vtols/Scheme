#include <stdio.h>

#include <eval.h>
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

object *proc_primitive_less(object *args)
{
    int last, cur, res = 1;

    last = NUM(CAR(args));
    args = CDR(args);

    while (res && args != null_object) {
        cur = NUM(CAR(args));
        res &= last < cur;
        last = cur;
        args = CDR(args);
    }

    return (res ? true_object : false_object);
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

object *proc_primitive_apply(object *args)
{
    return apply(CAR(args), CADR(args));
}

object *proc_primitive_display(object *args)
{
    object *arg;
    while (args != null_object) {
        arg = CAR(args);
        print_object(arg);
        args = CDR(args);
    }
    return NULL;
}

object *proc_primitive_newline(object *args)
{
    putchar('\n');
    return NULL;
}
