#include "include/variables.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// create new Local Variables
Variables* new_variables()
{
  Variables* variables = malloc(1*sizeof(Variables));
  
  variables->size = 50;
  variables->list = calloc(variables->size, sizeof(Node*));

  return variables;
}

void free_node(Node* x)
{
  if(x->next != NULL)
  {
    free_node(x->next);
  }

  free(x->next);
}

void free_variables(Variables* variables)
{
  for(int i = 0; i < variables->size; i++)
  {
    Node* node = variables->list[i];

    if(node != NULL)
    {
      free_node(node->next);
    }
    free(variables->list[i]);
  }
  free(variables);
}

// returns hash as int with modulo as size of hash table
int variables_hash(char* str, int size)
{
  const int mod = size;
  int res = 0, p = 1;

  for(unsigned int i = 0, strl = strlen(str); i < strl; i++)
  {
    res = (res + ((str[i] - 'a' + 1) * p) % mod ) % mod;
    p = (p * 31) % mod;
  }

  return res;
}

// insert new variable to hash table
void variables_add(Variables* variables, char* var_name, int val)
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
      node->value = val;
      return;
    }
  }

  node->next = malloc(1*sizeof(Node));
  node->next->key = strdup(var_name);
  node->next->value = val;
  node->next->next = NULL;
}

// returns {does_exists, value} from hash table
int* variables_get(Variables* variables, char* var_name)
{ 
  int* res = calloc(2, sizeof(int));
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
      res[0] = 1, res[1] = node->value;
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