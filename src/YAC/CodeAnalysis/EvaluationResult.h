#pragma once

#include <any>
#include <vector>
#include <string>

// Class to encapsulate the result of an evaluation
class EvaluationResult {
public:
    EvaluationResult(std::vector<std::string> diagnostics, std::any value);

    const std::vector<std::string>& diagnostics() const;
    const std::any& value() const;

private:
    std::vector<std::string> _diagnostics;
    std::any _value;
};