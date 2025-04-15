#include "Compilation.h"
#include "Syntax/CompilationUnit.h"


Compilation::Compilation(const SyntaxTree& syntaxTree)
    : _syntaxTree(syntaxTree) {
}

EvaluationResult Compilation::evaluate(std::unordered_map<VariableSymbol, std::any>& variables) const{
    // Create a Binder with the copy of existing variables

    Binder binder(variables);
    const auto boundExpression = binder.bindExpression(_syntaxTree.root()->exp());

    // Collect diagnostics
    DiagnosticBag* diagnostic_bag = binder.diagnostics();

    const auto syntaxDiagnostics = _syntaxTree.diagnostics();
    diagnostic_bag->addRange(*syntaxDiagnostics);


    if (!diagnostic_bag->isEmpty()) {
        return EvaluationResult(diagnostic_bag->getDiagnostics(), nullptr);
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