#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    lex_State* lexer;
    tok_Token ctok; // current token
    tok_Token ntok; // next token
    char* error;
} parser_State;

parser_State* parser_new(lex_State *lxr);
struct program* parse_program(parser_State *parser);
struct expression* parse_expression(parser_State *parser, int precedence);
char* statement_to_string(struct statement *stmt);

#endif