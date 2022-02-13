#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct LEXER_STRUCT
{
  char* content;
  char current_c; // current content[i] = current_c
  unsigned int i;
  unsigned int current_line;
} Lexer;

Lexer* new_lexer(char* content);

void free_lexer(Lexer* lexer);

void lexer_advance(Lexer* lexer);

char lexer_peek(Lexer* lexer);

void lexer_skip_whitespace(Lexer* lexer);

Token* lexer_get_next_token(Lexer* lexer);

Token* lexer_peek_next_token(Lexer* lexer);

int lexer_get_line(Lexer* lexer);

char* lexer_collect_string(Lexer* lexer);

char* lexer_collect_id(Lexer* lexer);

char* lexer_collect_int(Lexer* lexer);

Token* lexer_advance_with_token(Lexer* lexer, Token* token);

char* lexer_get_current_char_as_string(Lexer* lexer);

void lexer_print_error(Lexer* lexer);

#endif //! LEXER_H