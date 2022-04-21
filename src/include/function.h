#ifndef FUNCTION_H
#define FUNCTION_H
#include "ast.h"
#include "variables.h"

typedef struct FUNCTIONS_STRUCT
{
  AST* contents;
} Function;

Function* new_function();

void free_function(Function* function);

#endif // !FUNCTION_H