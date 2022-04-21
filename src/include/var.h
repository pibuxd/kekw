#ifndef VAR_H
#define VAR_H

typedef struct VAR_STRUCT {
  void* value;
  enum
  {
    VAR_INT = 0,
    VAR_CHAR = 1,
    VAR_STR = 2,
    VAR_FUNC = 3,
  } type;
} Var;

static const char* const var_map[] = {
  [VAR_INT] = "VAR_INT",
  [VAR_CHAR] = "VAR_CHAR",
  [VAR_STR] = "VAR_STRING",
  [VAR_FUNC] = "VAR_FUNC",
};

Var* new_var(void* val, int type);

void free_var(Var* var);

#endif // !VAR_H