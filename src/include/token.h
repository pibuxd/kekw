#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
  enum
  {
    TOKEN_ID,
    TOKEN_EQUALS,
    TOKEN_STRING,
    TOKEN_SEMI,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_COMMA,
    TOKEN_EOF
  } type;

  char* value;
} Token;

Token* new_token(int type, char* value);

#endif // !TOKEN_H