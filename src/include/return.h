#ifndef RETURN_H
#define RETURN_H
#include "var.h"

typedef struct RETURN_STRUCT {
  int isreturned;
  Var* var;
} Return;

Return* new_return(int isreturned, Var* var);

#endif // !RETURN_H