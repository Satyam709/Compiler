#pragma once

#include "Syntax/SyntaxTree.h"
#include "Binding/Binder.h"
#include "Evaluator.h"
#include "EvaluationResult.h" // Include the EvaluationResult header
#include "Diagnostics.h"

// Class to manage the compilation process
class Compilation {
public:
    explicit Compilation(const SyntaxTree& syntaxTree);
    DiagnosticBag *diagnostic_bag;

    EvaluationResult evaluate();

private:
    const SyntaxTree& _syntaxTree;
};