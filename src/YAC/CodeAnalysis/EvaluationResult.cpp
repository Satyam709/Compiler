#include "EvaluationResult.h"

// Constructor to initialize diagnostics and value
EvaluationResult::EvaluationResult(std::vector<Diagnostic> diagnostics, std::any value)
    : _diagnostics(std::move(diagnostics)), _value(std::move(value)) {}

// Accessor for diagnostics
const std::vector<Diagnostic>& EvaluationResult::diagnostics() const {
    return _diagnostics;
}

// Accessor for value
const std::any& EvaluationResult::value() const {
    return _value;
}

