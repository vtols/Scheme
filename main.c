#include <stdio.h>

#include <parser.h>
#include <hashtable.h>
#include <eval.h>
#include <init.h>

int main(void)
{
    object *obj;
    env_hashtable *env;
    
    env = env_hashtable_new();
    init_global_environment(env);
    
    for (;;) {
        printf("> ");
        
        obj = parse_single(NULL);
        if (obj == null_object)
            break;
        obj = eval(obj, env);
        print_object_newline(obj);
    }
    
    return 0;
}
