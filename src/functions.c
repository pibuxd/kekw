#include "include/functions.h"
#include <stdlib.h>

Functions* new_functions()
{
  Functions* functions = calloc(1, sizeof(Functions));

  functions->functions = calloc(1, sizeof(AST));
  functions->functions_size = 0;
  functions->func_size = calloc(1, sizeof(unsigned int));
  functions->functions_args_order = calloc(1, sizeof(int*));
  functions->functions_args_order_size = calloc(1, sizeof(int));
  functions->functions_it = calloc(1000000, sizeof(int));

  return functions;
}

void free_functions(Functions* functions)
{
  for(int i = 0; i <= functions->functions_size; i++)
  {
    for(int fi = 0; fi <= functions->func_size[i]; fi++)
    {
      free_ast(functions->functions[i][fi]);
    }
    free(functions->functions_args_order[i]);
  }

  free(functions->func_size);
  free(functions->functions_args_order_size);
  free(functions->functions_it);
  free(functions);
}

void functions_add_new(Functions* functions, unsigned int func_idx, int func_name_hash)
{
  functions->functions_it[func_name_hash] = func_idx;

  functions->func_size = realloc(functions->func_size, (func_idx+1)*sizeof(int));
  functions->func_size[func_idx] = 0;

  functions->functions_args_order = realloc(functions->functions_args_order, (func_idx+1)*sizeof(int*));
  functions->functions_args_order[func_idx] = calloc(1, sizeof(int));
  
  functions->functions_args_order_size = realloc(functions->functions_args_order_size, 3 * (func_idx+1)*sizeof(int)); // don't know why have to multiply sizze by 3
  functions->functions_args_order_size[func_idx] = 0;
  
  functions->functions = realloc(functions->functions, (func_idx+1)*sizeof(AST));
  functions->functions[func_idx] = calloc(1, sizeof(AST));
}