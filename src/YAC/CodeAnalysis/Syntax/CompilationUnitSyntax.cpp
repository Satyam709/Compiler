//
// Created by satya on 15-04-2025.
//
#include "CompilationUnitSyntax.h"
#include "Expression.h"


CompilationUnitSyntax::CompilationUnitSyntax(ExpressionSyntax &root, SyntaxToken endOfFileToken):_exp(root), _eof(endOfFileToken) {
}


SyntaxKind CompilationUnitSyntax::getKind() const {
    return SyntaxKind::CompilationUnit;
}

const std::vector<SyntaxNode *> &CompilationUnitSyntax::getChildren() const {
    return _exp.getChildren();
}
