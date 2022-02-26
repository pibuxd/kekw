#ifndef VISITOR_H
#define VISITOR_H
#include "parser.h"
#include "return.h"

void visit_compound(Parser* parser);

Return visit(Parser* parser, AST* ast, Variables* local_variables);

int visit_condition(Parser* parser, AST* ast, Variables* local_variables);

int visit_expr(Parser* parser, AST* ast, int val, Variables* local_variables);

void visit_assign_var(Parser* parser, AST* ast, Variables* local_variables);

int visit_get_var(Parser* parser, char* name, Variables* local_variables);

void visit_define_function(Parser* parser, AST* ast);

int visit_call_function(Parser* parser, AST* ast, Variables* local_variables);

int visit_print_function(Parser* parser, AST* ast, Variables* local_variables);

Return visit_if(Parser* parser, AST* ast, Variables* local_variables);

#endif // !VISITOR_H