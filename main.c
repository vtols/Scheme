#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <parser.h>
#include <hashtable.h>
#include <eval.h>
#include <init.h>

#define PROGRAM_NAME "sch"
#define DEFAULT_PROMPT "> "

void print_usage();
void run_interactive_loop(const char *prompt, env_hashtable *env);
void run_single(const char *expr, env_hashtable *env);

int main(int argc, char *argv[])
{
    env_hashtable *global_env;
    char *expr = NULL,
         *prompt = NULL;
    int opt;
    
    while ((opt = getopt(argc, argv, "e:hp:")) != -1){
        switch (opt) {
            case 'e':
                expr = strdup(optarg);
                break;
            case 'p':
                prompt = strdup(optarg);
                break;
            case 'h':
            case '?':
                print_usage();
                exit(EXIT_FAILURE);
            default:
                break;
        };
    };
    
    global_env = env_hashtable_new();
    init_global_environment(global_env);
    
    if (!expr)
        run_interactive_loop(prompt, global_env);
    else {
        run_single(expr, global_env);
        free(expr);
    }
    
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
    
    while (1) {
        printf("%s", (prompt ? prompt : DEFAULT_PROMPT));
        
        obj = parse_single(NULL);
        if (obj == null_object)
            break;
        obj = eval(obj, env);
        print_object_newline(obj);
    }
}

void run_single(const char *expr, env_hashtable *env)
{
    object *obj;
    
    obj = eval_str(expr, env);
    print_object_newline(obj);
}
