#ifndef AST_H
#define AST_H
#include "token.h"

typedef struct AST_STRUCT {
  struct AST_STRUCT* left;
  struct AST_STRUCT* right;
  Token* token;
} AST;

AST* new_ast(AST* left, AST* right, Token* token);

#endif // !AST_H