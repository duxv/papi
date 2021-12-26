#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

typedef struct {
    int num;
    bool boolean;
    char* string;
} tok_Value;

typedef enum {
    tok_Bool, tok_Num, tok_Str,
    tok_Ident, tok_Plus, tok_Minus,
    tok_Slash, tok_Asterisk, tok_Assign,
    tok_Percent, tok_Colon, tok_FatArrow,
    tok_GreaterThan, tok_LessThan,
    tok_Equal, tok_NotEqual, tok_Bang,
    tok_GreaterThanEqual, tok_LessThanEqual,
    tok_If, tok_Elseif, tok_Else,
    tok_Fun, tok_End, tok_Pass, tok_EndOfFile,
    tok_Newline, tok_Illegal, tok_Lparen,
    tok_Rparen, tok_Lbracket, tok_Rbracket,
    tok_True, tok_False, tok_And, tok_Or,
} tok_Token;

extern const char* const tok_names[];

#endif
