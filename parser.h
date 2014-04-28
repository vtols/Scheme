#ifndef PARSER_H
#define PARSER_H

#include <object.h>

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

extern char *names[];

object *parse_single();
object *parse_list();
object *parse_element();

#endif /* PARSER_H */
