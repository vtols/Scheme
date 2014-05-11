#include <stdio.h>
#include <string.h>

#include <run.h>
#include <init.h>

#include <config.h>

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

char *expr = NULL,
     *prompt = NULL,
     *in_file = NULL;

void print_usage();
void parse_options(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    env_hashtable *global_env;
    FILE *f;
    
    parse_options(argc, argv);
    
    global_env = env_hashtable_new();
    init_global_environment(global_env);
    
    if (in_file) {
        f = fopen(in_file, "r");
        if (!f) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        run_opened_file(f, global_env);
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

void parse_options(int argc, char *argv[])
{
#ifdef HAVE_GETOPT
    int opt;
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
    in_file = argv[optind];
#else
    int optind = 0, subind = 0;
    size_t s_len;

#define ARG_PARAMETER (subind == s_len - 1 ? argv[++optind] : argv[optind] + subind + 1)

    optind++;
    while (optind < argc) {
        if (subind == 0) {
            s_len = strlen(argv[optind]);
            if (!s_len)
                continue;
            if (argv[optind][0] == '-')
                subind++;
            else
                in_file = argv[optind];
        } else if (argv[optind][subind] == 'e') {
            expr = ARG_PARAMETER;
            optind++;
            subind = 0;
        } else if (argv[optind][subind] == 'p') {
            prompt = ARG_PARAMETER;
            optind++;
            subind = 0;
        }
    }

#undef ARG_PARAMETER
#endif
}
