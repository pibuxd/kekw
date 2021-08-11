#include "include/parser.h"
#include "include/lexer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  Lexer* lexer = NULL;
  Token* token = NULL;
  Parser* parser = NULL;

  while(1)
  {
    char* input = calloc(1000, sizeof(char));
    fgets(input, 1000, stdin);
    lexer = new_lexer(input);
    token = lexer_get_next_token(lexer);
    parser = new_parser(lexer, token);
    int res = parser_expr(parser);

    printf("res = %d\n", res);
  }


  // for(token = lexer_get_next_token(lexer); token->type != TOKEN_EOF; token = lexer_get_next_token(lexer)){
  //   printf("TOKEN(%d, %s)\n", token->type, token->value);
  // }

}