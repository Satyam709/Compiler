#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <string>
#include "Text/TextSpan.h"

class Diagnostic {
public:
    Diagnostic(const TextSpan& span, const std::string& message);

    const TextSpan& getSpan() const;
    const std::string& getMessage() const;

    std::string toString() const;

private:
    TextSpan m_span;
    std::string m_message;
};

#endif // DIAGNOSTIC_H
