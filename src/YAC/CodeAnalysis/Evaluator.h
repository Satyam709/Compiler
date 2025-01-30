
#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Binding/Binder.h"
#include "Syntax/Expression.h"

class Evaluator {
public:
    explicit Evaluator(const BoundExpression& root);

    std::any evaluateExpression(const BoundExpression* node);

    std::any evaluate();

private:
    const BoundExpression& _root;
};

#endif //EVALUATOR_H
