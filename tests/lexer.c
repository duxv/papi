#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/lexer.h"
#include "../src/token.h"

bool test_read_number() {
    char* literals[] = {
        "1", "0", "123321", "432423",
        "432", "123", "42343", "2147483647",
        "-214783647", "-1", "-89452249", "123",
        "41243", "10000", "+9321", "-2147483647",
        "+2147483647",
    };
    
    int array_size = sizeof(literals) / sizeof(literals[0]);

    for(int i = 0; i < array_size; i++) {
        lex_State* lxr = lex_new(literals[i]);
        long long parsed_num = lex_read_number(lxr),
             correct = atoll(literals[i]);

        if (parsed_num != correct) {
            printf("#test_read_number: wanted '%lld', got '%lld'\n", correct, parsed_num);
            return false;
        }
    }
    return true;
}

bool test_read_identifier() {
    char* literals [] = {
        "somevar", "som2ing", "pass", "some2432",
        "pass", "fun", "end", "elseif", "else", 
        "Else", "Else1",
    };
    
    int array_size = sizeof(literals) / sizeof(literals[0]);

    for(int i = 0; i < array_size; i++) {
        lex_State* lxr = lex_new(literals[i]); 
        char *correct = literals[i],
            *parsed = lex_read_identifier(lxr);

        if(strcmp(correct, parsed) != 0) {
            printf("#test_read_identifier: wanted '%s', got '%s'\n", correct, parsed);
            return false;
        }
    }

    return true;
}

bool test_identifier_type() {
    typedef struct {
        tok_Token correct;
        char* literal;
    } test_case;

    test_case tests[] = {
        {tok_Ident, "something"},
        {tok_Ident, "string"},
        {tok_Ident, "some2else"},
        {tok_Ident, "illegalsh33z$sh"},
        {tok_Ident, "$"},
        {tok_Ident, "_"},
        {tok_If, "if"},
        {tok_Elseif, "elseif"},
        {tok_Else, "else"},
        {tok_Fun, "fun"},
        {tok_End, "end"},
        {tok_Pass, "pass"},
    };

    int array_size = sizeof(tests) / sizeof(tests[0]);

    for(int i = 0; i < array_size; i++) {
        tok_Token parsed = identifier_type(tests[i].literal);  
        if (tests[i].correct != identifier_type(tests[i].literal)) {
            printf("#test_identifier_type: wanted '%s', got '%s'\n", tok_names[tests[i].correct], tok_names[parsed]);
            return false;
        }
    }
    return true;
}

tok_Value* new_tok_num(int num) {
    tok_Value* ret  = malloc(sizeof(tok_Value));
    ret->num = num;
    return ret;
}

tok_Value* new_tok_string(char* str) {
    tok_Value* ret = malloc(sizeof(tok_Value));
    ret->string = str;
    return ret;
}

bool test_next_token() {
    char* input = 
    "x = 1\n"
    "fun something():\n"
    "   if x > 2\n"
    "      print x\n"
    "   elseif x == 3\n"
    "      print 493 - 3\n"
    "   else\n"
    "      print -34209\n"
    "   end\n"
    "end\n"
    "pass fun end elseif $ ! != >= <= < > => true false & | && ||\n";

    lex_State* lxr =  lex_new(input);

    typedef struct {
        tok_Token type;
        tok_Value* value;
    } test_case;

    test_case tests[] = {
        {tok_Ident, NULL},
        {tok_Assign, NULL},
        {tok_Num, new_tok_num(1)},
        {tok_Newline, NULL},
        {tok_Fun, NULL},
        {tok_Ident, new_tok_string("something")},
        {tok_Lparen, NULL},
        {tok_Rparen, NULL},
        {tok_Colon, NULL},
        {tok_Newline, NULL},
        {tok_If, NULL},
        {tok_Ident, new_tok_string("x")},
        {tok_GreaterThan, NULL},
        {tok_Num, new_tok_num(2)},
        {tok_Newline, NULL},
        {tok_Ident, new_tok_string("print")},
        {tok_Ident, new_tok_string("x")},
        {tok_Newline, NULL},
        {tok_Elseif, NULL},
        {tok_Ident, new_tok_string("x")},
        {tok_Equal, NULL},
        {tok_Num, new_tok_num(3)},
        {tok_Newline, NULL},
        {tok_Ident, new_tok_string("print")},
        {tok_Num, new_tok_num(493)},
        {tok_Minus, NULL},
        {tok_Num, new_tok_num(3)},
        {tok_Newline, NULL},
        {tok_Else, NULL},
        {tok_Newline, NULL},
        {tok_Ident, new_tok_string("print")},
        {tok_Minus, NULL},
        {tok_Num, new_tok_num(34209)},
        {tok_Newline, NULL},
        {tok_End, NULL},
        {tok_Newline, NULL},
        {tok_End, NULL},
        {tok_Newline, NULL},
        {tok_Pass, NULL},
        {tok_Fun, NULL},
        {tok_End, NULL},
        {tok_Elseif, NULL},
        {tok_Ident, new_tok_string("$")},
        {tok_Bang, NULL},
        {tok_NotEqual, NULL},
        {tok_GreaterThanEqual, NULL},
        {tok_LessThanEqual, NULL},
        {tok_LessThan, NULL},
        {tok_GreaterThan, NULL},
        {tok_FatArrow, NULL},
        {tok_True, NULL},
        {tok_False, NULL},
        {tok_Illegal, NULL},
        {tok_Illegal, NULL},
        {tok_And, NULL},
        {tok_Or, NULL},
        {tok_Newline, NULL},
        {tok_EndOfFile, NULL},
    };

    int array_size = sizeof(tests) / sizeof(tests[0]);

    for(int i = 0; i < array_size; i++) {
        tok_Token tok = lex_next_token(lxr);
        if (tok != tests[i].type) {
            printf("#test_identifier_type: wanted '%s', got '%s'\n", tok_names[tests[i].type], tok_names[tok]);
            return false;
        }

        if (tests[i].value != NULL && (tests[i].value->num != lxr->last.num && strcmp(tests[i].value->string, lxr->last.string) != 0)) {
            printf("#test_identifier_type: wanted value '%d', got '%d'\n", tests[i].value->num, lxr->last.num);
            return false;
        };
    }

    return true;
}

int main() {
    if (!test_read_number()) {
        printf("TESTING FAILED AT 'test_read_number'\n");
        exit(1);
    } else if (!test_read_identifier()) {
        printf("TESTING FAILED AT 'test_read_identifier'\n");
        exit(1);
    } else if (!test_identifier_type()) {
        printf("TESTING FAILED AT 'test_identifier_type'\n");
        exit(1);
    } else if (!test_next_token()) {
        printf("TESTING FAILED AT 'test_next_token'\n");
        exit(1);
    } else {
        printf("OK\n");
    }
}
