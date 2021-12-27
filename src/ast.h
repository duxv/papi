#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    OP_PLUS,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_GT,
    OP_GTE,
    OP_LT,
    OP_LTE,
    OP_NOT_EQ,
    OP_BANG,
    OP_AND,
    OP_OR,
    OP_MODULO,
} operator_t;

typedef enum {
    EXPR_INFIX,
    EXPR_PREFIX,
    EXPR_NUM,
    EXPR_IDENT,
    EXPR_BOOL,
} expression_type_t;

extern const char* const expr_types[];

typedef enum {
    STMT_EXPR,
} statement_type_t;

struct expression {
    expression_type_t type;
    union {
        char* string;
        int64_t number;
        bool boolean;
        char* ident;
        struct prefix_expression* prefix;
        struct infix_expression* infix;
        struct assign_expression* assignment;
    };
};

struct assign_expression {
    char* ident;
    struct expression *value;
};

struct prefix_expression {
    operator_t operator;
    struct expression *right;
};

struct infix_expression {
    struct expression *left;
    operator_t operator;
    struct expression *right;
};

struct statement {
    statement_type_t type;
    union {
        struct expression *expr;
    };
};

struct program {
    struct statement *statements;
    int size;
};

#endif
