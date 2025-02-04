#include "Compilation.h"

Compilation::Compilation(const SyntaxTree& syntaxTree)
    : _syntaxTree(syntaxTree) {
}

EvaluationResult Compilation::evaluate(std::unordered_map<VariableSymbol, std::any>& variables) {
    // Create a Binder with the existing variables
    Binder binder(variables);  // Make sure Binder constructor takes variables by reference
    const auto boundExpression = binder.bindExpression(_syntaxTree.root());

    // Collect diagnostics
    std::vector<Diagnostic> diagnostics;

    auto syntaxDiagnostics = _syntaxTree.diagnostics()->getDiagnostics();
    diagnostics.insert(diagnostics.end(),
                      syntaxDiagnostics.begin(),
                      syntaxDiagnostics.end());

    auto binderDiagnostics = binder.diagnostics()->getDiagnostics();
    diagnostics.insert(diagnostics.end(),
                      binderDiagnostics.begin(),
                      binderDiagnostics.end());

    if (!diagnostics.empty()) {
        return EvaluationResult(diagnostics, nullptr);
    }

    // Create Evaluator with the variables
    Evaluator evaluator(*boundExpression, variables);  // Make sure Evaluator uses the same variables
    const auto value = evaluator.evaluate();

    // Debug output (temporary)
    std::cout << "Variables in Compilation after evaluation:" << std::endl;
    for(const auto& [var, val] : variables) {
        std::cout << var.getName() << " = ";
        if (val.type() == typeid(int)) {
            std::cout << std::any_cast<int>(val);
        } else if (val.type() == typeid(bool)) {
            std::cout << std::boolalpha << std::any_cast<bool>(val);
        }
        std::cout << std::endl;
    }

    return EvaluationResult(std::vector<Diagnostic>{}, value);
}