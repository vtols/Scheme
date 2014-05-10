#include <stdio.h>

#include <run.h>
#include <init.h>

#include <config.h>

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

void print_usage();

int main(int argc, char *argv[])
{
    env_hashtable *global_env;
    char *expr = NULL,
         *prompt = NULL;
    int opt, ind = 1;
    FILE *f;
    
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
    ind = optind;
#endif
    
    global_env = env_hashtable_new();
    init_global_environment(global_env);
    
    if (ind < argc) {
        f = fopen(argv[optind], "r");
        if (!f) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        run_file(f, global_env);
    } else if (!expr)
        run_interactive_loop(prompt, global_env);
    else
        run_expression(expr, global_env);
    
    return 0;
}

void print_usage()
{
    printf("%s\n",
        "Usage: "
        PROGRAM_NAME " [-e <expression>]");
}
