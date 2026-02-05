#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct {
    enum {
        KWVar,
        Id,
        Eq,
        Plus,
        Minus,
        Star,
        Slash,
        LParen,
        RParen,
        NumLiteral,
        Eof,
        Endline,
    } token_type;

    union {
        char* id;
        int64_t num_literal;
    };
} Token;

Token create_eof();

Token create_by_char(char c);

typedef struct {
    char* text;
    size_t cur;
} Lexer;

Token get_num_literal(Lexer *lexer);

Token get_id(Lexer *lexer);

Token next_token(Lexer *lexer);

Token get_peek(Lexer *lexer);

void print_token(Token token);

Lexer create_lexer(char *text);