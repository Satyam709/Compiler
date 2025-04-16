//
// Created by satya on 15-04-2025.
//
#include "CompilationUnitSyntax.h"
#include "StatementSyntax.h"


CompilationUnitSyntax::CompilationUnitSyntax(StatementSyntax &root, SyntaxToken endOfFileToken):_statement(root), _eof(endOfFileToken) {
}


SyntaxKind CompilationUnitSyntax::getKind() const {
    return SyntaxKind::CompilationUnit;
}

const std::vector<SyntaxNode *> &CompilationUnitSyntax::getChildren() const {
    return _statement.getChildren();
}
