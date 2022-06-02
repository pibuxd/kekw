#include "include/scope.h"
#include <stdlib.h>
#include <stdio.h>

Scope* new_scope()
{
  Scope* scope = calloc(1, sizeof(Scope));
  scope->size = 0;
  scope->variables = calloc(51, sizeof(Variables*));

  return scope;
}

void scope_insert(Scope* scope, Variables* variables)
{
  scope->size += 1;
  scope->variables[scope->size] = variables;
}

void scope_pop(Scope* scope)
{
  scope->variables[scope->size] = new_variables();
  scope->size -= 1;
}

Variables* scope_top(Scope* scope)
{
  return scope->variables[scope->size];
}