#include "include/parser.h"
#include "include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// create new Parser
Parser* new_parser(Lexer* lexer, Token* token)
{
  Parser* parser = calloc(1, sizeof(Parser));

  parser->lexer = lexer;
  parser->current_t = token;

  // MAIN
  parser->ast = calloc(2, sizeof(AST));
  parser->ast_size = 1;
  parser->ids = calloc(1000000, sizeof(int));
  parser->ids_exi = calloc(1000000, sizeof(int));
  parser->ids_type = calloc(1000000, sizeof(int));
  
  // FUNCTIONS iterating starts at 1
  parser->functions = calloc(2, sizeof(AST));
  parser->functions_size = 1;
  parser->func_size = calloc(2, sizeof(unsigned int));
  parser->functions_ids_order = calloc(2, sizeof(int));
  parser->functions_ids_order_size = calloc(2, sizeof(int));
  parser->functions_it = calloc(1000000, sizeof(int));
  parser->functions_ids = calloc(2, sizeof(int));
  parser->functions_ids_exi = calloc(2, sizeof(int));
  parser->functions_ids_type = calloc(2, sizeof(int));

  return parser;
}

// get next token to parser
void parser_get_next_token(Parser* parser)
{
  parser->current_t = lexer_get_next_token(parser->lexer);
}

// returns current Token of Parser
Token* parser_current_token(Parser* parser){
  return parser->current_t;
}

// eats token or warn
void parser_eat(Parser* parser, int value)
{
  if(parser_current_token(parser)->type != value)
  {
    printf("expected token: %d, got: %d\n", value, parser_current_token(parser)->type);
    exit(-1);
  }

  // printf("token: {%d, %s}\n", parser_current_token(parser)->type, parser_current_token(parser)->value);
  parser_get_next_token(parser);
}

// returns AST with parsed condition
// CONDITION = EXPR > or < EXPR
AST* parser_condition(Parser* parser)
{
  AST* res = parser_expr(parser);

  Token* token = parser_current_token(parser);

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

// returns AST with parsed expression
// EXPR = TERM + or - TERM
AST* parser_expr(Parser* parser)
{
  AST* res = parser_term(parser);

  while(parser_current_token(parser)->type == TOKEN_PLUS || parser_current_token(parser)->type == TOKEN_MINUS)
  {
    Token* token = parser_current_token(parser);
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

// returns AST with parsed term
// TERM = FACTOR * or / FACTOR
AST* parser_term(Parser* parser)
{
  AST* res = parser_factor(parser);

  while(parser_current_token(parser)->type == TOKEN_MUL || parser_current_token(parser)->type == TOKEN_DIV)
  {
    Token* token = parser_current_token(parser);
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

// return AST with parsed int or variable in expression
// FACTOR = INT or ID or ( EXPR )
AST* parser_factor(Parser* parser)
{
  Token* token = parser_current_token(parser);
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

// start parsing file
void parser_compound(Parser* parser)
{
  while(parser_current_token(parser)->type != TOKEN_EOF)
  {
    parser->ast = realloc(parser->ast, (parser->ast_size+1) * sizeof(AST));
    parser_statement(parser, parser->ast, parser->ast_size);
    parser->ast_size += 1;
    parser_eat(parser, TOKEN_SEMI);
  }
  parser_eat(parser, TOKEN_EOF);
}

// detect wich statements is it and parse it
void parser_statement(Parser* parser, AST** ast, int i)
{
  if(strcmp(parser_current_token(parser)->value, "var") == 0)
  {
    parser_assignment_statement(parser, ast, i);
  }
  else if(strcmp(parser_current_token(parser)->value, "if") == 0)
  {
    parser_if(parser, ast);
  }
  else // TODO to change (there can't be else)
  {
    ast[i] = parser_call_function(parser);
  }
}

// change AST with new variable or function assignment
void parser_assignment_statement(Parser* parser, AST** ast, int i)
{
  ast[i] = calloc(1, sizeof(AST));
  parser_eat(parser, TOKEN_ID);
  
  Token* var_tok = parser_current_token(parser);
  parser_eat(parser, TOKEN_ID);

  Token* tok = parser_current_token(parser);
  parser_eat(parser, TOKEN_EQUALS);

  // if LPAREN found, jump into defining function
  if(parser_current_token(parser)->type == TOKEN_LPAREN)
  {
    return parser_define_function(parser, i, var_tok->value);
  }
  ast[i]->left = new_ast(NULL, NULL, var_tok);
  ast[i]->token = tok;
  ast[i]->right = parser_condition(parser);
}

// put new function to parser->functions...
void parser_define_function(Parser* parser, int ast_it, char* f_name)
{
  char* func_name = calloc(strlen(f_name)+1, sizeof(char));
  strcpy(func_name, f_name);
  int func_name_hash = utils_hash_string(func_name);

  parser->ast[ast_it] = new_ast(NULL, NULL, new_token(TOKEN_FUNC, func_name));

  parser->functions_it = realloc(parser->functions_it, (parser->functions_size+1)*sizeof(int));
  parser->functions_it[func_name_hash] = parser->functions_size;
  parser->functions_ids_order = realloc(parser->functions_ids_order, (parser->functions_size+1)*sizeof(AST));
  parser->functions_ids = realloc(parser->functions_ids, (parser->functions_size+1)*sizeof(AST));
  parser->functions_ids[parser->functions_size] = calloc(1000000, sizeof(int));
  parser->functions_ids_exi[parser->functions_size] = calloc(1000000, sizeof(int));
  parser->functions_ids_order[parser->functions_size] = calloc(2, sizeof(int));
  parser->functions_ids_order_size[parser->functions_size] = 1;

  parser_eat(parser, TOKEN_LPAREN);

  while(parser_current_token(parser)->type == TOKEN_ID)
  {
    char* arg_name = calloc(strlen(parser_current_token(parser)->value) + 1, sizeof(char));
    strcpy(arg_name, parser_current_token(parser)->value);
    int arg_name_hash = utils_hash_string(arg_name);

    parser->functions_ids_order[parser->functions_size] = realloc(parser->functions_ids_order[parser->functions_size], (parser->functions_ids_order_size[parser->functions_size]+1)*sizeof(int));
    parser->functions_ids_order[parser->functions_size][parser->functions_ids_order_size[parser->functions_size]] = arg_name_hash;

    parser_eat(parser, TOKEN_ID);
    
    parser->functions_ids_order_size[parser->functions_size] += 1; 
    if(parser_current_token(parser)->type == TOKEN_COMMA)
      parser_eat(parser, TOKEN_COMMA);
  }

  parser_eat(parser, TOKEN_RPAREN);
  parser_eat(parser, TOKEN_LBRACE);

  parser->functions = realloc(parser->functions, (parser->functions_size)+1*sizeof(AST));

  while(parser_current_token(parser)->type != TOKEN_RBRACE)
  {
    parser->functions[parser->functions_size] = realloc(parser->functions[parser->functions_size], (parser->func_size[parser->functions_size]+1)*sizeof(AST));
    parser_statement(parser, parser->functions[parser->functions_size], parser->func_size[parser->functions_size]+1);
    parser_eat(parser, TOKEN_SEMI);
    parser->func_size[parser->functions_size] += 1;
  }

  parser_eat(parser, TOKEN_RBRACE);
  parser->functions_size += 1;
}

// returns parsed whole void function
AST* parser_call_function(Parser* parser)
{
  AST* ast = new_ast(NULL, NULL, NULL);
  ast->token = parser_current_token(parser);

  if(parser_current_token(parser)->type == TOKEN_ID)
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

// returns arguments from called function to AST
AST* parser_get_args(Parser* parser)
{
  AST* ast = new_ast(NULL, NULL, NULL);
  Token* token = parser_current_token(parser);

  if(parser_current_token(parser)->type != TOKEN_RPAREN)
  {
    if(token->type == TOKEN_STRING)
    {
      parser_eat(parser, TOKEN_STRING);
      ast->token = token;
      if(parser_current_token(parser)->type == TOKEN_COMMA)
      {
      parser_eat(parser, TOKEN_COMMA);
      ast->right = parser_get_args(parser);
      }
    }
    else
    {
      ast->left = parser_expr(parser);
      ast->token = new_token(TOKEN_EQUALS, "=");

      if(parser_current_token(parser)->type == TOKEN_COMMA)
      {
      parser_eat(parser, TOKEN_COMMA);
      ast->right = parser_get_args(parser);
      }
    }
  }

  return ast;
}

// TODO: parse if
void parser_if(Parser* parser, AST** ast)
{
  
}
