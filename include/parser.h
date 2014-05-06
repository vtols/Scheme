#ifndef PARSER_H
#define PARSER_H

#include <object.h>
#include <parser_object.h>

object *parse_single(parser *p);
object *parse_list(parser *p);
object *parse_element(parser *p);

#endif /* PARSER_H */
