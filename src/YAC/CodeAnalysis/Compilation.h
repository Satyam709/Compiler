#pragma once

#include "Syntax/SyntaxTree.h"
#include "Binding/Binder.h"
#include "Evaluator.h"
#include "EvaluationResult.h"
#include "Diagnostics.h"
#include <unordered_map>

// Class to manage the compilation process
class Compilation {
public:
    explicit Compilation(const SyntaxTree& syntaxTree);

    const SyntaxTree& getSyntaxTree() const { return _syntaxTree; }

    EvaluationResult evaluate(std::unordered_map<std::string, std::any>& variables);

private:
    const SyntaxTree& _syntaxTree;
};