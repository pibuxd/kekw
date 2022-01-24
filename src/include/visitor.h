#ifndef VISITOR_H
#define VISITOR_H
#include "parser.h"

void visit_compound(Parser* parser);

void visit(Parser* parser, AST* ast, int i, int* local_variables, int* local_variables_exi);

int visit_condition(Parser* parser, AST* ast, int* local_variables, int* local_variables_exi);

int visit_expr(Parser* parser, AST* ast, int val, int* local_variables, int* local_variables_exi);

void visit_assign_var(Parser* parser, AST* ast, int* local_variables, int* local_variables_exi);

int visit_get_var(Parser* parser, char* name, int* local_variables, int* local_variables_exi);

void visit_define_function(Parser* parser, AST* ast);

void visit_call_function(Parser* parser, AST* ast, int* local_variables, int* local_variables_exi);

void visit_print_function(Parser* parser, AST* ast, int* local_variables, int* local_variables_exi);

#endif // !VISITOR_H