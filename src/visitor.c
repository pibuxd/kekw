#include "include/visitor.h"
#include "include/utils.h"
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
    val += visit_expr(parser, ast->left, val) - visit_expr(parser, ast->right, val);
  }
  else if(ast->token->type == TOKEN_MUL)
  {
    val += visit_expr(parser, ast->left, val) * visit_expr(parser, ast->right, val);
  }
  else if(ast->token->type == TOKEN_DIV)
  {
    val += visit_expr(parser, ast->left, val) / visit_expr(parser, ast->right, val);
  }
  else if(ast->token->type == TOKEN_INT)
  {
    return utils_stoi(ast->token->value);
  }

  return val;
}

void visit_assign_var(Parser* parser, AST* ast)
{
  char* var_name = ast->left->token->value;
  int var_name_hashed = utils_hash_string(var_name);
  parser->ids[var_name_hashed] = visit_expr(parser, ast->right, 0);
}