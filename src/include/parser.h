#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef struct PARSER_STRUCT
{
    Lexer* lexer;
    Token* current_t;
    char** ids;
} Parser;

Parser* new_parser(Lexer* lexer, Token* token);

void parser_get_next_token(Parser* parser);

void parser_eat(Parser* parser, int value);

int parser_expr(Parser* parser);

int parser_term(Parser* parser);

int parser_factor(Parser* parser);

int parser_stoi(char* str);

#endif // !PARSER_H