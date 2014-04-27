#include <stdio.h>
#include <string.h>

#include <parser.h>
#include <object.h>

#define MAXBUF 2000
#define DIGIT(x) ((x) >= '0' && (x) <= '9')

char *names[] =
{
    [TOK_SYMBOL] = "TOK_SYMBOL",
    [TOK_NUMBER] = "TOK_NUMBER",
    [TOK_LPAREN] = "TOK_LPAREN",
    [TOK_RPAREN] = "TOK_RPAREN",
    [TOK_DOT] = "TOK_DOT",
    [TOK_SINGLE_QUOTE] = "TOK_SINGLE_QUOTE",
};

char buf[MAXBUF], c = EOF;
char rbuf[MAXBUF];
int rpos = 0;
/* Flag */
int single_line = 0, read_buffer = 0;
enum token tok;

int read()
{
    if (read_buffer)
        c = rbuf[rpos++];
    else
        c = getchar();
    if (c == 0 || (single_line && c == '\n'))
        c = EOF;
    return c;
}

int next()
{
    int ret = TOK_EOF, i = 0, all_digit = 1;
    
    while (c == ' ' || c == '\n')
        read();
        
    switch(c) {
        case EOF:
            return TOK_EOF;
        case '(':
            ret = TOK_LPAREN;
            break;
        case ')':
            ret = TOK_RPAREN;
            break;
        case '.':
            ret = TOK_DOT;
            break;
        case '\'':
            ret = TOK_SINGLE_QUOTE;
            break;
        default:
            while (c != '(' && c != ')' && 
                   c != ' ' && c != '\n' &&
                   c != EOF) {
                all_digit &= DIGIT(c);
                buf[i++] = c;
                read();
            }
            buf[i] = '\0';
            ret = (all_digit ? TOK_NUMBER : TOK_SYMBOL);
            return ret;
            break;
    }
    
    buf[0] = c;
    buf[1] = '\0';
    read();
    
    return ret;
}

int next_tok()
{
    return tok = next();
}

object *parse_single(char *s)
{
    if (s) {
        strcpy(rbuf, s);
        read_buffer = 1;
        rpos = 0;
    } else
        read_buffer = 0;
    single_line = 1;
    
    read();
    next_tok();
    
    return parse_element();
}

object *parse_element()
{
    object *ret = null_object;
    if (tok == TOK_LPAREN)
        ret = parse_list();
    else if (tok == TOK_SYMBOL) {
        ret = symbol(buf);
        next_tok();
    } else if (tok == TOK_NUMBER) {
        ret = number_str(buf);
        next_tok();
    } else if (tok == TOK_SINGLE_QUOTE) {
        next_tok();
        ret = cons(symbol("quote"),
                   cons(parse_element(), null_object));
    }
    return ret;
}

object *parse_list()
{
    object *list = null_object,
           *last_pair = NULL,
           *t;
    
    next_tok(); /* ( */
    
    while (tok != TOK_RPAREN) {
        if (tok == TOK_DOT) {
            next_tok(); /* . */
            last_pair->obj.pr.cdr = parse_element();
            break;
        }
        t = cons(parse_element(), null_object);
        if (!last_pair)
            list = t;
        else
            last_pair->obj.pr.cdr = t;
        last_pair = t;
    }
    
    next_tok(); /* ) */
    
    return list;
}
