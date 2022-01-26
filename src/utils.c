#include "include/utils.h"
#include <string.h>

int utils_hash_string(char* str)
{
  const int mod = 999983;
  int res = 0, p = 1;

  for(unsigned int i = 0; i < strlen(str); i++)
  {
    res = (res + ((str[i] - 'a' + 1) * p) % mod ) % mod;
    p = (p * 31) % mod;
  }

  return res;
}

int utils_stoi(char* str)
{
  int res = 0;

  for(int i = strlen(str)-1, temp = 1; i >= 0; i--)
  {
    int factor = (int)(str[i] - '0');
    res += factor * temp;
    temp *= 10;
  }

  return res;
}