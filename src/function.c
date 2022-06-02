#include "include/function.h"
#include <stdlib.h>

Function* new_function()
{
  Function* function = calloc(1, sizeof(Function));
  function->contents = new_ast(NULL, NULL, NULL);

  return function;
}

void free_function(Function* function)
{
  free_ast(function->contents);
  free(function);
}