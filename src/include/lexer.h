#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct LEXER_STRUCT
{
  char* content;
  char current_c; // current content[i] = current_c
  unsigned int i;
} Lexer;

Lexer* new_lexer(char* content);

void lexer_advance(Lexer* lexer);

void lexer_skip_whitespace(Lexer* lexer);

Token* lexer_get_next_token(Lexer* lexer);

char* lexer_collect_string(Lexer* lexer);

char* lexer_collect_id(Lexer* lexer);

Token* lexer_advance_with_token(Lexer* lexer, Token* token);

char* lexer_get_current_char_as_string(Lexer* lexer);

#endif //! LEXER_H