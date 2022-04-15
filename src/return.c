#include "include/return.h"
#include <stdlib.h>

Return* new_return()
{
  Return* res = calloc(1, sizeof(Return));
  return res;
}