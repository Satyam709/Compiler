//
// Created by satya on 17-04-2025.
//

#include "ElseClauseSyntax.h"

ElseClauseSyntax::ElseClauseSyntax(const SyntaxToken& elseKeyword, StatementSyntax& elseStatement)
    : _elseKeyword(elseKeyword)
    , _elseStatement(elseStatement)
{
    _children.push_back(new SyntaxNodeToken(elseKeyword));
    _children.push_back(&elseStatement);
}

SyntaxKind ElseClauseSyntax::getKind() const {
    return SyntaxKind::ElseClause;
}

const std::vector<SyntaxNode*>& ElseClauseSyntax::getChildren() const {
    return _children;
}