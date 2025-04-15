//
// Created by satya on 15-04-2025.
//
#include "CompilationUnit.h"
#include "Expression.h"


CompilationUnit::CompilationUnit(ExpressionSyntax &root, SyntaxToken endOfFileToken):_exp(root), _eof(endOfFileToken) {
}


SyntaxKind CompilationUnit::getKind() const {
    return SyntaxKind::CompilationUnit;
}

const std::vector<SyntaxNode *> &CompilationUnit::getChildren() const {
    return _exp.getChildren();
}
