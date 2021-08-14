#include "include/ast.h"
#include <stdlib.h>

AST* new_ast(AST* left, AST* right, Token* token){
  AST* ast = calloc(1, sizeof(AST));
  ast->left = left;
  ast->right = right;
  ast->token = token;
  
  return ast;
}