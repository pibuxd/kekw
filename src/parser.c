#include "include/parser.h"
#include "include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

Parser* new_parser(Lexer* lexer, Token* token){
  Parser* parser = calloc(1, sizeof(Parser));
  parser->lexer = lexer;
  parser->current_t = token;
  parser->ast = calloc(1, sizeof(AST));
  parser->ast_size = 1;

  parser->id_type = calloc(1000000, sizeof(int));
  parser->ids = calloc(1000000, sizeof(int));
  return parser;
}

void parser_get_next_token(Parser* parser)
{
  parser->current_t = lexer_get_next_token(parser->lexer);
}

void parser_eat(Parser* parser, int value)
{
  if(parser->current_t->type != value)
  {
    printf("expected token: %d, got: %d\n", value, parser->current_t->type);
    exit(-1);
  }
  parser_get_next_token(parser);
}

AST* parser_expr(Parser* parser)
{
  AST* res = parser_term(parser);

  while(parser->current_t->type == TOKEN_PLUS || parser->current_t->type == TOKEN_MINUS)
  {
    Token* token = parser->current_t;
    if(token->type == TOKEN_PLUS){
      parser_eat(parser, TOKEN_PLUS);
      printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
    }
    else if(token->type == TOKEN_MINUS)
    {
      parser_eat(parser, TOKEN_MINUS);
      printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
    }

    res = new_ast(res, parser_term(parser), token);
  }

  return res;
}

AST* parser_term(Parser* parser)
{
  AST* res = parser_factor(parser);

  while(parser->current_t->type == TOKEN_MUL || parser->current_t->type == TOKEN_DIV)
  {
    Token* token = parser->current_t;
    if(token->type == TOKEN_MUL){
      parser_eat(parser, TOKEN_MUL);
      printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
    }
    else if(token->type == TOKEN_DIV)
    {
      parser_eat(parser, TOKEN_DIV);
      printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
    }

    res = new_ast(res, parser_factor(parser), token);
  }

  return res;
}

AST* parser_factor(Parser* parser)
{
  Token* token = parser->current_t;
  if(token->type == TOKEN_INT)
  {
    parser_eat(parser, TOKEN_INT);
    printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
    return new_ast(NULL, NULL, token);
  }
  else if(token->type == TOKEN_LPAREN)
  {
    parser_eat(parser, TOKEN_LPAREN);
    AST* res = parser_expr(parser);
    parser_eat(parser, TOKEN_RPAREN);
    return res;
  }
  else if(token->type == TOKEN_ID)
  {
    printf("not yet: variable");
  }
  printf("NO FACTOR");
  exit(-1);
}

void parser_compound(Parser* parser)
{
  while(parser->current_t->type != TOKEN_EOF)
  {
    printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
    parser->ast = realloc(parser->ast, parser->ast_size * sizeof(AST));
    parser->ast[parser->ast_size] = parser_statement(parser);
    parser->ast_size += 1;
    parser_eat(parser, TOKEN_SEMI);
  }
  parser_eat(parser, TOKEN_EOF);
}

AST* parser_statement(Parser* parser)
{
  AST* ast = calloc(1, sizeof(AST));
  int hashed_value = utils_hash_string(parser->current_t->value);
  if(strcmp(parser->current_t->value, "var") == 0)
  {
    ast = parser_assignment_statement(parser);
  }
  else
  {
    ast = parser_call_function(parser);
  }

  return ast;
}

AST* parser_assignment_statement(Parser* parser)
{
  AST* ast = calloc(1, sizeof(AST));

  printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
  parser_eat(parser, TOKEN_ID);
  
  ast->left = new_ast(NULL, NULL, parser->current_t);
  printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
  parser_eat(parser, TOKEN_ID);
  printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);

  ast->token = parser->current_t;
  parser_eat(parser, TOKEN_EQUALS);
  printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);

  ast->right = parser_expr(parser);

  return ast;
}

AST* parser_call_function(Parser* parser)
{
  AST* ast = calloc(1, sizeof(AST));

  printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
  parser_eat(parser, TOKEN_ID);
  
  printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
  parser_eat(parser, TOKEN_LPAREN);

  parser_get_args(parser);

  ast->token = parser->current_t;
  parser_eat(parser, TOKEN_RPAREN);
  printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);

  return ast;
}

AST* parser_get_args(Parser* parser)
{
  Token* token = parser->current_t;
  if(lexer_peek(parser->lexer) != ')')
  {
    parser_eat(parser, TOKEN_ID);
    parser_eat(parser, TOKEN_COMMA);
    AST* ast = new_ast(NULL, parser_get_args(parser), token);

    return ast;
  }
  else
  {
    parser_eat(parser, TOKEN_ID);
    AST* ast = new_ast(NULL, NULL, token);
    return ast;
  }  
}

int parser_get_var(Parser* pareser, int hashed){

}