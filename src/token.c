#include "include/token.h"
#include <stdlib.h>

Token* new_token(int type, char* value)
{
  Token* token = calloc(1, sizeof(Token));
  token->type = type;
  token->value = value;

  return token;
}