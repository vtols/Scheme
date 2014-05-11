#include <stdio.h>
#include <string.h>

#include <config.h>

#if !defined(HAVE_POPEN) && defined(HAVE__POPEN)
# define popen _popen
#else
# error "Popen not found."
#endif

int soft_strcmp(const char *a, const char *b)
{
    size_t a_len, b_len, max_len;

    a_len = strlen(a);
    b_len = strlen(b);

    while (a_len && (a[a_len - 1] == '\n' ||
                     a[a_len - 1] == '\r'))
        a_len--;

    while (b_len && (b[b_len - 1] == '\n' ||
                     b[b_len - 1] == '\r'))
        b_len--;

    max_len = (a_len > b_len ? a_len : b_len);

    return strncmp(a, b, max_len);
}

int main(int argc, char *argv[])
{
    char *bin_name,
         *test,
         *right_result,
         program_result[1000],
         run_cmd[1000];
    FILE *output;
    size_t res_length = 0;
    int c, ret;

    bin_name = argv[1];
    test = argv[2];
    right_result = argv[3];

    sprintf(run_cmd, "%s -e \"%s\"", bin_name, test);

    if (!(output = popen(run_cmd, "r")))
        return -1;

    while ((c = fgetc(output)) != EOF)
        program_result[res_length++] = c;
    program_result[res_length] = '\0';

    fclose(output);

    ret = (soft_strcmp(right_result, program_result));

    return ret;
}
