#include <stdio.h>

#include <run.h>
#include <parser.h>
#include <eval.h>

#include <config.h>

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

void run_file(char *path, env_hashtable *env)
{
    FILE *f;

    f = fopen(path, "r");
    run_opened_file(f, env);
}

void run_opened_file(FILE *f, env_hashtable *env)
{
    object *obj;
    parser *p = parser_new();

    parser_set_file(p, f);

    while (1) {
        obj = parse_single(p);
        if (obj == null_object)
            break;
        obj = eval(obj, env);
    }

    parser_free(p);
}

void run_expression(const char *expr, env_hashtable *env)
{
    object *obj;
    parser *p = parser_new();
    
    parser_set_str(p, expr);

    while (1) {
        obj = parse_single(p);
        if (obj == null_object)
            break;
        obj = eval(obj, env);
        print_object_newline(obj);
    }

    parser_free(p);
}
