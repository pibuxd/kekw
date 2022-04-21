#include "include/var.h"
#include "include/function.h"
#include <stdlib.h>
#include <string.h>

Var* new_var(void* val, char* type)
{
  Var* var = calloc(1, sizeof(Var));
  var->value = val;
  var->type = strdup(type);
  return var;
}

void free_var(Var* var)
{
  if(strcmp(var->type, "int"))
  {
    free(var->value);
  }
  else if(strcmp(var->type, "str"))
  {
    free_str(var->value);
  }
  else if(strcmp(var->type, "char"))
  {
    free(var->value);
  }
  else if(strcmp(var->type, "func"))
  {
    free_function(var->value);
  }

  free(var->type);
  free(var);
}