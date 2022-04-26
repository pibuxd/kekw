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
    TOKEN_CHAR, // 12
    TOKEN_INT, // 13
    TOKEN_SEMI, // 14
    TOKEN_LPAREN, // 15
    TOKEN_RPAREN, // 16
    TOKEN_LSQUARE, // 17
    TOKEN_RSQUARE, // 18
    TOKEN_LBRACE, // 19
    TOKEN_RBRACE, // 20
    TOKEN_COMMA, // 21
    TOKEN_FUNC, // 22
    TOKEN_CALL, // 23
    TOKEN_IF, // 24
    TOKEN_RETURN, // 25
    TOKEN_WHILE, // 26
    TOKEN_EOF // 27
  } type;

  char* value;
} Token;

static const char* const token_map[] = {
  [TOKEN_ID] = "TOKEN_ID",
  [TOKEN_PLUS] = "TOKEN_PLUS",
  [TOKEN_MINUS] = "TOKEN_MINUS",
  [TOKEN_MUL] = "TOKEN_MUL",
  [TOKEN_DIV] = "TOKEN_DIV",
  [TOKEN_EQUALS] = "TOKEN_EQUALS",
  [TOKEN_GREATER] = "TOKEN_GREATER",
  [TOKEN_LESS] = "TOKEN_LESS",
  [TOKEN_EQ] = "TOKEN_EQ",
  [TOKEN_GREATEREQ] = "TOKEN_GREATEREQ",
  [TOKEN_LESSEQ] = "TOKEN_LESSEQ",
  [TOKEN_STRING] = "TOKEN_STRING",
  [TOKEN_CHAR] = "TOKEN_CHAR",
  [TOKEN_INT] = "TOKEN_INT",
  [TOKEN_SEMI] = "TOKEN_SEMI",
  [TOKEN_LPAREN] = "TOKEN_LPAREN",
  [TOKEN_RPAREN] = "TOKEN_RPAREN",
  [TOKEN_LBRACE] = "TOKEN_LBRACE",
  [TOKEN_RBRACE] = "TOKEN_RBRACE",
  [TOKEN_LSQUARE] = "TOKEN_LSQUARE",
  [TOKEN_RSQUARE] = "TOKEN_RSQUARE",
  [TOKEN_COMMA] = "TOKEN_COMMA",
  [TOKEN_FUNC] = "TOKEN_FUNC",
  [TOKEN_CALL] = "TOKEN_CALL",
  [TOKEN_IF] = "TOKEN_IF",
  [TOKEN_WHILE] = "TOKEN_WHILE",
  [TOKEN_RETURN] = "TOKEN_RETURN",
  [TOKEN_EOF] = "TOKEN_EOF",
};

Token* new_token(int type, char* value);

void free_token(Token* token);

#endif // !TOKEN_H