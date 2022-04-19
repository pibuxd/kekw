#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "lexer.h"
#include "variables.h"
#include "functions.h"
#include "string.h"
typedef struct PARSER_STRUCT
{
  Lexer* lexer;
  Token* current_t;
  Token* previous_t;

  // MAIN
  AST** ast;
  unsigned int ast_size;
  Variables* global_variables;

  // FUNCTIONS
  Functions* functions;
} Parser;

Parser* new_parser(Lexer* lexer, Token* token);

void free_parser(Parser* parser);

void parser_get_next_token(Parser* parser);

Token* parser_current_token(Parser* parser);

Token* parser_previous_token(Parser* parser);

void parser_eat(Parser* parser, int value);

AST* parser_condition(Parser* parser);

AST* parser_expr(Parser* parser);

AST* parser_term(Parser* parser);

AST* parser_factor(Parser* parser);

void parser_compound(Parser* parser);

AST* parser_statement(Parser* parser);

AST* parser_return(Parser* parser);

AST* parser_assignment_statement(Parser* parser);

AST* parser_define_function(Parser* parser, char* f_name);

AST* parser_call_function(Parser* parser);

AST* parser_if(Parser* parser);

AST* parser_get_args(Parser* parser);

int parser_get_var(Parser* pareser, int hashed);

#endif // !PARSER_H