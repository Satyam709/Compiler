
#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "CodeAnalysis/Syntax/Expression.h"

class Evaluator {
public:
    explicit Evaluator( ExpressionSyntax& root);

    int evaluateExpression(ExpressionSyntax* node);

    int evaluate();

private:
     ExpressionSyntax& _root;
};

#endif //EVALUATOR_H
