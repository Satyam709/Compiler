#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Binding/Binder.h"
#include <unordered_map>

#include "Binding/BoundBlockStatement.h"
#include "Binding/BoundExpressionStatement.h"
#include "Binding/BoundIfStatement.h"
#include "Binding/BoundStatement.h"
#include "Binding/BoundVariableDeclaration.h"
#include "Binding/BoundWhileStatement.h"
#include "Symbols/VariableSymbol.h"

class Evaluator {
public:
    explicit Evaluator(const BoundStatement& root, std::unordered_map<VariableSymbol, std::any>& variables);

    void EvaluateStatement(const BoundStatement *node);

    void EvaluateVariableDeclaration(const BoundVariableDeclaration *node);

    void EvaluateBlockStatement(const BoundBlockStatement *node);

    void EvaluateIfStatement(const BoundIfStatement *node);

    void EvaluateWhileStatement(const BoundWhileStatement *node);

    void EvaluateExpressionStatement(const BoundExpressionStatement *node);

    std::any evaluateExpression(const BoundExpression* node);
    std::any evaluate();

private:
    const BoundStatement& _root;
    std::unordered_map<VariableSymbol, std::any>& _variables;
    std::any _lastValue;
};

#endif // EVALUATOR_H