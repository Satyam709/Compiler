//
// Created by satya on 01-01-2025.
//
#ifndef SYNTAXFACTS_H
#define SYNTAXFACTS_H
#include <vector>

#include "Syntax.h"


class SyntaxFacts {
public:
    static int getPrecedence(const SyntaxKind &kind);
    static int getUnaryPrecedence(const SyntaxKind &kind);
    static SyntaxKind getKeywordKind(const std::string& text);
    static std::vector<SyntaxKind> GetUnaryOperatorKinds();
    static std::vector<SyntaxKind> GetBinaryOperatorKinds();
};


#endif //SYNTAXFACTS_H
