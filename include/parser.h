#ifndef PARSER_H
#define PARSER_H

#include <object.h>
#include <buffer.h>

typedef struct parser parser;

enum token
{
    TOK_SYMBOL,
    TOK_NUMBER,
    TOK_TRUE,
    TOK_FALSE,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_DOT,
    TOK_SINGLE_QUOTE,
    TOK_EOF,
};

struct parser
{
    /* Input file */
    FILE *in;
    
    /* Current character */
    int cur_c;
    
    /* Flag which means we read from buffer */
    int read_from_buffer;
    
    /* Flag which means we want to read single line */
    int single_line;
    
    /* Current position when reading from buffer */
    int read_pos;
    
    /* Current token type */
    enum token cur_tok;
    
    /* Current token value */
    buffer *token_buffer;
    
    /* Buffer for reading */
    char *input_buffer;
};

extern char *names[];

parser *parser_new();
void parser_init(parser *p);
void parser_reset(parser *p);
void parser_free(parser *p);

object *parse_single(parser *p, const char *s);
object *parse_list(parser *p);
object *parse_element(parser *p);

#endif /* PARSER_H */
