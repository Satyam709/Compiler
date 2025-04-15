#pragma once

#include "Syntax/SyntaxTree.h"
#include "Binding/Binder.h"
#include "Evaluator.h"
#include "EvaluationResult.h"
#include "Diagnostics.h"
#include <unordered_map>
#include "Symbols/VariableSymbol.h"

// Class to manage the compilation process
class Compilation {
public:
    explicit Compilation(SyntaxTree& syntaxTree);

    Compilation(Compilation* previous , SyntaxTree* syntaxTree);

    [[nodiscard]] const SyntaxTree& getSyntaxTree() const { return _syntaxTree; }

    EvaluationResult evaluate(std::unordered_map<VariableSymbol, std::any>& variables) const;

    Compilation* continuwWith(SyntaxTree* syntax_tree) {
        return new Compilation(this,syntax_tree);
    }

    [[nodiscard]] BoundGlobalScope * bound_global_scope() const {
        return _boundGlobalScope;
    }

    [[nodiscard]] BoundGlobalScope* boundGlobalScope() const;


    [[nodiscard]] Compilation * previous() const {
        return _previous;
    }

private:
    const SyntaxTree& _syntaxTree;
    mutable BoundGlobalScope* _boundGlobalScope = nullptr;
    Compilation* _previous;
    mutable std::mutex _mutex;
};