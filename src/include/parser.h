#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "lexer.h"

typedef struct PARSER_STRUCT
{
  Lexer* lexer;
  Token* current_t;

  // MAIN
  AST** ast;
  unsigned int ast_size;
  int* ids;
  int* ids_type;

  // FUNCTIONS
  AST*** functions;
  unsigned int functions_size;
  unsigned int* func_size;
  int** functions_ids_order;
  int* functions_ids_order_size;
  int* functions_it;
  int** functions_ids;
  int** functions_ids_type;
} Parser;

Parser* new_parser(Lexer* lexer, Token* token);

void parser_get_next_token(Parser* parser);

void parser_eat(Parser* parser, int value);

AST* parser_condition(Parser* parser);

AST* parser_expr(Parser* parser);

AST* parser_term(Parser* parser);

AST* parser_factor(Parser* parser);

void parser_compound(Parser* parser);

void parser_statement(Parser* parser, AST** ast, int i);

AST* parser_assignment_statement(Parser* parser);

void parser_define_function(Parser* parser, int ast_it);

AST* parser_call_function(Parser* parser);

void parser_if(Parser* parser, AST** ast);

AST* parser_get_args(Parser* parser);

int parser_get_var(Parser* pareser, int hashed);

#endif // !PARSER_H