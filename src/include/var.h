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
    VAR_NONE = 4,
  } type;
} Var;

static char* var_map[] = {
  [VAR_INT] = "VAR_INT",
  [VAR_CHAR] = "VAR_CHAR",
  [VAR_STR] = "VAR_STR",
  [VAR_FUNC] = "VAR_FUNC",
  [VAR_NONE] = "VAR_NONE",
};

static char* var_map2[] = {
  [VAR_INT] = "int",
  [VAR_CHAR] = "char",
  [VAR_STR] = "str",
  [VAR_FUNC] = "func",
  [VAR_NONE] = "none",
};

Var* new_var(void* val, int type);

void free_var(Var* var);

#endif // !VAR_H