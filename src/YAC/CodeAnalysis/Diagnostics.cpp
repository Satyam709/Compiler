#include "Diagnostics.h"

Diagnostic::Diagnostic(const TextSpan& span, const std::string& message)
    : m_span(span), m_message(message) {}

const TextSpan& Diagnostic::getSpan() const {
    return m_span;
}

const std::string& Diagnostic::getMessage() const {
    return m_message;
}

std::string Diagnostic::toString() const {
    return m_message;
}
