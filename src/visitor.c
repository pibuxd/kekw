#include "include/visitor.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// main function
void visit_compound(Parser* parser)
{
  for(int i = 1; i <= parser->ast_size; i++)
  {
    Return* res = visit(parser, parser->ast[i], parser->global_variables);
    // if "return" had been captured
    if(res->isreturned == 1)
    {
      free(res);
      return;
    }
    free(res);
  }
}

// visit line
Return* visit(Parser* parser, AST* ast, Variables* local_variables)
{
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
    return new_return(1, visit_condition(parser, ast->right, local_variables));
  }
  else if(strcmp(ast->token->value, "if") == 0)
  {
    return visit_if(parser, ast, local_variables);
  }
  else if(ast->token->type == TOKEN_CALL)
  {
    return new_return(0, visit_call_function(parser, ast, local_variables));
  }

  return new_return(0, new_var(0, "int"));
}

// get result from condition - can be int or string
Var* visit_condition(Parser* parser, AST* ast, Variables* local_variables)
{
  switch (ast->token->type)
  {
    case TOKEN_STRING:
      return new_var(new_str(ast->token->value), "str");
    case TOKEN_ID:
    {
      Var* _var = variables_get(local_variables, ast->token->value);
      if(strcmp(_var->type, "str") == 0)
      {
        return _var;
      }
      free(_var);
    }
  }

  return new_var((void*)(intptr_t)visit_condition_int(parser, ast, local_variables), "int");
}

// returns int from given condition in AST
// ! supported operations: +, -, *, /, , >, <, <=, >=, ==
int visit_condition_int(Parser* parser, AST* ast, Variables* local_variables)
{
  switch (ast->token->type)
  {
  case TOKEN_GREATER:
    return visit_expr_int(parser, ast->left, 0, local_variables) > visit_expr_int(parser, ast->right, 0, local_variables);
    break;
  case TOKEN_GREATEREQ:
    return visit_expr_int(parser, ast->left, 0, local_variables) >= visit_expr_int(parser, ast->right, 0, local_variables);
    break;
  case TOKEN_LESS:
    return visit_expr_int(parser, ast->left, 0, local_variables) < visit_expr_int(parser, ast->right, 0, local_variables);
    break;
  case TOKEN_LESSEQ:
    return visit_expr_int(parser, ast->left, 0, local_variables) <= visit_expr_int(parser, ast->right, 0, local_variables);
    break;
  case TOKEN_EQ:
    return visit_expr_int(parser, ast->left, 0, local_variables) == visit_expr_int(parser, ast->right, 0, local_variables);
    break;
  default:
    return visit_expr_int(parser, ast, 0, local_variables);
  }
}

// visit expression calculates expr, supports inteegers and variables
int visit_expr_int(Parser* parser, AST* ast, int curr_val, Variables* local_variables)
{
  switch (ast->token->type)
  {
  case TOKEN_PLUS:
    curr_val += visit_expr_int(parser, ast->right, curr_val, local_variables) + visit_expr_int(parser, ast->left, curr_val, local_variables);
    break;
  case TOKEN_MINUS:
    curr_val += visit_expr_int(parser, ast->left, curr_val, local_variables) - visit_expr_int(parser, ast->right, curr_val, local_variables);
    break;
  case TOKEN_MUL:
    curr_val += visit_expr_int(parser, ast->left, curr_val, local_variables) * visit_expr_int(parser, ast->right, curr_val, local_variables);
    break;
  case TOKEN_DIV:
    curr_val += visit_expr_int(parser, ast->left, curr_val, local_variables) / visit_expr_int(parser, ast->right, curr_val, local_variables);
    break;
  case TOKEN_INT:
    return utils_stoi(ast->token->value);
  case TOKEN_STRING:
    return utils_stoi(ast->token->value);
  case TOKEN_ID:
    return (int)(intptr_t)visit_get_var(parser, ast->token->value, local_variables)->value;
  case TOKEN_CALL:
    return (int)(intptr_t)visit_call_function(parser, ast, local_variables)->value;
  }
  return curr_val;
}

// assigns variable
void visit_assign_var(Parser* parser, AST* ast, Variables* local_variables)
{ 
  char* var_name = strdup(ast->left->token->value);
  Var* cond = visit_condition(parser, ast->right, local_variables);

  if(strcmp(cond->type, "str") == 0)
  {
    // printf("add STRING: %s\n", var_name);
    variables_add(local_variables, var_name, cond->value, "str");
  }
  else if(strcmp(cond->type, "int") == 0)
  {
    // printf("add INT: %s\n", var_name);
    variables_add(local_variables, var_name, cond->value, "int");
  }
  
  free(var_name);
}

// gets variable (function is also variable)
Var* visit_get_var(Parser* parser, char* name, Variables* local_variables)
{
  Var* var = variables_get(local_variables, name);
  if(var->value != NULL)
  {
    return var;
  }
  else // global variable
  {
    return variables_get(parser->global_variables, name);
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
Var* visit_call_function(Parser* parser, AST* ast, Variables* local_variables)
{
  if(strcmp(ast->token->value, "print") == 0)
  {
    return new_var((void*)(intptr_t)visit_print_function(parser, ast->right, local_variables), "int");
  }

  Return* res = new_return(0, new_var(0, "int"));

  char* func_name = strdup(ast->token->value);
  int func_name_hash = utils_hash_string(func_name);
  int func_idx = parser->functions->functions_it[func_name_hash];
  free(func_name);

  // assign local variables passed in arguments
  AST *v = ast->right;
  Variables* func_variables = new_variables();
  for(int i = 1; i <= parser->functions->functions_args_order_size[func_idx]; i++)
  {
    Var* var = visit_condition(parser, v->left, local_variables);
    variables_add(func_variables, parser->functions->functions_args_order[func_idx][i], var->value, var->type);
    v = v->right;
  }

  for(int i = 1; i <= parser->functions->func_size[func_idx]; i++)
  {
    res = visit(parser, parser->functions->functions[func_idx][i], func_variables);
    if(res->isreturned == 1)
    {
      goto ret;
    }
  }

  ret:
  free_variables(func_variables);
  Var* result = res->var;
  free(res);
  return result;
}

// builtin print function (temporary)
int visit_print_function(Parser* parser, AST* ast, Variables* local_variables)
{
  Var* cond = visit_condition(parser, ast->left, local_variables);

  if(strcmp(cond->type, "str") == 0)
  {
    printf("%s", ((Str*)cond->value)->value);
  }
  else if(ast->token->type == TOKEN_EQUALS)
  {
    printf("%d", (intptr_t)cond->value);
  }

  free(cond);
  if(ast->right != NULL)
  {
    return visit_print_function(parser, ast->right, local_variables);
  }

  printf("\n");
  return 0;
}

Return* visit_if(Parser* parser, AST* ast, Variables* local_variables)
{
  if(!visit_condition_int(parser, ast->left, local_variables))
  {
    return new_return(0, new_var(0, "int"));
  }

  AST* _ast = ast;
  while(_ast->mid != NULL)
  {
    _ast = _ast->mid;
    Return* _ret = visit(parser, _ast->right, local_variables);
    if(_ret->isreturned == 1)
    {
      return _ret;
    }
    free(_ret);
  }
  
  return new_return(0, new_var(0, "int"));
}