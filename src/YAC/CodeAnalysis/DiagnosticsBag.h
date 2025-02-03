#ifndef DIAGNOSTIC_BAG_H
#define DIAGNOSTIC_BAG_H

#include <iostream>
#include <vector>
#include <string>
#include "Diagnostics.h"
#include "Text/TextSpan.h"
#include "Syntax/Syntax.h"

class DiagnosticBag {
public:
    DiagnosticBag() : m_diagnostics(std::vector<Diagnostic>()) {}

    void addRange(DiagnosticBag& diagnostics);
    void reportInvalidNumber(const TextSpan& span, const std::string& text, const std::string& type);
    void reportBadCharacter(int position, char character);
    void reportUnexpectedToken(const TextSpan& span, SyntaxKind actualKind, SyntaxKind expectedKind);
    void reportUndefinedUnaryOperator(const TextSpan& span, const std::string& operatorText, const std::string& operandType);
    void reportUndefinedBinaryOperator(const TextSpan& span, const std::string& operatorText, const std::string& leftType, const std::string& rightType);
    bool isEmpty() const ;
    const std::vector<Diagnostic>& getDiagnostics() const { return m_diagnostics; }

private:
    void report(const TextSpan& span, const std::string& message);
    std::vector<Diagnostic> m_diagnostics;
};

#endif // DIAGNOSTIC_BAG_H
