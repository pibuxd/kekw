#include "include/lexer.h"
//#include "include/token.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
  Lexer* lexer = new_lexer(
    "var name = \"Pi Bu\"\n"
    "print(name, \"hello world\")\n"
  );

  Token* token = NULL;
  for(token = lexer_get_next_token(lexer); token->type != TOKEN_EOF; token = lexer_get_next_token(lexer))
  {
    printf("TOKEN(%d, %s)\n", token->type, token->value);
  }
}