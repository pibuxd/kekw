#include "include/visitor.h"
#include <stdio.h>
#include <stdlib.h>

void ast_walking(AST* ast, int side)
{
  // if(ast->token->type == TOKEN_INT)
  // {
  //   printf("%d: int -> %s\n", side, ast->token->value);
  // }
  // else if(ast->token->type == TOKEN_ID)
  // {
  //   printf("%d: id -> %s\n", side, ast->token->value);
  // }
  printf("%d: node -> %s\n", side, ast->token->value);

  if(ast->right != NULL)
    ast_walking(ast->right, 1);

  if(ast->left != NULL)
    ast_walking(ast->left, 0);
}

int main(int argc, char* argv[])
{
  Lexer* lexer = NULL;
  Token* token = NULL;
  Parser* parser = NULL;

  // char* input = calloc(1000, sizeof(char));
  // fgets(input, 1000, stdin);
  lexer = new_lexer(
    "var x = (0 - 1) * (9 * 5 + 10 - 3)*5 + 1 - 2 - 3 * (0 - 1);\n"
    "var y = 10*5;\n"
    "x;\n"
    "y;\n"
  );
  token = lexer_get_next_token(lexer);
  parser = new_parser(lexer, token);

  parser_compound(parser);
  visit_compound(parser);

  // ast_walking(parser->ast[1], -1);
  // ast_walking(parser->ast[2], -1);

  // visit_assign_var(parser, parser->ast[1]);
  // visit_assign_var(parser, parser->ast[2]);
  printf("xd: %d", parser->ids['y'-'a'+1]);
  // ast_walking(parser->ast[2], -1);
}