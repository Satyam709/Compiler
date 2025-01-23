
#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Binding/Binder.h"
#include "CodeAnalysis/Syntax/Expression.h"

class Evaluator {
public:
    explicit Evaluator(const BoundExpression& root);

    int evaluateExpression(const BoundExpression* node);

    int evaluate();

private:
    const BoundExpression& _root;
};

#endif //EVALUATOR_H
