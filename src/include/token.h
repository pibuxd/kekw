#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
  enum
  {
    TOKEN_ID, // 0
    TOKEN_PLUS, // 1
    TOKEN_MINUS, // 2
    TOKEN_MUL, // 3
    TOKEN_DIV, // 4
    TOKEN_EQUALS, // 5 
    TOKEN_STRING, // 6
    TOKEN_INT, // 7
    TOKEN_SEMI, // 8
    TOKEN_LPAREN, // 9
    TOKEN_RPAREN, // 10
    TOKEN_LBRACE, // 11
    TOKEN_RBRACE, // 12
    TOKEN_COMMA, // 13
    TOKEN_EOF // 14
  } type;

  char* value;
} Token;

Token* new_token(int type, char* value);

#endif // !TOKEN_H