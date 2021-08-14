#include "include/parser.h"
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
      // res = res + parser_term(parser);
    }
    else if(token->type == TOKEN_MINUS)
    {
      parser_eat(parser, TOKEN_MINUS);
      printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
      // res = res - parser_term(parser);
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
      // res = res * parser_factor(parser);
    }
    else if(token->type == TOKEN_DIV)
    {
      parser_eat(parser, TOKEN_DIV);
      printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
      // res = res / parser_factor(parser);
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
    // int integer = parser_stoi(token->value);
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

int parser_stoi(char* str){
  int res = 0;

  int temp = 1;
  for(int i = strlen(str)-1; i >= 0; i--)
  {
    int factor = (int)(str[i] - '0');
    res += factor * temp;
    temp *= 10;
  }

  return res;
}

void parser_compound(Parser* parser)
{
  parser->ast = realloc(parser->ast, parser->ast_size * sizeof(AST));
  parser->ast[parser->ast_size] = parser_statement(parser);
  parser->ast_size += 1;

  while(parser->current_t->type == TOKEN_SEMI)
  {
    parser_eat(parser, TOKEN_SEMI);
    printf("token: %d, %s\n", parser->current_t->type, parser->current_t->value);
    parser->ast = realloc(parser->ast, parser->ast_size * sizeof(AST));
    parser->ast[parser->ast_size] = parser_statement(parser);
    parser->ast_size += 1;
  }
}

AST* parser_statement(Parser* parser)
{
  AST* ast = calloc(1, sizeof(AST));
  
  if(parser->current_t->type == TOKEN_ID)
  {
    ast = parser_assignment_statement(parser);
  }
  else
  {
    ast = parser_ast_empty();
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

AST* parser_ast_empty()
{
  AST* ast = new_ast(NULL, NULL, NULL);
  return ast;
}