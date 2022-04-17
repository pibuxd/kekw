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
    TOKEN_GREATER, // 6 >
    TOKEN_LESS, // 7 <
    TOKEN_EQ, // 8 ==
    TOKEN_GREATEREQ, // 9
    TOKEN_LESSEQ, // 10
    TOKEN_STRING, // 11
    TOKEN_INT, // 12
    TOKEN_SEMI, // 13
    TOKEN_LPAREN, // 14
    TOKEN_RPAREN, // 15
    TOKEN_LBRACE, // 16
    TOKEN_RBRACE, // 17
    TOKEN_COMMA, // 18
    TOKEN_FUNC, // 19
    TOKEN_CALL, // 20
    TOKEN_IF, // 21
    TOKEN_RETURN, // 22
    TOKEN_EOF // 23
  } type;

  char* value;
} Token;

// char* typelut[] = {
  // [TOKEN_ID] = "TOKEN_ID",
// }

Token* new_token(int type, char* value);

void free_token(Token* token);

#endif // !TOKEN_H