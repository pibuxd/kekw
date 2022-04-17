#include "include/return.h"
#include <stdlib.h>

Return* new_return(int isreturned, Var* var)
{
  Return* res = calloc(1, sizeof(Return));
  res->isreturned = isreturned;
  res->var = var;
  return res;
}