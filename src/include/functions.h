#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "ast.h"
#include "variables.h"

typedef struct FUNCTIONS_STRUCT
{
  AST*** functions;
  unsigned int functions_size;
  unsigned int* func_size;
  int** functions_args_order;
  int* functions_args_order_size;
  int* functions_it;
} Functions;

Functions* new_functions();

void free_functions(Functions* functions);

void functions_add_new(Functions* functions, unsigned int func_idx, int func_name_hash);

#endif // !FUNCTIONS_H