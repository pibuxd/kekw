#ifndef SCOPEE_H
#define SCOPEE_H
#include "variables.h"

// * scope is static stack
typedef struct SCOPE_STRUCT
{
  int size;
  Variables** variables;
} Scope;

Scope* new_scope();

void scope_insert(Scope* scope, Variables* variables);

void scope_pop(Scope* scope);

Variables* scope_top(Scope* scope);

#endif // !SCOPEE_H