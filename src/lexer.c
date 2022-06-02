#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

Lexer* new_lexer(char* content)
{
  Lexer* lexer = calloc(1, sizeof(Lexer));

  lexer->content = strdup(content);
  lexer->i = 0;
  lexer->current_c = content[lexer->i];
  lexer->current_line = 1;
  lexer->content_len = strlen(lexer->content);

  return lexer;
}

void free_lexer(Lexer* lexer)
{
  free(lexer->content);
  free(lexer);
}

// just advance Lexer
void lexer_advance(Lexer* lexer)
{
  if(lexer->current_c != '\0' && lexer->i < lexer->content_len){
    lexer->i += 1;
    lexer->current_c = lexer->content[lexer->i];
  }
}

// peek char without advancing Lexer
char lexer_peek(Lexer* lexer)
{ 
  if(lexer->i == lexer->content_len)
  {
    return ' ';
  }
  return lexer->content[lexer->i+1];
}

// skip whitespaces with advancing Lexer
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

void lexer_skip_comment(Lexer* lexer)
{
  while(lexer->current_c != 10) // 10 is Line Feed in ASCII
  {
    lexer_advance(lexer);
  }

  lexer->current_line += 1;
}

void lexer_skip_multicomment(Lexer* lexer)
{
  lexer_advance(lexer);
  lexer_advance(lexer);
  while(lexer->current_c != '#') // 10 is Line Feed in ASCII
  {
    if(lexer->current_c == 10)
    {
      lexer->current_line += 1;
    }
    lexer_advance(lexer);
  }
  lexer_advance(lexer);
  lexer_advance(lexer);
}

// returns next Token and advance Lexer
Token* lexer_get_next_token(Lexer* lexer)
{
  while(lexer->current_c != '\0' && lexer->i < lexer->content_len)
  {
    if(lexer->current_c == ' ' || lexer->current_c == 10)
      lexer_skip_whitespace(lexer);
    if(lexer->current_c == '#')
    {
      if(lexer_peek(lexer) == '#')
        lexer_skip_multicomment(lexer);
      else
        lexer_skip_comment(lexer);
      continue;
    }

    if(isdigit(lexer->current_c))
    {
      char* str = lexer_collect_int(lexer);
      Token* tok = new_token(TOKEN_INT, str);
      free(str);
      return tok;
    }
    else if(isalnum(lexer->current_c))
    {
      char* str = lexer_collect_id(lexer);
      Token* tok = new_token(TOKEN_ID, str);
      free(str);
      return tok;
    }
    else if(lexer->current_c == '"')
    {
      char* str = lexer_collect_string(lexer);
      Token* tok = new_token(TOKEN_STRING, str);
      free(str);
      return tok;
    }
    else if(lexer->current_c == '\'')
    {
      char* str = lexer_collect_char(lexer);
      Token* tok = new_token(TOKEN_CHAR, str);
      free(str);
      return tok;
    }
    
    char peek = lexer_peek(lexer);

    switch(lexer->current_c)
    {
      case '+': return lexer_advance_with_token(lexer, new_token(TOKEN_PLUS, lexer_get_current_char_as_string(lexer)));
      case '-': return lexer_advance_with_token(lexer, new_token(TOKEN_MINUS, lexer_get_current_char_as_string(lexer)));
      case '*': return lexer_advance_with_token(lexer, new_token(TOKEN_MUL, lexer_get_current_char_as_string(lexer)));
      case '/': return lexer_advance_with_token(lexer, new_token(TOKEN_DIV, lexer_get_current_char_as_string(lexer)));
      case '=': return lexer_advance_with_token(lexer, new_token(peek == '=' ? TOKEN_EQ : TOKEN_EQUALS, peek == '=' ? "==" : "="));
      case '>': return lexer_advance_with_token(lexer, new_token(peek == '=' ? TOKEN_GREATEREQ : TOKEN_GREATER, peek == '=' ? ">=" : ">"));
      case '<': return lexer_advance_with_token(lexer, new_token(peek == '=' ? TOKEN_LESSEQ : TOKEN_LESS, peek == '=' ? "<=" : "<"));
      case ';': return lexer_advance_with_token(lexer, new_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer)));
      case '(': return lexer_advance_with_token(lexer, new_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
      case ')': return lexer_advance_with_token(lexer, new_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
      case '{': return lexer_advance_with_token(lexer, new_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer)));
      case '}': return lexer_advance_with_token(lexer, new_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer)));
      case '[': return lexer_advance_with_token(lexer, new_token(TOKEN_LSQUARE, lexer_get_current_char_as_string(lexer)));
      case ']': return lexer_advance_with_token(lexer, new_token(TOKEN_RSQUARE, lexer_get_current_char_as_string(lexer)));
      case ',': return lexer_advance_with_token(lexer, new_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer)));
    }
  }

  return new_token(TOKEN_EOF, "EOF");
}

// returns next Token but without advancing Lexer
Token* lexer_peek_next_token(Lexer* lexer)
{
  char __current_c = lexer->current_c;
  unsigned int __i = lexer->i;
  unsigned int __current_line = lexer->current_line;

  Token* tok = lexer_get_next_token(lexer);
  lexer->current_c = __current_c;
  lexer->i = __i;
  lexer->current_line = __current_line;

  return tok;
}

int lexer_get_line(Lexer* lexer)
{
  return lexer->current_line;
}

char* lexer_collect_string(Lexer* lexer)
{
  lexer_advance(lexer);

  char* str = malloc(1*sizeof(char));
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

char* lexer_collect_char(Lexer* lexer)
{
  lexer_advance(lexer);

  char* str = malloc(1*sizeof(char));
  str[0] = '\0';

  char* c = lexer_get_current_char_as_string(lexer);
  str = realloc(str, (strlen(str)+strlen(c)+1) * sizeof(char));
  strcat(str, c);
  free(c);

  lexer_advance(lexer);
  lexer_advance(lexer);

  return str;
}

// returns string with collected id from Lexer
char* lexer_collect_id(Lexer* lexer)
{
  char* str = malloc(1*sizeof(char));
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

// returns string with collected int from Lexer
char* lexer_collect_int(Lexer* lexer)
{
  char* str = malloc(1*sizeof(char));
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

// returns Token after advancing Lexers
Token* lexer_advance_with_token(Lexer* lexer, Token* token){
  lexer_advance(lexer);

  if(8 <= token->type && token->type <= 10) // skip Token created from two separated eg. >= ('>' + '=')
    lexer_advance(lexer);
  return token;
}

// XD
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