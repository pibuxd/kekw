#ifndef VARIABLES_H
#define VARIABLES_H
#include "return.h"
#include "var.h"

typedef struct NODE_STRUCT
{
  char* key;
  Var* var;
  struct NODE_STRUCT* next;
} Node;

typedef struct VARIABLES_STRUCT
{
  int size;
  Node** list;
} Variables;

Variables* new_variables();

void free_node(Node* x);

void free_variables(Variables* variables);

int variables_hash(char* str, int size);

void variables_add(Variables* variables, char* name, void* val, char* type);

Var* variables_get(Variables* variables, char* var_name);

void variables_delete(Variables* variables, int name_hash);

void variables_delete_all(Variables* variables);

#endif // !VARIABLES_H