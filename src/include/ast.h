#ifndef AST_H
#define AST_H
#include "token.h"

typedef struct AST_STRUCT {
  struct AST_STRUCT* left;
  struct AST_STRUCT* right;
  struct AST_STRUCT* mid;
  Token* token;
} AST;

AST* new_ast(AST* left, AST* right, Token* token);

void free_ast(AST* ast);

#endif // !AST_H