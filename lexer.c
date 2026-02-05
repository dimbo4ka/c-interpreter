#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


Token create_eof() {
    Token token;
    token.token_type = Eof;
    return token;
}

Token create_by_char(char c) {
    Token token;
    switch (c) {
        case '=': {
            token.token_type = Eq;
            break;
        }
        case '-': {
            token.token_type = Minus;
            break;
        }
        case '+': {
            token.token_type = Plus;
            break;
        }
        case '*': {
            token.token_type = Star;
            break;
        }
        case '/': {
            token.token_type = Slash;
            break;
        }
        case '(': {
            token.token_type = LParen;
            break;
        }
        case ')': {
            token.token_type = RParen;
            break;
        }
        case '\n': {
            token.token_type = Endline;
            break;
        }
    }
    return token;
}

Token get_num_literal(Lexer *lexer) {
    size_t start = lexer->cur;
    while (isdigit(lexer->text[lexer->cur])) {
        ++lexer->cur;
    }

    Token token;
    token.token_type = NumLiteral;
    token.num_literal = 0;
    for (size_t i = start; i < lexer->cur; ++i) {
        token.num_literal *= 10;
        token.num_literal += lexer->text[i] - '0';
    }
    return token;
}

Token get_id(Lexer *lexer) {
    size_t start = lexer->cur;
    while (isalpha(lexer->text[lexer->cur])) {
        ++lexer->cur;
    }

    size_t sz = lexer->cur - start;
    char *id = (char*)malloc(sizeof(char) * (sz + 1));
    for (size_t i = start; i < lexer->cur; ++i) {
        id[i - start] = lexer->text[i];
    }
    id[sz] = '\0';
    
    Token token;
    if (sz == 3 && strncmp(lexer->text, "var", 3) == 0) {
        token.token_type = KWVar;
        return token;       
    } 

    token.token_type = Id;
    token.id = id;

    return token;
}

Token next_token(Lexer *lexer) {
    // printf("lexer->cur = %llu\n", (unsigned long long)lexer->cur);
    while (lexer->text[lexer->cur] == ' ') {
        ++lexer->cur;
    }
    if (lexer->text[lexer->cur] == '\0')
        return create_eof();

    char c = lexer->text[lexer->cur];
    switch (c) {
        case '-': case '+': case '*': case '/': case '=': case '(': case ')': case '\n': {
            ++lexer->cur;
            return create_by_char(c);
        }
    }
    if (isdigit(c))
        return get_num_literal(lexer);
    if (isalpha(c))
        return get_id(lexer);
    printf("Error in code. Invalid symbol in position %llu", (unsigned long long)lexer->cur);
    exit(1);
}

Token get_peek(Lexer *lexer) {
    size_t cur = lexer->cur;
    Token token = next_token(lexer);
    lexer->cur = cur;
    return token;
}

void print_token(Token token) {
    switch (token.token_type) {
        case KWVar:
            printf("var");
            break;
        case Id:
            printf("id(%s)", token.id);
            break;
        case Eq:
            printf("=");
            break;
        case Plus:
            printf("+");
            break;
        case Minus:
            printf("-");
            break;
        case Star:
            printf("*");
            break;
        case Slash:
            printf("/");
            break;
        case LParen:
            printf("(");
            break;
        case RParen:
            printf(")");
            break;
        case Endline:
            printf("Endline");
            break;
        case Eof:
            printf("Eof");
            break;
        case NumLiteral:
            printf("NumLiteral(%lli)", token.num_literal);
            break;
    }
    printf("\n");
}

Lexer create_lexer(char *text) {
    Lexer lexer;
    lexer.text = text;
    lexer.cur = 0;
    return lexer;
}
