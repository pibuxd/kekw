#include "include/visitor.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// main function
void visit_compound(Parser* parser)
{
  int* res = calloc(2, sizeof(int));
  res[0] = 0, res[1] = 0;

  for(int i = 1; i <= parser->ast_size; i++)
  {
    res = visit(parser, parser->ast[i], i, parser->global_variables);
    // if "return" captured
    if(res[0] == 1)
    {
      return;
    }
  }
}

// visit line
int* visit(Parser* parser, AST* ast, int ast_it, Variables* local_variables)
{
  int* res = calloc(2, sizeof(int));
  res[0] = 0, res[1] = 0;

  if(ast->token->type == TOKEN_EQUALS)
  {
    visit_assign_var(parser, ast, local_variables);
  }
  else if(ast->token->type == TOKEN_FUNC)
  {
    visit_define_function(parser, ast);
  }
  else if(ast->token->type == TOKEN_RETURN)
  {
    res[0] = 1;
    res[1] = visit_condition(parser, ast->right, local_variables);
  }
  else if(strcmp(ast->token->value, "if") == 0)
  {
    // TODO
  }
  else if(ast->token->type == TOKEN_CALL)
  {
    visit_call_function(parser, ast, local_variables);
  }

  return res;
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
  else if(ast->token->type == TOKEN_CALL)
  {
    return visit_call_function(parser, ast, local_variables);
  }

  return curr_val;
}

// assigns variable
void visit_assign_var(Parser* parser, AST* ast, Variables* local_variables)
{ 
  char* var_name = ast->left->token->value;
  int var_name_hashed = utils_hash_string(var_name);
  variables_add_new(local_variables, var_name_hashed, visit_condition(parser, ast->right, local_variables));
}

// gets variable (function is also variable)
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
int visit_call_function(Parser* parser, AST* ast, Variables* local_variables)
{
  int* res = calloc(2, sizeof(int));
  res[0] = 0, res[1] = 0;

  if(strcmp(ast->token->value, "print") == 0)
  {
    return visit_print_function(parser, ast->right, local_variables);
  }

  char* func_name = calloc(strlen(ast->token->value)+1, sizeof(char));
  strcpy(func_name, ast->token->value);

  int func_name_hash = utils_hash_string(func_name);
  int func_idx = parser->functions->functions_it[func_name_hash];

  // for recursive
  // variables_delete_all(parser->functions->local_variables[func_idx]);

  // assign local variables passed in arguments
  AST *v = ast->right;
  for(int i = 1; i <= parser->functions->functions_args_order_size[func_idx]; i++){
    variables_add_new(parser->functions->local_variables[func_idx], parser->functions->functions_args_order[func_idx][i], visit_condition(parser, v->left, local_variables));
    v = v->right;
  }

  for(int i = 1; i <= parser->functions->func_size[func_idx]; i++)
  {
    res = visit(parser, parser->functions->functions[func_idx][i], i, parser->functions->local_variables[func_idx]);
    
    if(res[0] == 1)
    {
      goto ret;
    }
  }

  // delete all local variables
  ret:
  variables_delete_all(parser->functions->local_variables[func_idx]);
  return res[1];
}

// builtin print function
int visit_print_function(Parser* parser, AST* ast, Variables* local_variables)
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
  return 0;
}