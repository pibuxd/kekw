#include "include/ast.h"
#include <stdlib.h>

AST* new_ast(AST* left, AST* right, Token* token){
  AST* ast = malloc(1*sizeof(AST));
  ast->left = left;
  ast->right = right;
  ast->token = token;
  ast->mid = NULL;
  ast->mid2 = NULL;
  
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