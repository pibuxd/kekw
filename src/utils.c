#include "include/utils.h"
#include <string.h>
#include <stdlib.h>

int utils_hash_string(char* str)
{
  const int mod = 999983;
  int res = 0, p = 1;

  for(unsigned int i = 0, strl = strlen(str); i < strl; i++)
  {
    res = (res + ((str[i] - 'a' + 1) * p) % mod ) % mod;
    p = (p * 31) % mod;
  }

  return res;
}

int utils_stoi(char* str)
{
  return atoi(str);
}