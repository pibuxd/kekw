#include "include/visitor.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void visit_compound(Parser* parser)
{
  for(int i = 1; i < parser->ast_size; i++)
  {
    visit(parser, parser->ast[i], i);
  }
}

void visit(Parser* parser, AST* ast, int i)
{
  if(ast->token->type == TOKEN_EQUALS)
  {
    visit_assign_var(parser, ast);
  }
  else if(strcmp(ast->token->value, "if") == 0)
  {

  }
  else
  {
    visit_call_function(parser, ast);
  }
}

int visit_condition(Parser* parser, AST* ast)
{
  if(ast->token->type == TOKEN_GREATER)
  {
    return visit_expr(parser, ast->left, 0) > visit_expr(parser, ast->right, 0);
  }
  else if(ast->token->type == TOKEN_LESS)
  {
    return visit_expr(parser, ast->left, 0) < visit_expr(parser, ast->right, 0);
  }
  else
    return visit_expr(parser, ast, 0);
}

int visit_expr(Parser* parser, AST* ast, int val)
{
  if(ast->token->type == TOKEN_PLUS)
  {
    val += visit_expr(parser, ast->right, val) + visit_expr(parser, ast->left, val);
  }
  else if(ast->token->type == TOKEN_MINUS)
  {
    val += visit_expr(parser, ast->left, val) - visit_expr(parser, ast->right, val);
  }
  else if(ast->token->type == TOKEN_MUL)
  {
    val += visit_expr(parser, ast->left, val) * visit_expr(parser, ast->right, val);
  }
  else if(ast->token->type == TOKEN_DIV)
  {
    val += visit_expr(parser, ast->left, val) / visit_expr(parser, ast->right, val);
  }
  else if(ast->token->type == TOKEN_INT)
  {
    return utils_stoi(ast->token->value);
  }
  else if(ast->token->type == TOKEN_ID)
  {
    return visit_get_var(parser, ast->token->value);
  }

  return val;
}

void visit_assign_var(Parser* parser, AST* ast)
{
  char* var_name = ast->left->token->value;
  int var_name_hashed = utils_hash_string(var_name);
  parser->ids[var_name_hashed] = visit_condition(parser, ast->right);
}

int visit_get_var(Parser* parser, char* name)
{
  char* var_name = calloc(strlen(name) + 1, sizeof(char));
  strcpy(var_name, name);
  int var_name_hashed = utils_hash_string(var_name);
  return parser->ids[var_name_hashed];
}

void visit_call_function(Parser* parser, AST* ast)
{
  if(strcmp(ast->token->value, "print") == 0)
  {
    visit_print_function(parser, ast->right);
  }
}

void visit_print_function(Parser* parser, AST* ast)
{
  if(ast->token->type == TOKEN_ID)
  {
    printf("%d\n", visit_get_var(parser, ast->token->value));
  }
  else if(ast->token->type == TOKEN_STRING)
  {
    printf("%s\n", ast->token->value);
  }
  else if(ast->token->type == TOKEN_EQUALS)
  { 
    printf("%d\n", visit_expr(parser, ast->left, 0));
  }

  if(ast->right != NULL)
  {
    visit_print_function(parser, ast->right);
  }
}