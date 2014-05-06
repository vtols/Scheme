#include <stdio.h>
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
    p->in = stdin;
    p->cur_c = EOF;
    p->cur_tok = EOF;
    p->input_buffer = NULL;
    p->read_from_buffer = 0;
    p->single_line = 0;
    p->token_buffer = buffer_new(NULL);
}

void parser_reset(parser *p)
{
    if (p->input_buffer)
        free(p->input_buffer);
    parser_init(p);
}

void parser_free(parser *p)
{
    if (p->input_buffer)
        free(p->input_buffer);
    free(p->token_buffer);
    free(p);
}
