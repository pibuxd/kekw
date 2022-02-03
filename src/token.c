#include "include/token.h"
#include <stdlib.h>
#include <string.h>

Token* new_token(int type, char* value)
{
  Token* token = calloc(1, sizeof(Token));
  token->type = type;
  token->value = calloc(strlen(value)+1, sizeof(char));
  strcpy(token->value, value);

  return token;
}

void free_token(Token* token)
{
  free(token->value);
  free(token);
}