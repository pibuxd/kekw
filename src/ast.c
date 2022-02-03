#include "include/ast.h"
#include <stdlib.h>

AST* new_ast(AST* left, AST* right, Token* token){
  AST* ast = calloc(1, sizeof(AST));
  ast->left = left;
  ast->right = right;
  ast->token = token;
  
  return ast;
}

void free_ast(AST* ast)
{
  if(ast->left != NULL)
  {
    free_ast(ast->left);
  }
  if(ast->right != NULL)
  {
    free_ast(ast->right);
  }
  if(ast->token != NULL)
  {
    free_token(ast->token);
  }

  free(ast);
}