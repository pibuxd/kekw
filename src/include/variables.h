#ifndef VARIABLES_H
#define VARIABLES_H

typedef struct NODE_STRUCT
{
  char* key;
  int value;
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

void variables_add(Variables* variables, char* name, int val);

int* variables_get(Variables* variables, char* var_name);

void variables_delete(Variables* variables, int name_hash);

void variables_delete_all(Variables* variables);

#endif // !VARIABLES_H