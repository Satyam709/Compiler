#include "Compilation.h"

// Constructor to initialize the Compilation with a syntax tree
Compilation::Compilation(const SyntaxTree& syntaxTree)
    : _syntaxTree(syntaxTree) {
}

// Method to evaluate the syntax tree
EvaluationResult Compilation::evaluate(std::unordered_map<std::string, std::any>& variables) {
    // Create a Binder to bind the syntax tree
    Binder binder(variables);
    const auto boundExpression = binder.bindExpression(_syntaxTree.root());

    // Create a vector to store all diagnostics
    std::vector<Diagnostic> diagnostics;

    // Collect diagnostics from the syntax tree
    auto syntaxDiagnostics = _syntaxTree.diagnostics()->getDiagnostics();
    diagnostics.insert(diagnostics.end(),
                      syntaxDiagnostics.begin(),
                      syntaxDiagnostics.end());

    // Collect diagnostics from the binder
    auto binderDiagnostics = binder.diagnostics()->getDiagnostics();
    diagnostics.insert(diagnostics.end(),
                      binderDiagnostics.begin(),
                      binderDiagnostics.end());

    // If there are diagnostics, return them with an empty value
    if (!diagnostics.empty()) {
        return EvaluationResult(diagnostics, nullptr);
    }

    // Create an Evaluator to evaluate the bound expression
    Evaluator evaluator(*boundExpression, variables);
    const auto value = evaluator.evaluate();

    // Return the evaluation result with the value and empty diagnostics
    return EvaluationResult(std::vector<Diagnostic>{}, value);
}