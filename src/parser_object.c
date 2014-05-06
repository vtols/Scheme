#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <parser_object.h>

parser *parser_new()
{
    parser *p = (parser *) malloc(sizeof(parser));
    parser_init(p);
    return p;
}

void parser_init(parser *p)
{
    p->in = NULL;
    p->cur_c = EOF;
    p->cur_tok = p->last_tok = TOK_EOF;
    p->input_buffer = NULL;
    p->read_pos = 0;
    p->read_from_buffer = 0;
    p->single_line = 0;
    p->token_buffer = buffer_new(NULL);
}

void parser_set_file(parser *p, FILE *f)
{
    parser_reset(p);
    p->in = f;
}

void parser_set_str(parser *p, const char *s)
{
    parser_reset(p);
    p->input_buffer = strdup(s);
    p->read_from_buffer = 1;
}

void parser_reset(parser *p)
{
    if (p->in)
        fclose(p->in);
    if (p->input_buffer)
        free(p->input_buffer);
    parser_init(p);
}

int parser_at_end(parser *p)
{
    return p->cur_tok == TOK_EOL ||
           p->cur_tok == TOK_EOF;
}

void parser_free(parser *p)
{
    if (p->in)
        fclose(p->in);
    if (p->input_buffer)
        free(p->input_buffer);
    free(p->token_buffer);
    free(p);
}
