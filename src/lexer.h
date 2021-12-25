#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#include "token.h"

typedef struct {
    char* input;
    int pos;
    char ch0, ch1;
    tok_Value last; // value of the last parsed number, string or identifier
    int length;
    int line;
    int column;
} lex_State;

// allocates a new instance of the lex_State
// on the heap, binds the pointer input to
// the lex_State input and returns a pointer 
// to it
lex_State* lex_new(char* input);

// mark the current char as read and jump
// to the next one
void lex_readch(lex_State* lxr);

// call the lex_readch() function until the
// current char is no longer a whitespace
void lex_skipws(lex_State* lxr);

// calls the lex_readch() function until the
// next character is a newline
void lex_skipcomm(lex_State* lxr);


// get the next token in the lexer
tok_Token lex_next_token(lex_State* lxr);

// check if a character can be at the start
// of an identifier
bool is_identifier_start(char ch);

// check if a character can be part of an
// identifier
bool is_identifier_part(char ch);

// read an identifier
char* lex_read_identifier(lex_State* lxr);

// read a number without floating point precision
// and return it
long long lex_read_number(lex_State* lxr);

// check if a token is either a keyword or
// an identifier
tok_Token identifier_type(char* ident);

#endif
