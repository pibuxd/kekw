#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

Lexer* new_lexer(char* content)
{
  Lexer* lexer = calloc(1, sizeof(Lexer));
  
  lexer->content = content;
  lexer->i = 0;
  lexer->current_c = content[lexer->i];
  lexer->current_line = 0;

  return lexer;
}

void lexer_advance(Lexer* lexer)
{
  if(lexer->current_c != '\0' && lexer->i < strlen(lexer->content)){
    lexer->i += 1;
    lexer->current_c = lexer->content[lexer->i];
  }
}

char lexer_peek(Lexer* lexer)
{
  if(lexer->current_c != '\0' && lexer->i < strlen(lexer->content))
    return lexer->content[lexer->i];
  else 
    return ' ';
}

void lexer_skip_whitespace(Lexer* lexer)
{
  while(lexer->current_c == ' ' || lexer->current_c == 10) // 10 is Line Feed in ASCII
  {
    if(lexer->current_c == 10)
    {
      lexer->current_line += 1;
    }
    lexer_advance(lexer);
  }
}

Token* lexer_get_next_token(Lexer* lexer)
{
  while(lexer->current_c != '\0' && lexer->i < strlen(lexer->content))
  {
    if(lexer->current_c == ' ' || lexer->current_c == 10)
      lexer_skip_whitespace(lexer);
    
    if(isdigit(lexer->current_c))
      return new_token(TOKEN_INT, lexer_collect_int(lexer));
    else if(isalnum(lexer->current_c))
      return new_token(TOKEN_ID, lexer_collect_id(lexer));
    else if(lexer->current_c == '"')
      return new_token(TOKEN_STRING, lexer_collect_string(lexer));

    switch(lexer->current_c)
    {
      case '+': return lexer_advance_with_token(lexer, new_token(TOKEN_PLUS, lexer_get_current_char_as_string(lexer)));
      case '-': return lexer_advance_with_token(lexer, new_token(TOKEN_MINUS, lexer_get_current_char_as_string(lexer)));
      case '*': return lexer_advance_with_token(lexer, new_token(TOKEN_MUL, lexer_get_current_char_as_string(lexer)));
      case '/': return lexer_advance_with_token(lexer, new_token(TOKEN_DIV, lexer_get_current_char_as_string(lexer)));
      case '=': return lexer_advance_with_token(lexer, new_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
      case '>': return lexer_advance_with_token(lexer, new_token(TOKEN_GREATER, lexer_get_current_char_as_string(lexer)));
      case '<': return lexer_advance_with_token(lexer, new_token(TOKEN_LESS, lexer_get_current_char_as_string(lexer)));
      case ';': return lexer_advance_with_token(lexer, new_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer)));
      case '(': return lexer_advance_with_token(lexer, new_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
      case ')': return lexer_advance_with_token(lexer, new_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
      case '{': return lexer_advance_with_token(lexer, new_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer)));
      case '}': return lexer_advance_with_token(lexer, new_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer)));
      case ',': return lexer_advance_with_token(lexer, new_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer)));
    }
  }

  return new_token(TOKEN_EOF, "EOF");
}

int lexer_get_line(Lexer* lexer)
{
  return lexer->current_line;
}

char* lexer_collect_string(Lexer* lexer)
{
  lexer_advance(lexer);

  char* str = calloc(1, sizeof(char));
  str[0] = '\0';

  while(lexer->current_c != '"')
  {
    char* c = lexer_get_current_char_as_string(lexer);
    str = realloc(str, (strlen(str)+strlen(c)+1) * sizeof(char));
    strcat(str, c);
    free(c);

    lexer_advance(lexer);
  }
  lexer_advance(lexer);

  return str;
}

char* lexer_collect_id(Lexer* lexer)
{
  char* str = calloc(1, sizeof(char));
  str[0] = '\0';

  while(isalnum(lexer->current_c))
  {
    char* c = lexer_get_current_char_as_string(lexer);
    str = realloc(str, (strlen(str)+strlen(c)+1) * sizeof(char));
    strcat(str, c);
    free(c);

    lexer_advance(lexer);
  }

  return str;
}

char* lexer_collect_int(Lexer* lexer)
{
  char* str = calloc(1, sizeof(char));
  str[0] = '\0';

  while(isdigit(lexer->current_c))
  {
    char* c = lexer_get_current_char_as_string(lexer);
    str = realloc(str, (strlen(str)+strlen(c)+1) * sizeof(char));
    strcat(str, c);
    free(c);

    lexer_advance(lexer);
  }

  return str;
}

Token* lexer_advance_with_token(Lexer* lexer, Token* token){
  lexer_advance(lexer);

  return token;
}

char* lexer_get_current_char_as_string(Lexer* lexer){
  char* str = calloc(2, sizeof(char));
  str[0] = lexer->current_c;
  str[1] = '\0';

  return str;
}

void lexer_print_error(Lexer* lexer)
{
  printf("error on line %d: ", lexer_get_line(lexer));
}