
#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Compiler/Expression.h"
#include "Compiler/Binder.h"

class Evaluator {
public:
    Evaluator( BoundExpression& root);

    int evaluateExpression(BoundExpression* node);

    int evaluate();

private:
     BoundExpression& _root;
};

#endif //EVALUATOR_H
