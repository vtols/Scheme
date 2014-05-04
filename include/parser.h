#ifndef PARSER_H
#define PARSER_H

#include <object.h>

#define MAXBUF 2000

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
    
    /* Length of current token */
    int token_len;
    
    /* Flag which means we read from buffer */
    int read_buffer;
    
    /* Flag which means we want to read single line */
    int single_line;
    
    /* Current position when reading from buffer */
    int read_pos;
    
    /* Current token type */
    enum token cur_tok;
    
    /* Current token value */
    char token_buffer[MAXBUF];
    
    /* Buffer for reading */
    char input_buffer[MAXBUF];
};

extern char *names[];

void parser_reset(parser *p);

object *parse_single();
object *parse_list();
object *parse_element();

#endif /* PARSER_H */
