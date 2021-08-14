#include "include/visitor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void visit_compound(Parser* parser)
{
  for(int i = 1; i < parser->ast_size; i++)
  {
    visit(parser, parser->ast[i]);
  }
}

void visit(Parser* parser, AST* ast)
{
  if(ast->token->type == TOKEN_EQUALS)
  {
    visit_assign_var(parser, ast);
  }
}

int visit_expr(Parser* parser, AST* ast, int val)
{
  if(ast->token->type == TOKEN_PLUS)
  {
    val += visit_expr(parser, ast->right, val) + visit_expr(parser, ast->left, val);
  }
  else if(ast->token->type == TOKEN_MINUS)
  {
    val += -1*(visit_expr(parser, ast->right, val) - visit_expr(parser, ast->left, val));
  }
  else if(ast->token->type == TOKEN_MUL)
  {
    val += visit_expr(parser, ast->right, val) * visit_expr(parser, ast->left, val);
    //val *= visit_expr(parser, ast->left, val);
  }
  else if(ast->token->type == TOKEN_DIV)
  { //! bad rn
    if(ast->right != NULL)
      val /= visit_expr(parser, ast->right, val);
    if(ast->left != NULL)
      val /= visit_expr(parser, ast->left, val);
  }
  else if(ast->token->type == TOKEN_INT)
  {
    return visit_stoi(ast->token->value);
  }

  return val;
}

void visit_assign_var(Parser* parser, AST* ast)
{
  char* var_name = ast->left->token->value;
  int var_name_hashed = visit_hash_string(var_name);
  parser->ids[var_name_hashed] = visit_expr(parser, ast->right, 0);
  printf("\nTEST: %d", parser->ids[var_name_hashed]);
}

void visit_get_var(Parser* parser, AST* ast)
{
  
}

int visit_stoi(char* str){
  int res = 0;

  int temp = 1;
  for(int i = strlen(str)-1; i >= 0; i--)
  {
    int factor = (int)(str[i] - '0');
    res += factor * temp;
    temp *= 10;
  }

  return res;
}

int visit_hash_string(char* str)
{
  int ans = 0;
  int p = 1;
  for(unsigned int i = 0; i < strlen(str); i++)
  {
    ans += (str[i] - 'a' + 1) * p;
    p *= 31;
  }

  return ans;
}

void visit_noop(Parser* parser, AST* ast)
{

}