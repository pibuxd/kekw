#include "include/variables.h"
#include <stdlib.h>

// create new Local Variables
Variables* new_variables()
{
  Variables* variables = calloc(1, sizeof(Variables));
  
  variables->values = calloc(1000000, sizeof(int));
  variables->exists = calloc(1000000, sizeof(int));
  variables->types = calloc(1000000, sizeof(int));

  return variables;
}
