#include "include/str.h"
#include <stdlib.h>
#include <string.h>

Str* new_str(char* value)
{
  Str* str = calloc(1, sizeof(Str));

  str->value = strdup(value);
  str->lenght = strlen(value);

  return str;
}

void free_str(Str* str)
{
  free(str->value);
  free(str);
}