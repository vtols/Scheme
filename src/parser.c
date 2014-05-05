#include <stdio.h>
#include <string.h>

#include <parser.h>
#include <object.h>

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

static int read_char(parser *p);
static int read_token(parser *p);
static int next_token(parser *p);

static int read_char(parser *p)
{
    if (p->cur_c != EOF) {
        p->token_buffer[p->token_len++] = p->cur_c;
        p->token_buffer[p->token_len] = '\0';
    }
    if (p->read_buffer)
        p->cur_c = p->input_buffer[p->read_pos++];
    else
        p->cur_c = fgetc(p->in);
    if (p->cur_c == 0 || (p->single_line && p->cur_c == '\n'))
        p->cur_c = EOF;
    return p->cur_c;
}

static int read_token(parser *p)
{
    int ret = TOK_EOF, all_digit = 1;
    
    p->token_len = 0;
    
    while (p->cur_c == ' ' || p->cur_c == '\n') {
        read_char(p);
        p->token_len = 0;
    }
        
    switch(p->cur_c) {
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
        case '#':
            read_char(p);
            if (p->cur_c == 't')
                ret = TOK_TRUE;
            else if (p->cur_c == 'f')
                ret = TOK_FALSE;
            read_char(p);
            return ret;
        default:
            while (p->cur_c != '(' && p->cur_c != ')' && 
                   p->cur_c != ' ' && p->cur_c != '\n' &&
                   p->cur_c != EOF) {
                all_digit &= DIGIT(p->cur_c);
                read_char(p);
            }
            ret = (all_digit ? TOK_NUMBER : TOK_SYMBOL);
            return ret;
            break;
    }
    read_char(p);
    
    return ret;
}

static int next_token(parser *p)
{
    return p->cur_tok = read_token(p);
}

void parser_reset(parser *p)
{
    p->in = stdin;
    p->cur_c = EOF;
    p->cur_tok = EOF;
    p->read_buffer = 0;
    p->single_line = 0;
    p->token_len = 0;
}

object *parse_single(parser *p, char *s)
{
    parser_reset(p);
    if (s) {
        strcpy(p->input_buffer, s);
        p->read_buffer = 1;
        p->read_pos = 0;
    }
    p->single_line = 1;
    
    read_char(p);
    next_token(p);
    
    return parse_element(p);
}

object *parse_element(parser *p)
{
    object *ret = null_object;
    if (p->cur_tok == TOK_LPAREN)
        ret = parse_list();
    else if (p->cur_tok == TOK_SYMBOL) {
        ret = symbol(p->token_buffer);
        next_token(p);
    } else if (p->cur_tok == TOK_NUMBER) {
        ret = number_str(p->token_buffer);
        next_token(p);
    } else if (p->cur_tok == TOK_TRUE) {
        ret = true_object;
        next_token(p);
    } else if (p->cur_tok == TOK_FALSE) {
        ret = false_object;
        next_token(p);
    } else if (p->cur_tok == TOK_SINGLE_QUOTE) {
        next_token(p);
        ret = cons(symbol("quote"),
                   cons(parse_element(p), null_object));
    }
    return ret;
}

object *parse_list(parser *p)
{
    object *list = null_object,
           *last_pair = NULL,
           *t;
    
    next_token(p); /* ( */
    
    while (p->cur_tok != TOK_RPAREN) {
        if (p->cur_tok == TOK_DOT) {
            next_token(p); /* . */
            CDR(last_pair) = parse_element(p);
            break;
        }
        t = cons(parse_element(p), null_object);
        if (!last_pair)
            list = t;
        else
            CDR(last_pair) = t;
        last_pair = t;
    }
    
    next_token(p); /* ) */
    
    return list;
}
