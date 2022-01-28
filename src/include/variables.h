#ifndef VARIABLES_H
#define VARIABLES_H

typedef struct VARIABLES_STRUCT
{
  int* values;
  int* exists;
  int* types;
} Variables;

Variables* new_variables();

void variables_add_new(Variables* variables, int name_hash, int val);

void variables_delete(Variables* variables, int name_hash);

void variables_delete_all(Variables* variables);

#endif // !VARIABLES_H