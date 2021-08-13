#ifndef PARSER_H
#define PARSER_H
#include "visitor.h"
#include "lexer.h"

typedef struct PARSER_STRUCT
{
    Lexer* lexer;
    Token* current_t;
    AST** ast;
    unsigned int ast_size;
} Parser;

Parser* new_parser(Lexer* lexer, Token* token);

void parser_get_next_token(Parser* parser);

void parser_eat(Parser* parser, int value);

AST* parser_expr(Parser* parser);

AST* parser_term(Parser* parser);

AST* parser_factor(Parser* parser);

int parser_stoi(char* str);

void parser_compound(Parser* parser);

AST* parser_statement(Parser* parser);

AST* parser_assignment_statement(Parser* parser);

AST* parser_ast_empty();

#endif // !PARSER_H