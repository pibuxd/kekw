#ifndef VISITOR_H
#define VISITOR_H
#include "parser.h"
#include "return.h"
#include "var.h"

void visit_compound(Parser* parser);

Return* visit(Parser* parser, AST* ast, Variables* local_variables);

Var* visit_condition(Parser* parser, AST* ast, Variables* local_variables);

Var* visit_condition(Parser* parser, AST* ast, Variables* local_variables);

Var* visit_expr(Parser* parser, AST* ast, int val, Variables* local_variables);

void visit_assign_var(Parser* parser, AST* ast, Variables* local_variables);

Var* visit_get_var(Parser* parser, char* name, Variables* local_variables);

void visit_define_function(Parser* parser, AST* ast);

Var* visit_call_function(Parser* parser, AST* ast, Variables* local_variables);

int visit_print_function(Parser* parser, AST* ast, Variables* local_variables);

Var* visit_int_function(Parser* parser, AST* ast, Variables* local_variables);

Var* visit_char_function(Parser* parser, AST* ast, Variables* local_variables);

Return* visit_if(Parser* parser, AST* ast, Variables* local_variables);

#endif // !VISITOR_H