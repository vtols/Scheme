#ifndef PARSER_OBJECT_H
#define PARSER_OBJECT_H

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
    TOK_EOL,
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
    enum token last_tok;
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
void parser_set_file(parser *p, FILE *f);
void parser_set_str(parser *p, const char *s);
int parser_at_end(parser *p);
void parser_free(parser *p);

#endif /* PARSER_OBJECT_H */
