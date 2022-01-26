#ifndef VARIABLES_H
#define VARIABLES_H

typedef struct VARIABLES_STRUCT
{
  int* values;
  int* exists;
  int* types;
} Variables;

Variables* new_variables();

#endif // !VARIABLES_H