#include "Compilation.h"
#include "Syntax/CompilationUnitSyntax.h"


Compilation::Compilation(SyntaxTree &syntaxTree): _syntaxTree(syntaxTree),_previous(nullptr) {
}

Compilation::Compilation(Compilation *previous, SyntaxTree *syntaxTree): _syntaxTree(*syntaxTree), _previous(previous) {
}

BoundGlobalScope* Compilation::boundGlobalScope() const {
    if (_boundGlobalScope == nullptr) {
        std::lock_guard<std::mutex> lock(_mutex);  // Ensure thread safety
        if (_boundGlobalScope == nullptr) {
            BoundGlobalScope* previousScope = _previous ? _previous->boundGlobalScope() : nullptr;
            _boundGlobalScope = Binder::BindGlobalScope(previousScope, _syntaxTree.root());
        }
    }
    return _boundGlobalScope;
}


EvaluationResult Compilation::evaluate(std::unordered_map<VariableSymbol, std::any>& variables) const {
    const BoundGlobalScope* scope = boundGlobalScope();

    auto* diagnostics = new DiagnosticBag();
    diagnostics->addRange(*_syntaxTree.diagnostics());
    diagnostics->addRange(scope->diagnostics());

    if (!diagnostics->isEmpty()) {
        return EvaluationResult(diagnostics->getDiagnostics(), nullptr);
    }

    Evaluator evaluator(*scope->statement(), variables);
    const std::any value = evaluator.evaluate();

    // Debug output
    std::cout << "Variables in Compilation after evaluation:\n";
    for (const auto& [var, val] : variables) {
        std::cout << var.getName() << " = ";
        if (val.type() == typeid(int)) {
            std::cout << std::any_cast<int>(val);
        } else if (val.type() == typeid(bool)) {
            std::cout << std::boolalpha << std::any_cast<bool>(val);
        } else {
            std::cout << "[unknown type]";
        }
        std::cout << std::endl;
    }

    return EvaluationResult({}, value);
}

void Compilation::printBoundTree(std::ostream& out) const {
    const BoundGlobalScope* scope = boundGlobalScope();
    if (!scope) return;

    if (scope->statement()) {
        scope->statement()->writeTo(out);
    } else {
        out << "No bound tree available\n";
    }
}