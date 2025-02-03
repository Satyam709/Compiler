#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Binding/Binder.h"
#include "Syntax/Expression.h"
#include <unordered_map>

class Evaluator {
public:
    explicit Evaluator(const BoundExpression& root, std::unordered_map<std::string, std::any>& variables);

    std::any evaluateExpression(const BoundExpression* node);

    std::any evaluate();

private:
    const BoundExpression& _root;
    std::unordered_map<std::string, std::any>& _variables;
};

#endif //EVALUATOR_H