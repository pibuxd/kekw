#ifndef VISITOR_H
#define VISITOR_H
#include "parser.h"
#include "return.h"
#include "var.h"
#include "scope.h"

void visit_compound(Parser* parser);

Return* visit(Parser* parser, AST* ast, Scope* scope);

Var* visit_condition(Parser* parser, AST* ast, Scope* scope);

Var* visit_condition(Parser* parser, AST* ast, Scope* scope);

Var* visit_expr(Parser* parser, AST* ast, int val, Scope* scope);

void visit_assign_var(Parser* parser, AST* ast, Scope* scope);

Var* visit_get_var(Parser* parser, char* name, Scope* scope);

Var* visit_define_function(Parser* parser, AST* ast, Scope* scope);

Var* visit_call_function(Parser* parser, AST* ast, Scope* scope, Var* funcv);

int visit_print_function(Parser* parser, AST* ast, Scope* scope);

Var* visit_int_function(Parser* parser, AST* ast, Scope* scope);

Var* visit_char_function(Parser* parser, AST* ast, Scope* scope);

Var* visit_type_function(Parser* parser, AST* ast, Scope* scope);

Return* visit_if(Parser* parser, AST* ast, Scope* scope);

Return* visit_while(Parser* parser, AST* ast, Scope* scope);

#endif // !VISITOR_H