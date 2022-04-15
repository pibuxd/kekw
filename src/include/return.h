#ifndef RETURN_H
#define RETURN_H

typedef struct RETURN_STRUCT {
  int isreturned;
  void* value;
} Return;

Return* new_return();

#endif // !RETURN_H