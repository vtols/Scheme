#include <stdio.h>

#include <parser.h>
#include <hashtable.h>
#include <eval.h>
#include <init.h>

#include <config.h>

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

void print_usage();
void run_interactive_loop(const char *prompt, env_hashtable *env);
void run_single(const char *expr, env_hashtable *env);

int main(int argc, char *argv[])
{
    env_hashtable *global_env;
    char *expr = NULL,
         *prompt = NULL;
    int opt;
    
#ifdef HAVE_GETOPT
    while ((opt = getopt(argc, argv, "e:hp:")) != -1){
        switch (opt) {
            case 'e':
                expr = optarg;
                break;
            case 'p':
                prompt = optarg;
                break;
            case 'h':
            case '?':
                print_usage();
                exit(EXIT_FAILURE);
            default:
                break;
        };
    };
#endif
    
    global_env = env_hashtable_new();
    init_global_environment(global_env);
    
    if (!expr)
        run_interactive_loop(prompt, global_env);
    else
        run_single(expr, global_env);
    
    return 0;
}

void print_usage()
{
    printf("%s\n",
        "Usage: "
        PROGRAM_NAME " [-e <expression>]");
}

void run_interactive_loop(const char *prompt, env_hashtable *env)
{
    object *obj;
    parser *p = parser_new();
    
    parser_set_file(p, stdin);

    while (1) {
        if (parser_at_end(p))
            printf("%s", (prompt ? prompt : DEFAULT_PROMPT));
        
        obj = parse_single(p);
        if (obj == null_object)
            break;
        obj = eval(obj, env);
        print_object_newline(obj);
    }
    
    parser_free(p);
}

void run_single(const char *expr, env_hashtable *env)
{
    object *obj;
    
    obj = eval_str(expr, env);
    print_object_newline(obj);
}
