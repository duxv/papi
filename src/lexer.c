#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast.h"


lex_State* lex_new(char* input) {
    lex_State* lxr = malloc(sizeof(lex_State));
    lxr->input = input;
    lxr->pos = -1;
    lxr->ch0 = 0;
    lxr->ch1 = 0;
    lxr->length = strlen(input);
    lxr->line = 1;
    lxr->column = 1;

    lex_readch(lxr);

    return lxr;
}


void lex_readch(lex_State *lxr) { 
    int npos = lxr->pos + 1;
    if(lxr->pos >= lxr->length) {
        lxr->ch0 = 0;
        return;
    }

    lxr->pos = npos;
    lxr->ch0 = lxr->input[lxr->pos];

    if (lxr->ch0 == '\n') {
        lxr->line += 1;
        lxr->column = 1;
    } else {
        lxr->column += 1;
    }

    npos = lxr->pos + 1;

    if(npos >= lxr->length)
        lxr->ch1 = 0;
    else
        lxr->ch1 = lxr->input[npos];
}


void lex_skipws(lex_State *lxr) {
    while (lxr->ch0 == ' ' ||
           lxr->ch0 == '\t' ||
           lxr->ch0 == '\r' ) lex_readch(lxr);
}

void lex_skipcomm(lex_State* lxr) {
    while (lxr->ch0 != '\n' && lxr->ch0 != 0) {
        lex_readch(lxr);
    }
}

tok_Token lex_next_token(lex_State* lxr) {
    lex_skipws(lxr);

    char ch0 = lxr->ch0,
         ch1 = lxr->ch1;
    tok_Token token;


    switch (ch0) {

        case '\n':
            token = tok_Newline;
            break;

        case 0:
            token = tok_EndOfFile;
            break;

        case '+':
            token = tok_Plus;
            break;

        case '-':
            token = tok_Minus;
            break;

        case '/':
            token = tok_Slash;
            break;

        case '*':
            token = tok_Asterisk;
            break;

        case '(':
            token = tok_Lparen;
            break;

        case ')':
            token = tok_Rparen;
            break;

        case '[':
            token = tok_Lbracket;
            break;
        
        case ']':
            token = tok_Rbracket;

        case '|':
            if (ch1 == '|') {
                lex_readch(lxr);
                token = tok_Or;
            } else {
                token = tok_Illegal;
            }
            break;

        case '&':
            if (ch1 == '&') {
                lex_readch(lxr);
                token = tok_And;
            } else {
                token = tok_Illegal;
            }
            break;

        case '=':
            if (ch1 == '=') {
                lex_readch(lxr);
                token = tok_Equal;
            } else if (ch1 == '>') {
                lex_readch(lxr);
                token = tok_FatArrow;
            } else {
                token = tok_Assign;
            }
            break;

        case '!':
            if (ch1 == '=') {
                lex_readch(lxr);
                token = tok_NotEqual;
            } else {
                token = tok_Bang;
            }
            break;

        case '%':
            token = tok_Percent;
            break;

        case ':':
            token = tok_Colon;
            break;

        case '>':
            if (ch1 == '=') {
                lex_readch(lxr);
                token = tok_GreaterThanEqual;
            } else {
                token = tok_GreaterThan;
            }
            break;

        case '<':
            if (ch1 == '=') {
                lex_readch(lxr);
                token = tok_LessThanEqual;
            } else {
                token = tok_LessThan;
            }
            break;

        default:
           if ('0' <= ch0 && ch0 <= '9') {
               lxr->last.num = lex_read_number(lxr);
               return tok_Num;
           } else if (is_identifier_start(lxr->ch0)) {
               char* ident = lex_read_identifier(lxr);
               tok_Token type = identifier_type(ident);

               if (type == tok_Ident)
                   lxr->last.string = ident;

                return identifier_type(ident);
           } else {
               printf("ILLEGAL: %d: '%c'\n", lxr->ch0, lxr->ch0);
               token = tok_Illegal; 
           }
    }

    lex_readch(lxr);

    return token;
}


bool is_identifier_start(char ch) {
    return ch == '_' || ch == '$' ||
        'a' <= ch && ch <= 'z' ||
        'A' <= ch && ch <= 'Z';
}

bool is_identifier_part(char ch) {
    return ch == '_' || ch == '$' ||
        '0' <= ch && ch <= '9' ||
        'a' <= ch && ch <= 'z' ||
        'A' <= ch && ch <= 'Z';
}



char* lex_read_identifier(lex_State* lxr) {
    int startPos = lxr->pos, length;

    char* src = lxr->pos + lxr->input;

    while (is_identifier_part(lxr->ch0)) lex_readch(lxr);

    length = lxr->pos - startPos;

    char* ident = malloc(length);
    ident[length] = '\0';
    strncpy(ident, src, length);

    return ident;
}


long long lex_read_number(lex_State* lxr) {
    int startPos = lxr->pos, length;

    
    if (lxr->ch0 == '-' || lxr->ch0 == '+')
        lex_readch(lxr);

    while ('0' <= lxr->ch0 && lxr->ch0 <= '9') lex_readch(lxr);

    length = lxr->pos - startPos;

    char *src = startPos + lxr->input,
          numberLiteral[length];

    numberLiteral[length] = '\0';

    strncpy(numberLiteral, src, length);
    return atoll(numberLiteral);
}


tok_Token identifier_type(char* ident) {
    if (strcmp(ident, "if") == 0)
        return tok_If;
    else if (strcmp(ident, "elseif") == 0)
        return tok_Elseif;
    else if (strcmp(ident, "else") == 0)
        return tok_Else;
    else if (strcmp(ident, "fun") == 0)
        return tok_Fun;
    else if (strcmp(ident, "end") == 0)
        return tok_End;
    else if (strcmp(ident, "pass") == 0)
        return tok_Pass;
    else if (strcmp(ident, "true") == 0)
        return tok_True;
    else if (strcmp(ident, "false") == 0)
        return tok_False;
    else
        return tok_Ident;
}


// char* lex_read_string(lex_State* lxr, char delim) {
//     int startPos = lxr->pos;

//     while()
// }
