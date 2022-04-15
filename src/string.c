#include "include/string.h"
#include <stdlib.h>
#include <string.h>

String* new_string(char* value)
{
  String* string = calloc(1, sizeof(string));

  string->value = strdup(value);

  return string;
}

void free_string(String* string)
{
  free(string->value);
  free(string);
}