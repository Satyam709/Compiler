
#include "Compilation.h"

// Constructor to initialize the Compilation with a syntax tree
Compilation::Compilation(const SyntaxTree& syntaxTree)
    : _syntaxTree(syntaxTree) ,diagnostic_bag(new DiagnosticBag()){}

// Method to evaluate the syntax tree
EvaluationResult Compilation::evaluate() {
    // Create a Binder to bind the syntax tree
    Binder binder{};
    const auto boundExpression = binder.bindExpression(_syntaxTree.root());

    // Collect diagnostics from the syntax tree and binder
    diagnostic_bag->addRange(*_syntaxTree.diagnostics());
    diagnostic_bag->addRange(*binder.diagnostics());

    // std::cout << diagnostic_bag->isEmpty();

    // If there are diagnostics, return them with an empty value
    if (!diagnostic_bag->isEmpty()) {
        return EvaluationResult(diagnostic_bag->getDiagnostics(), nullptr);
    }

    // Create an Evaluator to evaluate the bound expression
    Evaluator evaluator(*boundExpression);
    const auto value = evaluator.evaluate();

    // Return the evaluation result with the value and no diagnostics
    return {{}, value};
}