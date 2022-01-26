#include "include/visitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(char *filename){
  FILE *fp = fopen(filename, "rb"); // Open in binary mode
  int seek = fseek(fp, 0, SEEK_END);
  if(seek != 0)
  {
    printf("file \"%s\" doesn't exists\n", filename);
    fclose(fp);
    exit(-1);
  }

  size_t filesize = (size_t)ftell(fp);
  char* filedata = calloc(filesize + 1, 1); // Add 1 for the terminating "nul" character

  rewind(fp);
  fread(filedata, 1, filesize, fp); // Read whole file
  fclose(fp);

  return filedata;
}

int main(int argc, char* argv[])
{
  if(argc == 1)
  {
    puts("no such file given");
    exit(-1);
  }
  if(argc > 2)
  {
    puts("too much arguments given");
  }

  char *filename = calloc(strlen(argv[1]), sizeof(char));
  strcpy(filename, argv[1]);
  char* filedata = read_file(filename);

  Lexer* lexer = NULL;
  Token* token = NULL;
  Parser* parser = NULL;

  lexer = new_lexer(filedata);  
  token = lexer_get_next_token(lexer);
  parser = new_parser(lexer, token);

  parser_compound(parser);
  visit_compound(parser);

  free(filename);
  free(filedata);
  free(lexer);
  free(token);
  free(parser);
  
  return 0;
}