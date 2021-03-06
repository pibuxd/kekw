#include "include/visitor.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/function.h"

// main function (starts)
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

// visit new
Return* visit(Parser* parser, AST* ast, Variables* local_variables)
{
  switch (ast->token->type)
  {
    case TOKEN_EQUALS:
      visit_assign_var(parser, ast, local_variables);
      break;
    case TOKEN_FUNC:
      visit_define_function(parser, ast, local_variables);
      break;
    case TOKEN_RETURN:
      return new_return(1, visit_condition(parser, ast->right, local_variables));
    case TOKEN_IF:
      return visit_if(parser, ast, local_variables);
    case TOKEN_WHILE:
      return visit_while(parser, ast, local_variables);
    case TOKEN_CALL:
      return new_return(0, visit_call_function(parser, ast, local_variables));
  }

  return new_return(0, new_var(0, VAR_INT));
}

// returns Var from given condition in AST
// ! supported operators: +, -, *, /, , >, <, <=, >=, ==
// ! operators works only on int and char
Var* visit_condition(Parser* parser, AST* ast, Variables* local_variables)
{
  switch (ast->token->type)
  {
  case TOKEN_GREATER:
  {
    Var* ex = visit_expr(parser, ast->left, 0, local_variables);
    return new_var((void*)(intptr_t)((intptr_t)ex->value > (intptr_t)visit_expr(parser, ast->right, 0, local_variables)->value), ex->type);
  }
  case TOKEN_GREATEREQ:
  {
    Var* ex = visit_expr(parser, ast->left, 0, local_variables);
    return new_var((void*)(intptr_t)((intptr_t)ex->value >= (intptr_t)visit_expr(parser, ast->right, 0, local_variables)->value), ex->type);
  }
  case TOKEN_LESS:
  {
    Var* ex = visit_expr(parser, ast->left, 0, local_variables);
    return new_var((void*)(intptr_t)((intptr_t)ex->value < (intptr_t)visit_expr(parser, ast->right, 0, local_variables)->value), ex->type);
  }
  case TOKEN_LESSEQ:
  {
    Var* ex = visit_expr(parser, ast->left, 0, local_variables);
    return new_var((void*)(intptr_t)((intptr_t)ex->value <= (intptr_t)visit_expr(parser, ast->right, 0, local_variables)->value), ex->type);
  }
  case TOKEN_EQ:
  {
    Var* ex = visit_expr(parser, ast->left, 0, local_variables);
    return new_var((void*)(intptr_t)((intptr_t)ex->value == (intptr_t)visit_expr(parser, ast->right, 0, local_variables)->value), ex->type);
  }
  default:
    return visit_expr(parser, ast, 0, local_variables);
  }
}

// visit expression calculates expr, supports inteegers and variables
Var* visit_expr(Parser* parser, AST* ast, int curr_val, Variables* local_variables)
{
  switch (ast->token->type)
  {
  case TOKEN_PLUS:
    curr_val += (intptr_t)visit_expr(parser, ast->left, curr_val, local_variables)->value + (intptr_t)visit_expr(parser, ast->right, curr_val, local_variables)->value;
    break;
  case TOKEN_MINUS:
    curr_val += (intptr_t)visit_expr(parser, ast->left, curr_val, local_variables)->value - (intptr_t)visit_expr(parser, ast->right, curr_val, local_variables)->value;
    break;
  case TOKEN_MUL:
    curr_val += (intptr_t)visit_expr(parser, ast->left, curr_val, local_variables)->value * (intptr_t)visit_expr(parser, ast->right, curr_val, local_variables)->value;
    break;
  case TOKEN_DIV:
    curr_val += (intptr_t)visit_expr(parser, ast->left, curr_val, local_variables)->value / (intptr_t)visit_expr(parser, ast->right, curr_val, local_variables)->value;
    break;
  case TOKEN_INT:
    return new_var((void*)(intptr_t)atoi(ast->token->value), VAR_INT);
  case TOKEN_CHAR:
    return new_var((void*)(intptr_t)ast->token->value[0], VAR_CHAR);
  case TOKEN_STRING:
    return new_var(new_str(ast->token->value), VAR_STR);
  case TOKEN_ID:
    return visit_get_var(parser, ast->token->value, local_variables);
  case TOKEN_CALL:
    return visit_call_function(parser, ast, local_variables);
  case TOKEN_FUNC:
    return visit_define_function(parser, ast, local_variables);
  }

  return new_var((void*)(intptr_t)curr_val, VAR_INT);
}

// assigns Var
void visit_assign_var(Parser* parser, AST* ast, Variables* local_variables)
{ 
  char* var_name = strdup(ast->left->token->value);
  Var* cond = visit_condition(parser, ast->right, local_variables);

  switch (cond->type)
  {
    case VAR_FUNC:
      variables_add(local_variables, var_name, cond->value, VAR_FUNC);
      break;
    case VAR_CHAR:
      variables_add(local_variables, var_name, cond->value, VAR_CHAR);
      break;
    case VAR_INT:
      variables_add(local_variables, var_name, cond->value, VAR_INT);
      break;
    case VAR_STR:
      variables_add(local_variables, var_name, cond->value, VAR_STR);
      break;
  }
  
  free(var_name);
}

// gets Var
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
Var* visit_define_function(Parser* parser, AST* ast, Variables* local_variables)
{
  Function* funcv = new_function();
  funcv->contents = ast;
  new_var(funcv, VAR_FUNC);
}

// call function
Var* visit_call_function(Parser* parser, AST* ast, Variables* local_variables)
{
  if(strcmp(ast->token->value, "print") == 0)
  {
    return new_var((void*)(intptr_t)visit_print_function(parser, ast->right, local_variables), VAR_INT);
  }
  else if(strcmp(ast->token->value, "char") == 0)
  {
    return visit_char_function(parser, ast->right, local_variables);
  }
  else if(strcmp(ast->token->value, "int") == 0)
  {
    return visit_int_function(parser, ast->right, local_variables);
  }
  else if(strcmp(ast->token->value, "type") == 0)
  {
    return visit_type_function(parser, ast->right, local_variables);
  }

  Return* res = new_return(0, new_var(0, VAR_INT));
  Variables* variables = new_variables();

  char* func_name = strdup(ast->token->value);
  Var* funcv = visit_get_var(parser, func_name, local_variables);

  // assign local variables passed in arguments
  AST* v = ast->right;
  AST* mid2v = ((Function*)funcv->value)->contents;
  while(mid2v->mid2 != NULL)
  {
    mid2v = mid2v->mid2;
    Var* var = visit_condition(parser, v->left, local_variables);
    variables_add(variables, mid2v->token->value, var->value, var->type);
    v = v->right;
  }

  AST* midv = ((Function*)funcv->value)->contents;
  while(midv->mid != NULL)
  { 
    midv = midv->mid;

    res = visit(parser, midv->right, variables);
    if(res->isreturned == 1)
    {
      goto ret;
    }
  }

  ret:
  free_variables(variables);
  Var* result = res->var;

  free(func_name);
  return result;
}

// builtin print function (temporary), passes unlimited arguments
int visit_print_function(Parser* parser, AST* ast, Variables* local_variables)
{
  Var* cond = visit_condition(parser, ast->left, local_variables);
  
  if(cond->type == VAR_STR)
  {
    printf("%s", ((Str*)cond->value)->value);
  }
  else if(cond->type == VAR_CHAR)
  {
    printf("%c", cond->value);
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

// builtin int function
// * int function forces condition to be int
Var* visit_int_function(Parser* parser, AST* ast, Variables* local_variables)
{
  Var* cond = visit_condition(parser, ast->left, local_variables);
  cond->type = VAR_INT;
  return cond;
}

// * char function forces condition to be char
Var* visit_char_function(Parser* parser, AST* ast, Variables* local_variables)
{
  Var* cond = visit_condition(parser, ast->left, local_variables);
  cond->type = VAR_CHAR;
  return cond;
}

Var* visit_type_function(Parser* parser, AST* ast, Variables* local_variables)
{
  Var* cond = visit_condition(parser, ast->left, local_variables);
  cond->value = new_str(var_map2[cond->type]);
  cond->type = VAR_STR;
  return cond;
}

// just if
Return* visit_if(Parser* parser, AST* ast, Variables* local_variables)
{
  if((intptr_t)visit_condition(parser, ast->left, local_variables)->value == 0)
  {
    return new_return(0, new_var(0, VAR_INT));
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
  
  return new_return(0, new_var(0, VAR_INT));
}

// while
Return* visit_while(Parser* parser, AST* ast, Variables* local_variables)
{
  while((intptr_t)visit_condition(parser, ast->left, local_variables)->value != 0)
  {
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
  }

  
  return new_return(0, new_var(0, VAR_INT));
}