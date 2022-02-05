#include "include/variables.h"
#include <stdlib.h>

// create new Local Variables
Variables* new_variables()
{
  Variables* variables = malloc(1*sizeof(Variables));
  
  variables->values = calloc(1000000, sizeof(int));
  variables->exists = calloc(1000000, sizeof(int));
  variables->types = calloc(1000000, sizeof(int));

  return variables;
}

void free_variables(Variables* variables)
{
  free(variables->values);
  free(variables->exists);
  free(variables->types);
  free(variables);
}

void variables_add_new(Variables* variables, int name_hash, int val)
{
  variables->values[name_hash] = val;
  variables->exists[name_hash] = 1;
}

void variables_delete(Variables* variables, int name_hash)
{
  variables->values[name_hash] = 0;
  variables->exists[name_hash] = 0;
}

void variables_delete_all(Variables* variables)
{
  free_variables(variables);
  variables = new_variables();
}