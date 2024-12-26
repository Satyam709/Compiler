
#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Compiler/Expression.h"

class Evaluator {
public:
    Evaluator( ExpressionSyntax& root);

    int evaluateExpression(ExpressionSyntax* node);

    int evaluate();

private:
     ExpressionSyntax& _root;
};

#endif //EVALUATOR_H
