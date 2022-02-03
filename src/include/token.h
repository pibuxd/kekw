#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
  enum
  {
    TOKEN_ID = 0, // 0
    TOKEN_PLUS, // 1
    TOKEN_MINUS, // 2
    TOKEN_MUL, // 3
    TOKEN_DIV, // 4
    TOKEN_EQUALS, // 5
    TOKEN_GREATER, // 6
    TOKEN_LESS, // 7
    TOKEN_STRING, // 8
    TOKEN_INT, // 9
    TOKEN_SEMI, // 10
    TOKEN_LPAREN, // 11
    TOKEN_RPAREN, // 12
    TOKEN_LBRACE, // 13
    TOKEN_RBRACE, // 14
    TOKEN_COMMA, // 15
    TOKEN_FUNC, // 16
    TOKEN_CALL, // 17
    TOKEN_IF, // 18
    TOKEN_RETURN, // 19
    TOKEN_EOF // 20
  } type;

  char* value;
} Token;

Token* new_token(int type, char* value);

void free_token(Token* token);

#endif // !TOKEN_H