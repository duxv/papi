#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    LOWEST,
    COND, // || or &&
    ASSIGN, // =
    EQUALS, // == or !=
    LESSGREATER, // < > <= >=
    SUM, // + or -
    PRODUCT, // / or *
    MOD, // %
    PREFIX, // !X or -X
    HIGHEST,
};

static
void next_token(parser_State* parser) {
    parser->ctok = parser->ntok;
    parser->ntok = lex_next_token(parser->lexer);
}

parser_State* parser_new(lex_State* lxr) {
    parser_State* parser = malloc(sizeof(parser_State));

    parser->lexer = lxr;
    parser->error = NULL;

    next_token(parser);
    next_token(parser);

    return parser;
}

static
void add_statement(struct program* prog, struct statement* stmt) {
    int newSize = prog->size + 1;
    prog->statements = realloc(prog, newSize);

    prog->statements[prog->size].type = stmt->type;
    prog->statements[prog->size].expr = stmt->expr;

    prog->size = newSize;
}

static
struct expression* parse_ident(parser_State* parser) {
    struct expression* expr = malloc(sizeof(struct expression));
    expr->type = EXPR_IDENT;
    expr->ident = parser->lexer->last.string;

    return expr;
}

static
struct expression* parse_num(parser_State* parser) {
    struct expression* expr = malloc(sizeof(struct expression));
    expr->type = EXPR_NUM;
    expr->number = parser->lexer->last.num;

    return expr;
}

static
int get_precedence(tok_Token token) {
    switch (token) {
        case tok_Assign:           return ASSIGN;
        case tok_Equal:            return EQUALS;
        case tok_NotEqual:         return EQUALS;
        case tok_LessThan:         return LESSGREATER;
        case tok_GreaterThan:      return LESSGREATER;
        case tok_LessThanEqual:    return LESSGREATER;
        case tok_GreaterThanEqual: return LESSGREATER;
        case tok_Plus:             return SUM;
        case tok_Minus:            return SUM;
        case tok_Slash:            return PRODUCT;
        case tok_Asterisk:         return PRODUCT;
        case tok_Percent:          return MOD;
        default:                   return LOWEST;
    }
}

static
struct expression* parse_assign_expression(parser_State* parser, struct expression* expr) {
    if (expr->type != EXPR_IDENT) {
        printf("Can only assign values to identifiers. Not '%s'", expr_types[expr->type]);
        return NULL;
    }

    int ident_len = strlen(expr->ident);
    char* new_ident = malloc(ident_len);

    strncpy(new_ident, expr->ident, ident_len);
    expr->assignment->ident = new_ident;

    next_token(parser); // skip the =
    expr->assignment->value = parse_expression(parser, ASSIGN);

    return expr;
}


struct expression* parse_expression(parser_State* parser, int precedence) {
    struct expression* expr;
    printf("PARSE EXPRESSION\n");
    printf("%d: '%s'\n", parser->ctok, tok_names[parser->ctok]);
    
    // choose the prefix function
    switch (parser->ctok) {
        case tok_Ident:
            expr = parse_ident(parser);
            break;
        case tok_Num:
            expr = parse_num(parser);
            break;
        default:
            printf("NO PREFIX PARSE FUNCTION FOR %s\n", tok_names[parser->ctok]);
            exit(1);
    }

    while(parser->ntok != tok_Newline && precedence < get_precedence(parser->ntok)){
        switch (parser->ntok) {
            case tok_Assign:
                expr = parse_assign_expression(parser, expr);
            default:
                return expr;
        }
        next_token(parser);
    }

    return expr;
}

static
struct statement* parse_expression_statement(parser_State* parser) {
    printf("TIMES A EXPRESSION STATEMENT&\n");
    struct statement* stmt = malloc(sizeof(struct statement));
    stmt->type = STMT_EXPR;
    stmt->expr = parse_expression(parser, LOWEST);
    return stmt;
}

static
struct statement* parse_statement(parser_State* parser) {
    printf("TIMES A DAY&\n");
    switch (parser->ctok) {
        default:
            return parse_expression_statement(parser);
    }
    return NULL;
}

struct program* parse_program(parser_State* parser) {
    struct program* prog = calloc(1, sizeof(struct program));
    prog->statements = calloc(1, sizeof(struct statement));
    prog->size = 0;

    while(parser->ctok != tok_EndOfFile) {
        struct statement* stmt = parse_statement(parser);
        add_statement(prog, stmt);
        next_token(parser);
    }

    return prog;
}


char* statement_to_string(struct statement *stmt) {
    char* out;

    switch (stmt->type) {
        case STMT_EXPR:
                switch (stmt->expr->type) {
                    case EXPR_IDENT:
                        out = stmt->expr->ident;
                        break;
                    case EXPR_BOOL:
                        out = calloc(strlen("false"), sizeof(char));
                        if(stmt->expr->boolean) {
                            strcpy(out, "true");
                        } else {
                            strcpy(out, "false");
                        }
                        break;
                    
                    case EXPR_NUM:
                        out = calloc(30, sizeof(char));
                        snprintf(out, 30, "%ld", stmt->expr->number);
                        break;
                    default:
                        out = calloc(100, sizeof(char));
                        sprintf(out, "string does not work for that yet");
                }
        break;
    }
    return out;
}