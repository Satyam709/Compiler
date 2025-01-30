#include "Compilation.h"

// Constructor to initialize the Compilation with a syntax tree
Compilation::Compilation(const SyntaxTree& syntaxTree)
    : _syntaxTree(syntaxTree) {}

// Method to evaluate the syntax tree
EvaluationResult Compilation::evaluate() {
    // Create a Binder to bind the syntax tree
    Binder binder;
    auto boundExpression = binder.bindExpression(_syntaxTree.root());

    // Collect diagnostics from the syntax tree and binder
    std::vector<std::string> diagnostics;
    diagnostics.insert(diagnostics.end(),
                       _syntaxTree.diagnostics().begin(),
                       _syntaxTree.diagnostics().end());
    diagnostics.insert(diagnostics.end(),
                       binder.diagnostics().begin(),
                       binder.diagnostics().end());

    // If there are diagnostics, return them with an empty value
    if (!diagnostics.empty()) {
        return EvaluationResult(diagnostics, std::any());
    }

    // Create an Evaluator to evaluate the bound expression
    Evaluator evaluator(*boundExpression);
    auto value = evaluator.evaluate();

    // Return the evaluation result with the value and no diagnostics
    return EvaluationResult({}, value);
}