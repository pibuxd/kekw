#include "include/return.h"
#include <stdlib.h>

Return* new_return(int isreturned, Var* var)
{
  Return* res = calloc(1, sizeof(Return));
  res->isreturned = isreturned;
  res->var = var;
  return res;
}

void free_return(Return* ret)
{
  free_var(ret->var);
  free(ret);
}