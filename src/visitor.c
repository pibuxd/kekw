#include "include/visitor.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// start visiting
void visit_compound(Parser* parser)
{
  for(int i = 1; i <= parser->ast_size; i++)
  {
    visit(parser, parser->ast[i], i, parser->global_variables);
  }
}

// visit line
void visit(Parser* parser, AST* ast, int ast_it, Variables* local_variables)
{
  if(ast->token->type == TOKEN_EQUALS)
  {
    visit_assign_var(parser, ast, local_variables);
  }
  else if(ast->token->type == TOKEN_FUNC)
  {
    visit_define_function(parser, ast);
  }
  else if(strcmp(ast->token->value, "if") == 0)
  {
    // TODO
  }
  else
  {
    visit_call_function(parser, ast, local_variables);
  }
}

// returns int from given condition in AST
// !supported operations: +, -, *, /, , >, <
int visit_condition(Parser* parser, AST* ast, Variables* local_variables)
{
  if(ast->token->type == TOKEN_GREATER)
  {
    return visit_expr(parser, ast->left, 0, local_variables) > visit_expr(parser, ast->right, 0, local_variables);
  }
  else if(ast->token->type == TOKEN_LESS)
  {
    return visit_expr(parser, ast->left, 0, local_variables) < visit_expr(parser, ast->right, 0, local_variables);
  }
  else
    return visit_expr(parser, ast, 0, local_variables);
}

// visit expression calculates expr, supports inteegers and variables
int visit_expr(Parser* parser, AST* ast, int curr_val, Variables* local_variables)
{
  if(ast->token->type == TOKEN_PLUS)
  {
    curr_val += visit_expr(parser, ast->right, curr_val, local_variables) + visit_expr(parser, ast->left, curr_val, local_variables);
  }
  else if(ast->token->type == TOKEN_MINUS)
  {
    curr_val += visit_expr(parser, ast->left, curr_val, local_variables) - visit_expr(parser, ast->right, curr_val, local_variables);
  }
  else if(ast->token->type == TOKEN_MUL)
  {
    curr_val += visit_expr(parser, ast->left, curr_val, local_variables) * visit_expr(parser, ast->right, curr_val, local_variables);
  }
  else if(ast->token->type == TOKEN_DIV)
  {
    curr_val += visit_expr(parser, ast->left, curr_val, local_variables) / visit_expr(parser, ast->right, curr_val, local_variables);
  }
  else if(ast->token->type == TOKEN_INT)
  {
    return utils_stoi(ast->token->value);
  }
  else if(ast->token->type == TOKEN_ID)
  {
    return visit_get_var(parser, ast->token->value, local_variables);
  }

  return curr_val;
}

// assigns variable
void visit_assign_var(Parser* parser, AST* ast, Variables* local_variables)
{ 
  char* var_name = ast->left->token->value;
  int var_name_hashed = utils_hash_string(var_name);

  local_variables->values[var_name_hashed] = visit_condition(parser, ast->right, local_variables);
  local_variables->exists[var_name_hashed] = 1;

  free(var_name);
}

// gets variable
int visit_get_var(Parser* parser, char* name, Variables* local_variables)
{
  char* var_name = calloc(strlen(name) + 1, sizeof(char));
  strcpy(var_name, name);
  int var_name_hashed = utils_hash_string(var_name);

  if(local_variables->exists[var_name_hashed] == 1)
  {
    return local_variables->values[var_name_hashed];
  }
  else // global variable
  {
    return parser->global_variables->values[var_name_hashed];
  }
}

// define new function
void visit_define_function(Parser* parser, AST* ast)
{
  // char* func_name = calloc(strlen(ast->token->value)+1, sizeof(char));
  // strcpy(func_name, ast->token->value);
  
  // int func_name_hash = utils_hash_string(func_name);
  // int func_idx = parser->functions_it[func_name_hash];
  return;
}

// call functions
void visit_call_function(Parser* parser, AST* ast, Variables* local_variables)
{
  if(strcmp(ast->token->value, "print") == 0)
  {
    return visit_print_function(parser, ast->right, local_variables);
  }
  
  char* func_name = calloc(strlen(ast->token->value)+1, sizeof(char));
  strcpy(func_name, ast->token->value);

  int func_name_hash = utils_hash_string(func_name);
  int func_idx = parser->functions_it[func_name_hash];

  // assign local variables
  AST *v = ast->right;
  for(int i = 1; i <= parser->functions_args_order_size[func_idx]; i++){
    parser->local_variables[func_idx]->values[parser->functions_args_order[func_idx][i]] = visit_condition(parser, v->left, local_variables);
    parser->local_variables[func_idx]->exists[parser->functions_args_order[func_idx][i]] = 1;
    v = v->right;
  }

  for(int i = 1; i <= parser->func_size[func_idx]; i++)
  {
    visit(parser, parser->functions[func_idx][i], i, parser->local_variables[func_idx]);
  }

  // delete local variables
  v = ast->right;
  for(int i = 1; i <= parser->functions_args_order_size[func_idx]; i++){
    parser->local_variables[func_idx]->values[parser->functions_args_order[func_idx][i]] = 0;
    parser->local_variables[func_idx]->exists[parser->functions_args_order[func_idx][i]] = 0;
    v = v->right;
  }
}

// builtin print function
void visit_print_function(Parser* parser, AST* ast, Variables* local_variables)
{
  if(ast->token->type == TOKEN_STRING)
  {
    printf("%s", ast->token->value);
  }
  else if(ast->token->type == TOKEN_EQUALS)
  { 
    printf("%d", visit_condition(parser, ast->left, local_variables));
  }

  if(ast->right != NULL)
  {
    return visit_print_function(parser, ast->right, local_variables);
  }

  printf("\n");
}