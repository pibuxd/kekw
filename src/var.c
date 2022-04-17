#include "include/var.h"
#include <stdlib.h>
#include <string.h>

Var* new_var(void* val, char* type)
{
  Var* var = calloc(1, sizeof(Var));
  var->value = val;
  var->type = strdup(type);
  return var;
}