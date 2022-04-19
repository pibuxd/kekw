#ifndef STR_H
#define STR_H

typedef struct STR_STRUCT
{
  char* value;
  int lenght;
} Str;

Str* new_str(char* value);

void free_str(Str* str);

#endif // !STR_H