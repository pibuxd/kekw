#include "include/parser.h"
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

  while(1)
  {
    // char* input = calloc(1000, sizeof(char));
    // fgets(input, 1000, stdin);
    lexer = new_lexer("var x = 6 / 2 + 3 * (5 + 4) - 1;\n");
    token = lexer_get_next_token(lexer);
    parser = new_parser(lexer, token);

    parser_compound(parser);
    ast_walking(parser->ast[1], -1);
    break;
  }


  // for(token = lexer_get_next_token(lexer); token->type != TOKEN_EOF; token = lexer_get_next_token(lexer)){
  //   printf("TOKEN(%d, %s)\n", token->type, token->value);
  // }

}