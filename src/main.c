#include "include/visitor.h"
#include <stdio.h>
#include <stdlib.h>

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
    "print((0 - 1) * (9 * 5 + 10 - 3)*5 + 1 - 2 - 3 * (0 - 1), x, y, \"XD\");\n"
  );
  token = lexer_get_next_token(lexer);
  parser = new_parser(lexer, token);

  parser_compound(parser);
  puts("\nvisiting...\n");
  visit_compound(parser);
}