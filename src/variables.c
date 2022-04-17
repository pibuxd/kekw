#include "include/variables.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// create new Local Variables
Variables* new_variables()
{
  Variables* variables = calloc(1, sizeof(Variables));
  
  variables->size = 50;
  variables->list = calloc(variables->size, sizeof(Node*));

  return variables;
}

void free_node(Node* x)
{
  free(x->key);
  // free(x->var->value);
  // free(x->var->type);
  free(x->var);

  if(x->next != NULL)
  {
    free_node(x->next);
  }

  free(x);
}

void free_variables(Variables* variables)
{
  for(int i = 0; i < variables->size; i++)
  {
    if(variables->list[i] == NULL)
    {
      free(variables->list[i]);
      continue;
    }    

    Node* node = variables->list[i];
    if(node->next != NULL)
    {
      free_node(node->next);
    }

    free(node->key);
    free(variables->list[i]);
  }

  free(variables->list);
  free(variables);
}

// returns hash as int with modulo as size of hash table
int variables_hash(char* str, int size)
{
  const int MOD = size, P = 9973;
  int res = 0, p = 1;

  for(unsigned int i = 0, strl = strlen(str); i < strl; i++)
  {
    res = (res + ((str[i] - 'a' + 1) * p) % MOD ) % MOD;
    p = (p * P) % MOD;
  }

  return res;
}

// insert new variable to hash table
void variables_add(Variables* variables, char* var_name, void* val, char* type)
{
  // variables->size += 1;
  // variables->list = realloc(variables->list, variables->size*sizeof(Node*));
  int var_name_hash = variables_hash(var_name, variables->size);

  if(variables->list[var_name_hash] == NULL)
  {
    variables->list[var_name_hash] = calloc(1, sizeof(Node));
  }
  
  Node* node = variables->list[var_name_hash];

  while(node->next != NULL)
  {
    node = node->next;
    if(strcmp(node->key, var_name) == 0)
    {
      node->var->value = val;
      node->var->type = strdup(type);
      return;
    }
  }

  node->next = calloc(1, sizeof(Node));
  node->next->key = strdup(var_name);
  node->next->var = new_var(val, type);
  node->next->next = NULL;
}

// returns {value, type} from hash table
Var* variables_get(Variables* variables, char* var_name)
{ 
  Var* res = new_var(0, "int");
  int var_name_hash = variables_hash(var_name, variables->size);
  
  Node* node = variables->list[var_name_hash];

  if(node == NULL)
  {
    return res;
  }

  while(node->next != NULL)
  {
    node = node->next;

    if(strcmp(node->key, var_name) == 0)
    {
      res->value = node->var->value;
      res->type = node->var->type;
      return res;
    }
  }

  return res;
}

void variables_delete(Variables* variables, int name_hash)
{
  // variables->values[name_hash] = 0;
  // variables->exists[name_hash] = 0;
}

void variables_delete_all(Variables* variables)
{
  free_variables(variables);
  variables = new_variables();
}