#include "DiagnosticsBag.h"
#include <sstream>

void DiagnosticBag::addRange(DiagnosticBag &diagnostics) {
    if (!diagnostics.isEmpty())
    m_diagnostics.insert(m_diagnostics.end(), diagnostics.m_diagnostics.begin(), diagnostics.m_diagnostics.end());
}

void DiagnosticBag::report(const TextSpan& span, const std::string& message) {
    m_diagnostics.emplace_back(span, message);
}

void DiagnosticBag::reportInvalidNumber(const TextSpan& span, const std::string& text, const std::string& type) {
    std::stringstream ss;
    ss << "The number " << text << " isn't valid " << type << ".";
    report(span, ss.str());
}

void DiagnosticBag::reportBadCharacter(const int position, const char character) {
    TextSpan span(position, 1);
    std::stringstream ss;
    ss << "Bad character input: '" << character << "'.";
    report(span, ss.str());
}

void DiagnosticBag::reportUnexpectedToken(const TextSpan& span, const SyntaxKind actualKind, const SyntaxKind expectedKind) {
    std::stringstream ss;
    ss << "Unexpected token <" <<syntaxKindToString(actualKind) << ">, expected <" << syntaxKindToString(expectedKind) << ">.";
    report(span, ss.str());
}

void DiagnosticBag::reportUndefinedUnaryOperator(const TextSpan& span, const std::string& operatorText, const std::string& operandType) {
    std::stringstream ss;
    ss << "Unary operator '" << operatorText << "' is not defined for type " << operandType << ".";
    report(span, ss.str());
}

void DiagnosticBag::reportUndefinedBinaryOperator(const TextSpan& span, const std::string& operatorText, const std::string& leftType, const std::string& rightType) {
    std::stringstream ss;
    ss << "Binary operator '" << operatorText << "' is not defined for types " << leftType << " and " << rightType << ".";
    report(span, ss.str());
}

bool DiagnosticBag::isEmpty() const {
    return m_diagnostics.empty();
}
