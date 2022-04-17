#ifndef VAR_H
#define VAR_H

typedef struct VAR_STRUCT {
  void* value;
  char* type;
} Var;

Var* new_var(void* val, char* type);

#endif // !VAR_H