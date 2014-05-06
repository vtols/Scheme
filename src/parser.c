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
    if (p->cur_c != EOF)
        buffer_append_char(p->token_buffer, p->cur_c);
    if (p->read_from_buffer)
        p->cur_c = p->input_buffer[p->read_pos++];
    else
        p->cur_c = fgetc(p->in);
    if (p->cur_c == 0)
        p->cur_c = EOF;
    return p->cur_c;
}

static int read_token(parser *p)
{
    int ret = TOK_EOF, all_digit = 1;
    
    buffer_reset(p->token_buffer);
    
    if (p->last_tok == TOK_EOL) {
        read_char(p);
        buffer_reset(p->token_buffer);
    }
    
    while (p->cur_c == ' ') {
        read_char(p);
        buffer_reset(p->token_buffer);
    }
        
    switch(p->cur_c) {
        case EOF:
            return TOK_EOF;
        case '\n':
            return TOK_EOL;
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
    p->last_tok = p->cur_tok;
    return p->cur_tok = read_token(p);
}

object *parse_single(parser *p)
{
    if (p->cur_c == EOF) {
        read_char(p);
        next_token(p);
    }
    return parse_element(p);
}

object *parse_element(parser *p)
{
    object *ret = null_object;
    char *token_buffer_str;
    
    while (p->cur_tok == TOK_EOL)
        next_token(p);
    
    if (p->cur_tok == TOK_LPAREN)
        ret = parse_list(p);
    else if (p->cur_tok == TOK_SYMBOL) {
        token_buffer_str = buffer_to_str(p->token_buffer);
        ret = symbol(token_buffer_str);
        free(token_buffer_str);
        next_token(p);
    } else if (p->cur_tok == TOK_NUMBER) {
        token_buffer_str = buffer_to_str(p->token_buffer);
        ret = number_str(token_buffer_str);
        free(token_buffer_str);
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
        
        while (p->cur_tok == TOK_EOL)
            next_token(p);
    }
    
    next_token(p); /* ) */
    
    return list;
}
