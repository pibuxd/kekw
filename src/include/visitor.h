#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"

void visit_noop(AST* ast);

void visit_assign_var(AST* ast);

void visit_get_var(AST* ast);

#endif // !VISITOR_H