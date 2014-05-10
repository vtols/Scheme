#ifndef RUN_H
#define RUN_H

#include <hashtable.h>

void run_interactive_loop(const char *prompt, env_hashtable *env);
void run_file(char *path, env_hashtable *env);
void run_opened_file(FILE *f, env_hashtable *env);
void run_expression(const char *expr, env_hashtable *env);

#endif /* RUN_H */
