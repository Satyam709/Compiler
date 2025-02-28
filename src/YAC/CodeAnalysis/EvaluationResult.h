#pragma once

#include <any>
#include <vector>
#include "Diagnostics.h"  // Make sure to include the Diagnostic header

// Class to encapsulate the result of an evaluation
class EvaluationResult {
public:
    EvaluationResult(std::vector<Diagnostic> diagnostics, std::any value);

    const std::vector<Diagnostic>& diagnostics() const;
    const std::any& value() const;

private:
    std::vector<Diagnostic> _diagnostics;
    std::any _value;
};
