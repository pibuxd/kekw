#include "include/parser.h"
#include "include/utils.h"
#include "include/function.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// create new Parser
Parser* new_parser(Lexer* lexer, Token* token)
{
  // ! indexing starts at 1 (not 0)
  Parser* parser = calloc(1, sizeof(Parser));

  parser->lexer = lexer;
  parser->current_t = token;
  parser->previous_t = NULL;

  parser->ast = calloc(1, sizeof(AST*));
  parser->ast_size = 0;
  parser->global_variables = new_variables();
  
  return parser;
}

void free_parser(Parser* parser)
{
  free_lexer(parser->lexer);
  free_token(parser->current_t);
  free_token(parser->previous_t);
  free_variables(parser->global_variables);

  for(int i = 1; i <= parser->ast_size; i++)
  {
    free_ast(parser->ast[i]);
  }
  free(parser->ast);

  free(parser);
}

Token* parser_peek_token(Parser* parser)
{
  return lexer_peek_next_token(parser->lexer);
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
  if(value == TOKEN_SEMI)
  {
    if(parser_current_token(parser)->type == value)
    {
      parser_get_next_token(parser);
    }

    return;
  }

  if(parser_current_token(parser)->type != value)
  {
    // print error with wrong parsed token
    lexer_print_error(parser->lexer);
    printf("PARSER - expected token: %s, got: %s\n", token_map[value], token_map[parser_current_token(parser)->type]);
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

  switch (token->type)
  {
    case TOKEN_GREATER:
      parser_eat(parser, TOKEN_GREATER);
      break;
    case TOKEN_GREATEREQ:
      parser_eat(parser, TOKEN_GREATEREQ);
      break;
    case TOKEN_LESS:
      parser_eat(parser, TOKEN_LESS);
      break;
    case TOKEN_LESSEQ:
      parser_eat(parser, TOKEN_LESSEQ);
      break;
    case TOKEN_EQ:
      parser_eat(parser, TOKEN_EQ);
      break;
    default:
      return res;
  }
  
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
  if(token->type == TOKEN_LSQUARE)
  {
    return parser_define_function(parser);
  }
  if(token->type == TOKEN_INT)
  {
    parser_eat(parser, TOKEN_INT);
    return new_ast(NULL, NULL, token);
  }
  else if(token->type == TOKEN_CHAR)
  {
    parser_eat(parser, TOKEN_CHAR);
    return new_ast(NULL, NULL, token);
  }
  else if(token->type == TOKEN_STRING)
  {
    parser_eat(parser, TOKEN_STRING);
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
  if(parser_peek_token(parser)->type == TOKEN_EQUALS)
  {
    return parser_assignment_statement(parser);
  }
  else if(strcmp(parser_current_token(parser)->value, "if") == 0)
  {
    return parser_if(parser);
  }
  else if(strcmp(parser_current_token(parser)->value, "while") == 0)
  {
    return parser_while(parser);
  }
  else if(strcmp(parser_current_token(parser)->value, "return") == 0)
  {
    return parser_return(parser);
  }
  else if(lexer_peek_next_token(parser->lexer)->type == TOKEN_LPAREN)
  {
    return parser_call_function(parser);
  }
}

// change AST with return
AST* parser_return(Parser* parser)
{
  parser_eat(parser, TOKEN_ID);
  return new_ast(NULL, parser_condition(parser), new_token(TOKEN_RETURN, "return"));
}

// change AST with new variable or function assignment
AST* parser_assignment_statement(Parser* parser)
{
  Token* var_tok = parser_current_token(parser);
  parser_eat(parser, TOKEN_ID);

  Token* tok = parser_current_token(parser);
  parser_eat(parser, TOKEN_EQUALS);

  return new_ast(new_ast(NULL, NULL, var_tok), parser_condition(parser), tok);
}

// put new function to AST
AST* parser_define_function(Parser* parser)
{
  parser_eat(parser, TOKEN_LSQUARE);
  
  AST* ast = new_ast(NULL, NULL, new_token(TOKEN_FUNC, ""));
  AST* mid2v = ast;
  while(parser_current_token(parser)->type == TOKEN_ID)
  {
    char* arg_name = strdup(parser_current_token(parser)->value);
    mid2v->mid2 = new_ast(NULL, NULL, NULL);
    mid2v = mid2v->mid2;
    mid2v->token = new_token(TOKEN_ID, arg_name);

    parser_eat(parser, TOKEN_ID);
    
    if(parser_current_token(parser)->type == TOKEN_COMMA)
      parser_eat(parser, TOKEN_COMMA);

    free(arg_name);
  }

  parser_eat(parser, TOKEN_RSQUARE);
  parser_eat(parser, TOKEN_LBRACE);

  AST* midv = ast;
  while(parser_current_token(parser)->type != TOKEN_RBRACE)
  {
    midv->mid = new_ast(NULL, NULL, NULL);
    midv = midv->mid;
    midv->right = parser_statement(parser);

    parser_eat(parser, TOKEN_SEMI);
  }
  
  parser_eat(parser, TOKEN_RBRACE);

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

  if(parser_current_token(parser)->type != TOKEN_RPAREN)
  {
    ast->left = parser_condition(parser);
    ast->token = new_token(TOKEN_EQUALS, "=");

    if(parser_current_token(parser)->type == TOKEN_COMMA)
    {
      parser_eat(parser, TOKEN_COMMA);
      ast->right = parser_get_args(parser);
    }
  }

  return ast;
}

// returns AST with parsed if
AST* parser_if(Parser* parser)
{
  AST* ast = new_ast(NULL, NULL, new_token(TOKEN_IF, "if"));

  parser_eat(parser, TOKEN_ID);
  ast->left = parser_condition(parser);
  parser_eat(parser, TOKEN_LBRACE);

  AST* _ast = ast;

  while(parser_current_token(parser)->type != TOKEN_RBRACE)
  {
    _ast->mid = new_ast(NULL, NULL, NULL);
    _ast = _ast->mid;
    _ast->right = parser_statement(parser);
    parser_eat(parser, TOKEN_SEMI);
  }
  
  parser_eat(parser, TOKEN_RBRACE);
  
  return ast;
}

AST* parser_while(Parser* parser)
{
  AST* ast = new_ast(NULL, NULL, new_token(TOKEN_WHILE, "while"));

  parser_eat(parser, TOKEN_ID);
  ast->left = parser_condition(parser);
  parser_eat(parser, TOKEN_LBRACE);

  AST* _ast = ast;

  while(parser_current_token(parser)->type != TOKEN_RBRACE)
  {
    _ast->mid = new_ast(NULL, NULL, NULL);
    _ast = _ast->mid;
    _ast->right = parser_statement(parser);
    parser_eat(parser, TOKEN_SEMI);
  }
  
  parser_eat(parser, TOKEN_RBRACE);
  
  return ast;
}