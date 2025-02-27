#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Binding/Binder.h"
#include "Syntax/Expression.h"
#include <unordered_map>
#include "Symbols/VariableSymbol.h"

class Evaluator {
public:
    explicit Evaluator(const BoundExpression& root, std::unordered_map<VariableSymbol, std::any>& variables);

    std::any evaluateExpression(const BoundExpression* node);
    std::any evaluate();

private:
    const BoundExpression& _root;
    std::unordered_map<VariableSymbol, std::any>& _variables;
};

#endif // EVALUATOR_H