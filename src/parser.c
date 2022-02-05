#include "include/parser.h"
#include "include/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// create new Parser
Parser* new_parser(Lexer* lexer, Token* token)
{
  // ! indexing starts at 1 (not 0)
  Parser* parser = malloc(1*sizeof(Parser));

  parser->lexer = lexer;
  parser->current_t = token;
  parser->previous_t = NULL;

  // MAIN
  parser->ast = malloc(1*sizeof(AST*));
  parser->ast_size = 0;
  parser->global_variables = new_variables();
  
  // FUNCTIONS
  parser->functions = new_functions();

  return parser;
}

void free_parser(Parser* parser)
{
  free_lexer(parser->lexer);
  free_token(parser->current_t);
  free_token(parser->previous_t);
  free_variables(parser->global_variables);
  free_functions(parser->functions);

  for(int i = 1; i <= parser->ast_size; i++)
  {
    free_ast(parser->ast[i]);
  }
  free(parser->ast);

  free(parser);
}

// get next token to parser
void parser_get_next_token(Parser* parser)
{
  parser->previous_t = parser_current_token(parser);
  parser->current_t = lexer_get_next_token(parser->lexer);
}

// returns current Token of Parser
Token* parser_current_token(Parser* parser){
  return parser->current_t;
}

// returns previous Token of Parser
Token* parser_previous_token(Parser* parser){
  return parser->previous_t;
}

// eats token or warn
void parser_eat(Parser* parser, int value)
{
  if(parser_current_token(parser)->type != value)
  {
    // print error with wrong parsed token
    lexer_print_error(parser->lexer);
    printf("PARSER - expected token: %d, got: %d\n", value, parser_current_token(parser)->type);
    exit(-1);
  }

  // print parsed token (FOR DEBUG)
  // printf("PARSER - token: %s\n", parser_current_token(parser)->value);

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

    free_token(token);
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

    if(parser_current_token(parser)->type == TOKEN_LPAREN)
    {
      return parser_call_function(parser);
    }

    return new_ast(NULL, NULL, token);
  }
  else if(token->type == TOKEN_LPAREN)
  {
    parser_eat(parser, TOKEN_LPAREN);
    AST* res = parser_expr(parser);
    parser_eat(parser, TOKEN_RPAREN);
    return res;
  }

  lexer_print_error(parser->lexer);
  printf("PARSER - no factor\n");
  exit(-1);
}

// start parsing file
void parser_compound(Parser* parser)
{
  while(parser_current_token(parser)->type != TOKEN_EOF)
  {
    parser->ast_size += 1;
    parser->ast = realloc(parser->ast, (parser->ast_size+1)*sizeof(AST*));
    parser->ast[parser->ast_size] = parser_statement(parser);
    parser_eat(parser, TOKEN_SEMI);
  }

  parser_eat(parser, TOKEN_EOF);
}

// detect wich statements is it and parse it
AST* parser_statement(Parser* parser)
{
  if(strcmp(parser_current_token(parser)->value, "var") == 0)
  {
    return parser_assignment_statement(parser);
  }
  else if(strcmp(parser_current_token(parser)->value, "if") == 0)
  {
    return parser_if(parser);
  }
  else if(strcmp(parser_current_token(parser)->value, "return") == 0)
  {
    return parser_return(parser);
  }
  else // TODO to change (there can't be else)
  {
    return parser_call_function(parser);
  }
}

// change AST with return
AST* parser_return(Parser* parser)
{
  AST* ast = new_ast(NULL, NULL, NULL);

  parser_eat(parser, TOKEN_ID);

  ast->left = new_ast(NULL, NULL, NULL);
  ast->token = new_token(TOKEN_RETURN, "return");
  ast->right = parser_condition(parser);

  return ast;
}

// change AST with new variable or function assignment
AST* parser_assignment_statement(Parser* parser)
{
  AST* ast = new_ast(NULL, NULL, NULL);
  parser_eat(parser, TOKEN_ID);
  
  Token* var_tok = parser_current_token(parser);
  parser_eat(parser, TOKEN_ID);

  Token* tok = parser_current_token(parser);
  parser_eat(parser, TOKEN_EQUALS);

  // if LPAREN found, jump into defining function
  if(parser_current_token(parser)->type == TOKEN_LPAREN)
  {
    return parser_define_function(parser, var_tok->value);
  }

  ast->left = new_ast(NULL, NULL, var_tok);
  ast->token = tok;
  ast->right = parser_condition(parser);

  return ast;
}

// put new function to parser->functions...
AST* parser_define_function(Parser* parser, char* f_name)
{
  parser->functions->functions_size += 1;
  unsigned int func_idx = parser->functions->functions_size;

  char* func_name = malloc((strlen(f_name)+1)*sizeof(char));
  strcpy(func_name, f_name);
  int func_name_hash = utils_hash_string(func_name);

  AST* ast = new_ast(NULL, NULL, new_token(TOKEN_FUNC, func_name));

  functions_add_new(parser->functions, func_idx, func_name_hash);

  parser_eat(parser, TOKEN_LPAREN);

  while(parser_current_token(parser)->type == TOKEN_ID)
  {
    parser->functions->functions_args_order_size[func_idx] += 1; 

    char* arg_name = malloc((strlen(parser_current_token(parser)->value)+1) * sizeof(char));
    strcpy(arg_name, parser_current_token(parser)->value);
    int arg_name_hash = utils_hash_string(arg_name);

    parser->functions->functions_args_order[func_idx] = realloc(parser->functions->functions_args_order[func_idx], (parser->functions->functions_args_order_size[func_idx]+1)*sizeof(int*));
    parser->functions->functions_args_order[func_idx][parser->functions->functions_args_order_size[func_idx]] = arg_name_hash;

    parser_eat(parser, TOKEN_ID);
    
    if(parser_current_token(parser)->type == TOKEN_COMMA)
      parser_eat(parser, TOKEN_COMMA);

    free(arg_name);
  }

  parser_eat(parser, TOKEN_RPAREN);
  parser_eat(parser, TOKEN_LBRACE);

  while(parser_current_token(parser)->type != TOKEN_RBRACE)
  {
    parser->functions->func_size[func_idx] += 1;
    
    parser->functions->functions[func_idx] = realloc(parser->functions->functions[func_idx], (parser->functions->func_size[func_idx]+1)*sizeof(AST*));
    parser->functions->functions[func_idx][parser->functions->func_size[func_idx]] = parser_statement(parser);
    parser_eat(parser, TOKEN_SEMI);
  }

  parser_eat(parser, TOKEN_RBRACE);

  free(func_name);
  return ast;
}

// returns parsed whole void function
AST* parser_call_function(Parser* parser)
{
  AST* ast = new_ast(NULL, NULL, NULL);

  if(parser_current_token(parser)->type == TOKEN_ID)
  {
    parser_eat(parser, TOKEN_ID);
  }

  ast->token = new_token(TOKEN_CALL, parser_previous_token(parser)->value);
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
      ast->left = parser_condition(parser);
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
AST* parser_if(Parser* parser)
{
  AST* ast = new_ast(NULL, NULL, NULL);

  parser_eat(parser, TOKEN_ID);

  ast->token = new_token(TOKEN_IF, "if");

  ast->left = parser_condition(parser);
  parser_eat(parser, TOKEN_LBRACE);
  ast->right = parser_statement(parser);
  parser_eat(parser, TOKEN_SEMI);
  parser_eat(parser, TOKEN_RBRACE);

  return ast;
}
