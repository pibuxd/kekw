#include "include/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

Parser* new_parser(Lexer* lexer, Token* token){
  Parser* parser = calloc(1, sizeof(Parser));
  parser->lexer = lexer;
  parser->current_t = token;

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

int parser_expr(Parser* parser)
{
  int res = parser_term(parser);

  while(parser->current_t->type == TOKEN_PLUS || parser->current_t->type == TOKEN_MINUS)
  {
    Token* token = parser->current_t;
    if(token->type == TOKEN_PLUS){
      parser_eat(parser, TOKEN_PLUS);
      res = res + parser_term(parser);
    }
    else if(token->type == TOKEN_MINUS)
    {
      parser_eat(parser, TOKEN_MINUS);
      res = res - parser_term(parser);
    }
  }

  return res;
}

int parser_term(Parser* parser)
{
  int res = parser_factor(parser);

  while(parser->current_t->type == TOKEN_MUL || parser->current_t->type == TOKEN_DIV)
  {
    Token* token = parser->current_t;
    if(token->type == TOKEN_MUL){
      parser_eat(parser, TOKEN_MUL);
      res = res * parser_factor(parser);
    }
    else if(token->type == TOKEN_DIV)
    {
      parser_eat(parser, TOKEN_DIV);
      res = res / parser_factor(parser);
    }
  }

  return res;
}

int parser_factor(Parser* parser)
{
  Token* token = parser->current_t;
  if(token->type == TOKEN_INT)
  {
    parser_eat(parser, TOKEN_INT);
    int integer = parser_stoi(token->value);
    return integer;
  }
  else if(token->type == TOKEN_LPAREN)
  {
    parser_eat(parser, TOKEN_LPAREN);
    int res = parser_expr(parser);
    parser_eat(parser, TOKEN_RPAREN);
    return res;
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