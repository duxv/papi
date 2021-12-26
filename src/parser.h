#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    lex_State* lexer;
    tok_Token current_token;
    tok_Token next_token;
    int error_count;
    char** errors;
} parser_State;

parser_State* parser_new(lex_State* lxr);
void parse_next_token(parser_State* parser);
struct program* parse_program(parser_State* parser);

#endif