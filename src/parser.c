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

  printf("token: {%d, %s}\n", parser->current_t->type, parser->current_t->value);
  parser_get_next_token(parser);
}

AST* parser_condition(Parser* parser)
{
  AST* res = parser_expr(parser);

  Token* token = parser->current_t;

  if(token->type == TOKEN_GREATER)
  {
    parser_eat(parser, TOKEN_GREATER);
  }
  else if(token->type == TOKEN_LESS)
  {
    parser_eat(parser, TOKEN_LESS);
  }
  else return res;
  
  return new_ast(res, parser_expr(parser), token);
}

AST* parser_expr(Parser* parser)
{
  AST* res = parser_term(parser);

  while(parser->current_t->type == TOKEN_PLUS || parser->current_t->type == TOKEN_MINUS)
  {
    Token* token = parser->current_t;
    if(token->type == TOKEN_PLUS)
    {
      parser_eat(parser, TOKEN_PLUS);
    }
    else if(token->type == TOKEN_MINUS)
    {
      parser_eat(parser, TOKEN_MINUS);
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
    if(token->type == TOKEN_MUL)
    {
      parser_eat(parser, TOKEN_MUL);
    }
    else if(token->type == TOKEN_DIV)
    {
      parser_eat(parser, TOKEN_DIV);
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
    return new_ast(NULL, NULL, token);
  }
  else if(token->type == TOKEN_ID)
  {
    parser_eat(parser, TOKEN_ID);
    return new_ast(NULL, NULL, token);
  }
  else if(token->type == TOKEN_LPAREN)
  {
    parser_eat(parser, TOKEN_LPAREN);
    AST* res = parser_expr(parser);
    parser_eat(parser, TOKEN_RPAREN);
    return res;
  }

  printf("NO FACTOR");
  exit(-1);
}

void parser_compound(Parser* parser)
{
  while(parser->current_t->type != TOKEN_EOF)
  {
    parser->ast = realloc(parser->ast, parser->ast_size * sizeof(AST));
    parser_statement(parser, parser->ast);
    parser->ast_size += 1;
    parser_eat(parser, TOKEN_SEMI);
  }
  parser_eat(parser, TOKEN_EOF);
}

void parser_statement(Parser* parser, AST** ast)
{
  if(strcmp(parser->current_t->value, "var") == 0)
  {
    ast[parser->ast_size] = parser_assignment_statement(parser);
  }
  else if(strcmp(parser->current_t->value, "if") == 0)
  {
    parser_if(parser, ast);
  }
  else
  {
    ast[parser->ast_size] = parser_call_function(parser);
  }
}

AST* parser_assignment_statement(Parser* parser)
{
  AST* ast = calloc(1, sizeof(AST));

  parser_eat(parser, TOKEN_ID);
  
  ast->left = new_ast(NULL, NULL, parser->current_t);
  parser_eat(parser, TOKEN_ID);

  ast->token = parser->current_t;
  parser_eat(parser, TOKEN_EQUALS);

  ast->right = parser_condition(parser);

  return ast;
}

void parser_if(Parser* parser, AST** ast)
{
  
}

AST* parser_call_function(Parser* parser)
{
  AST* ast = calloc(1, sizeof(AST));

  ast->token = parser->current_t;
  if(parser->current_t->type == TOKEN_ID)
  {
    parser_eat(parser, TOKEN_ID);
  }
  else
  {
    parser_eat(parser, TOKEN_STRING);
  }  
  parser_eat(parser, TOKEN_LPAREN);

  ast->right = parser_get_args(parser);

  parser_eat(parser, TOKEN_RPAREN);

  return ast;
}

AST* parser_get_args(Parser* parser)
{
  Token* token = parser->current_t;
  AST* ast = NULL;

  if(lexer_peek(parser->lexer) != ')')
  {
    if(token->type == TOKEN_STRING)
    {
      parser_eat(parser, TOKEN_STRING);
      parser_eat(parser, TOKEN_COMMA);
      ast = new_ast(NULL, parser_get_args(parser), token);
    }
    else
    {
      ast = new_ast(parser_expr(parser), NULL, new_token(TOKEN_EQUALS, "="));
      parser_eat(parser, TOKEN_COMMA);
      ast->right = parser_get_args(parser);
    }
  }
  else
  {
    if(token->type == TOKEN_STRING)
    {
      parser_eat(parser, TOKEN_STRING);
      ast = new_ast(NULL, NULL, token);
    }
    else
    {
      ast = new_ast(parser_expr(parser), NULL, new_token(TOKEN_EQUALS, "="));
    }
  }  
  return ast;
}