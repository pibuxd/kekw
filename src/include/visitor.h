#ifndef VISITOR_H
#define VISITOR_H
#include "parser.h"

void visit_compound(Parser* parser);

void visit(Parser* parser, AST* ast);

int visit_condition(Parser* parser, AST* ast);

int visit_expr(Parser* parser, AST* ast, int val);

void visit_assign_var(Parser* parser, AST* ast);

void visit_call_function(Parser* parser, AST* ast);

int visit_get_var(Parser* parser, char* name);

void visit_print_function(Parser* parser, AST* ast);

#endif // !VISITOR_H