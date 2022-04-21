#include "include/var.h"
#include "include/function.h"
#include <stdlib.h>
#include <string.h>

Var* new_var(void* val, int type)
{
  Var* var = calloc(1, sizeof(Var));
  var->value = val;
  var->type = type;
  return var;
}

void free_var(Var* var)
{
  switch (var->type)
  {
    case VAR_INT:
      free(var->value);
      break;
    case VAR_STR:
      free_str(var->value);
      break;
    case VAR_CHAR:
      free(var->value);
      break;
    case VAR_FUNC:
      free_function(var->value);
  }

  free(var);
}