#include "DiagnosticsBag.h"

#include <iostream>
#include <sstream>

#include "YAC/Utils/Caster.h"

void DiagnosticBag::addRange(DiagnosticBag &diagnostics) {
    if (!diagnostics.isEmpty())
        m_diagnostics.insert(m_diagnostics.end(), diagnostics.m_diagnostics.begin(), diagnostics.m_diagnostics.end());
}

void DiagnosticBag::addRange(std::vector<Diagnostic> diag_vec) {
    if (!diag_vec.empty()) {
        m_diagnostics.insert(m_diagnostics.end(), diag_vec.begin(), diag_vec.end());
    }
}

void DiagnosticBag::report(const TextSpan &span, const std::string &message) {
    const Diagnostic diag = {span, message};
    m_diagnostics.push_back(diag);
}

void DiagnosticBag::reportInvalidNumber(const TextSpan &span, const std::string &text, const std::string &type) {
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

void DiagnosticBag::reportUnexpectedToken(const TextSpan &span, const SyntaxKind actualKind,
                                          const SyntaxKind expectedKind) {
    std::stringstream ss;
    ss << "Unexpected token <" << syntaxKindToString(actualKind) << ">, expected <" << syntaxKindToString(expectedKind)
            << ">.";
    report(span, ss.str());
}

void DiagnosticBag::reportUndefinedUnaryOperator(const TextSpan &span, const std::string &operatorText,
                                                 const std::string &operandType) {
    std::stringstream ss;
    ss << "Unary operator '" << operatorText << "' is not defined for type " << operandType << ".";
    report(span, ss.str());
}

void DiagnosticBag::reportUndefinedBinaryOperator(const TextSpan &span, const std::string &operatorText,
                                                  const std::string &leftType, const std::string &rightType) {
    std::stringstream ss;
    ss << "Binary operator '" << operatorText << "' is not defined for types " << leftType << " and " << rightType <<
            ".";
    report(span, ss.str());
}

void DiagnosticBag::reportUndefinedName(const TextSpan &span, const std::string &name) {
    std::stringstream ss;
    ss << "Variable '" << name << "' does not exist.";
    report(span, ss.str());
}

void DiagnosticBag::reportCannotConvert(const TextSpan &span, const std::type_info &fromType,
                                        const std::type_info &toType) {
    std::stringstream ss;
    ss << "cannot convert type '" << getTypeName(fromType) << " to " << getTypeName(toType) << ".";
    report(span, ss.str());
}

void DiagnosticBag::reportVariableAlreadyDeclared(const TextSpan& span, const std::string& name) {
    const auto message = "Variable '" + name + "' is already declared.";
    report(span, message);
}

void DiagnosticBag::reportCannotAssign(const TextSpan& span, const std::string& name) {
    const auto message = "Variable '" + name + "' is read-only and cannot be assigned to.";
    report(span, message);
}


bool DiagnosticBag::isEmpty() const {
    return m_diagnostics.empty();
}
