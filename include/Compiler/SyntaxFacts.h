//
// Created by satya on 01-01-2025.
//
#ifndef SYNTAXFACTS_H
#define SYNTAXFACTS_H
#include "Syntax.h"


class SyntaxFacts {
public:
    static int getPrecedence(const SyntaxKind &kind);
};


#endif //SYNTAXFACTS_H
