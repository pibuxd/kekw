#ifndef RETURN_H
#define RETURN_H
#include "var.h"

typedef struct RETURN_STRUCT {
  int isreturned;
  Var* var;
} Return;

Return* new_return(int isreturned, Var* var);

void free_return(Return* ret);

#endif // !RETURN_H