#ifndef STRING_H
#define STRING_H

typedef struct STRING_STRUCT
{
  char* value;
} String;

String* new_string(char* value);

void free_string(String* string);

#endif // !STRING_H