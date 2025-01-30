#pragma once

#include "Syntax/SyntaxTree.h"
#include "Binding/Binder.h"
#include "Evaluator.h"
#include "EvaluationResult.h" // Include the EvaluationResult header

// Class to manage the compilation process
class Compilation {
public:
    explicit Compilation(const SyntaxTree& syntaxTree);

    EvaluationResult evaluate();

private:
    const SyntaxTree& _syntaxTree;
};