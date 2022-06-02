#include "include/visitor.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/function.h"

// main function (starts)
void visit_compound(Parser* parser)
{
  Scope* scope = new_scope();
  scope_insert(scope, parser->global_variables);

  for(int i = 1; i <= parser->ast_size; i++)
  {
    Return* res = visit(parser, parser->ast[i], scope);
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
Return* visit(Parser* parser, AST* ast, Scope* scope)
{
  switch (ast->token->type)
  {
    case TOKEN_EQUALS:
      visit_assign_var(parser, ast, scope);
      break;
    case TOKEN_FUNC: // maybe useless
      visit_define_function(parser, ast, scope);
      break;
    case TOKEN_RETURN:
      return new_return(1, visit_condition(parser, ast->right, scope));
    case TOKEN_IF:
      return visit_if(parser, ast, scope);
    case TOKEN_WHILE:
      return visit_while(parser, ast, scope);
    case TOKEN_CALL:
      int cnt = 1;
      Return* rr = new_return(0, visit_call_function(parser, ast, scope, new_var(0, VAR_NONE)));
      if(rr->var->type != VAR_FUNC)
      {
        cnt = 0;
      }
      while(rr->var->type == VAR_FUNC)
      {
        cnt += 1;
        ast = ast->mid;
        rr = new_return(0, visit_call_function(parser, ast, scope, rr->var));
      }
      while(cnt--)
      {
        scope_pop(scope);
      }
      return rr;

  }

  return new_return(0, new_var(0, VAR_INT));
}

// returns Var from given condition in AST
// ! supported operators: +, -, *, /, , >, <, <=, >=, ==
// ! operators works only on int and char
Var* visit_condition(Parser* parser, AST* ast, Scope* scope)
{
  switch (ast->token->type)
  {
  case TOKEN_GREATER:
  {
    Var* ex = visit_expr(parser, ast->left, 0, scope);
    return new_var((void*)(intptr_t)((intptr_t)ex->value > (intptr_t)visit_expr(parser, ast->right, 0, scope)->value), ex->type);
  }
  case TOKEN_GREATEREQ:
  {
    Var* ex = visit_expr(parser, ast->left, 0, scope);
    return new_var((void*)(intptr_t)((intptr_t)ex->value >= (intptr_t)visit_expr(parser, ast->right, 0, scope)->value), ex->type);
  }
  case TOKEN_LESS:
  {
    Var* ex = visit_expr(parser, ast->left, 0, scope);
    return new_var((void*)(intptr_t)((intptr_t)ex->value < (intptr_t)visit_expr(parser, ast->right, 0, scope)->value), ex->type);
  }
  case TOKEN_LESSEQ:
  {
    Var* ex = visit_expr(parser, ast->left, 0, scope);
    return new_var((void*)(intptr_t)((intptr_t)ex->value <= (intptr_t)visit_expr(parser, ast->right, 0, scope)->value), ex->type);
  }
  case TOKEN_EQ:
  {
    Var* ex = visit_expr(parser, ast->left, 0, scope);
    return new_var((void*)(intptr_t)((intptr_t)ex->value == (intptr_t)visit_expr(parser, ast->right, 0, scope)->value), ex->type);
  }
  default:
    return visit_expr(parser, ast, 0, scope);
  }
}

// visit expression calculates expr, supports inteegers and variables
Var* visit_expr(Parser* parser, AST* ast, int curr_val, Scope* scope)
{
  switch (ast->token->type)
  {
  case TOKEN_PLUS:
    curr_val += (intptr_t)visit_expr(parser, ast->left, curr_val, scope)->value + (intptr_t)visit_expr(parser, ast->right, curr_val, scope)->value;
    break;
  case TOKEN_MINUS:
    curr_val += (intptr_t)visit_expr(parser, ast->left, curr_val, scope)->value - (intptr_t)visit_expr(parser, ast->right, curr_val, scope)->value;
    break;
  case TOKEN_MUL:
    curr_val += (intptr_t)visit_expr(parser, ast->left, curr_val, scope)->value * (intptr_t)visit_expr(parser, ast->right, curr_val, scope)->value;
    break;
  case TOKEN_DIV:
    curr_val += (intptr_t)visit_expr(parser, ast->left, curr_val, scope)->value / (intptr_t)visit_expr(parser, ast->right, curr_val, scope)->value;
    break;
  case TOKEN_INT:
    return new_var((void*)(intptr_t)atoi(ast->token->value), VAR_INT);
  case TOKEN_CHAR:
    return new_var((void*)(intptr_t)ast->token->value[0], VAR_CHAR);
  case TOKEN_STRING:
    return new_var(new_str(ast->token->value), VAR_STR);
  case TOKEN_ID:
    return visit_get_var(parser, ast->token->value, scope);
  case TOKEN_CALL:
    int cnt = 0;
    Var* rr = visit_call_function(parser, ast, scope, new_var(0, VAR_NONE));
    while(rr->type == VAR_FUNC)
    {
      cnt += 1;
      ast = ast->mid;
      rr = visit_call_function(parser, ast, scope, rr);
    }
    while(cnt--)
    {
      scope_pop(scope);
      // puts("DEL");
    }
    return rr;
  case TOKEN_FUNC:
    return visit_define_function(parser, ast, scope);
  }

  return new_var((void*)(intptr_t)curr_val, VAR_INT);
}

// assigns Var
void visit_assign_var(Parser* parser, AST* ast, Scope* scope)
{ 
  char* var_name = strdup(ast->left->token->value);
  Var* cond = visit_condition(parser, ast->right, scope);

  int scopei = 0;
  for(int i = scope->size; i >= 1; i--)
  {
    scopei = i;
    Var* var = variables_get(scope->variables[i], var_name);
    if(var->type != VAR_NONE)
    {
      goto assignvar;
    }
  }

  assignvar:
  switch (cond->type)
  {
    case VAR_FUNC:
      variables_add(scope->variables[scopei], var_name, cond->value, VAR_FUNC);
      break;
    case VAR_CHAR:
      variables_add(scope->variables[scopei], var_name, cond->value, VAR_CHAR);
      break;
    case VAR_INT:
      variables_add(scope->variables[scopei], var_name, cond->value, VAR_INT);
      break;
    case VAR_STR:
      variables_add(scope->variables[scopei], var_name, cond->value, VAR_STR);
      break;
  }
  
  free(var_name);
}

// gets Var
Var* visit_get_var(Parser* parser, char* name, Scope* scope)
{
  for(int i = scope->size; i >= 1; i--)
  {
    Var* var = variables_get(scope->variables[i], name);
    if(var->type != VAR_NONE)
    {
      return var;
    }
  }
  
  return new_var(0, VAR_INT);
}

// define new function
Var* visit_define_function(Parser* parser, AST* ast, Scope* scope)
{
  Function* funcv = new_function();
  funcv->contents = ast;
  return new_var(funcv, VAR_FUNC);
}

// call function
Var* visit_call_function(Parser* parser, AST* ast, Scope* scope, Var* funcv)
{
  if(funcv->value != NULL)
  {
    goto withvar;
  }
  if(strcmp(ast->token->value, "print") == 0)
  {
    return new_var((void*)(intptr_t)visit_print_function(parser, ast->right, scope), VAR_INT);
  }
  else if(strcmp(ast->token->value, "char") == 0)
  {
    return visit_char_function(parser, ast->right, scope);
  }
  else if(strcmp(ast->token->value, "int") == 0)
  {
    return visit_int_function(parser, ast->right, scope);
  }
  else if(strcmp(ast->token->value, "type") == 0)
  {
    return visit_type_function(parser, ast->right, scope);
  }

  char* func_name = strdup(ast->token->value);
  funcv = visit_get_var(parser, func_name, scope);
  free(func_name);
  withvar:

  Return* res = new_return(0, new_var(0, VAR_INT));
  Variables* variables = new_variables();

  // assign local variables passed in arguments
  AST* v = ast->right;
  AST* mid2v = ((Function*)funcv->value)->contents;
  while(mid2v->mid2 != NULL)
  {
    mid2v = mid2v->mid2;
    Var* var = visit_condition(parser, v->left, scope);
    variables_add(variables, mid2v->token->value, var->value, var->type);
    v = v->right;
  }

  scope_insert(scope, variables);

  AST* midv = ((Function*)funcv->value)->contents;
  while(midv->mid != NULL)
  { 
    midv = midv->mid;

    res = visit(parser, midv->right, scope);
    if(res->isreturned == 1)
    {
      goto ret;
    }
  }

  ret:
  Var* result = res->var;
  if(res->var->type != VAR_FUNC)
  {
    scope_pop(scope);
  }

  return result;
}

// builtin print function (temporary), passes unlimited arguments
int visit_print_function(Parser* parser, AST* ast, Scope* scope)
{
  Var* cond = visit_condition(parser, ast->left, scope);
  
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
    return visit_print_function(parser, ast->right, scope);
  }

  printf("\n");
  return 0;
}

// builtin int function
// * int function forces condition to be int
Var* visit_int_function(Parser* parser, AST* ast, Scope* scope)
{
  Var* cond = visit_condition(parser, ast->left, scope);
  cond->type = VAR_INT;
  return cond;
}

// * char function forces condition to be char
Var* visit_char_function(Parser* parser, AST* ast, Scope* scope)
{
  Var* cond = visit_condition(parser, ast->left, scope);
  cond->type = VAR_CHAR;
  return cond;
}

Var* visit_type_function(Parser* parser, AST* ast, Scope* scope)
{
  Var* cond = visit_condition(parser, ast->left, scope);
  cond->value = new_str(var_map2[cond->type]);
  cond->type = VAR_STR;
  return cond;
}

// just if
Return* visit_if(Parser* parser, AST* ast, Scope* scope)
{
  if((intptr_t)visit_condition(parser, ast->left, scope)->value == 0)
  {
    return new_return(0, new_var(0, VAR_INT));
  }
  
  AST* _ast = ast;
  while(_ast->mid != NULL)
  {
    _ast = _ast->mid;
    Return* _ret = visit(parser, _ast->right, scope);
    if(_ret->isreturned == 1)
    {
      return _ret;
    }
    free(_ret);
  }
  
  return new_return(0, new_var(0, VAR_INT));
}

// while
Return* visit_while(Parser* parser, AST* ast, Scope* scope)
{
  while((intptr_t)visit_condition(parser, ast->left, scope)->value != 0)
  {
    AST* _ast = ast;
    while(_ast->mid != NULL)
    {
      _ast = _ast->mid;
      Return* _ret = visit(parser, _ast->right, scope);
      if(_ret->isreturned == 1)
      {
        return _ret;
      }
      free(_ret);
    }
  }

  return new_return(0, new_var(0, VAR_INT));
}