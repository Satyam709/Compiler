//
// Created by satya on 17-04-2025.
//

#include "IfStatementSyntax.h"

IfStatementSyntax::IfStatementSyntax(const SyntaxToken& ifKeyword,
                                   ExpressionSyntax& condition,
                                   StatementSyntax& thenStatement,
                                   ElseClauseSyntax* elseClause)
    : _ifKeyword(ifKeyword)
    , _condition(condition)
    , _thenStatement(thenStatement)
    , _elseClause(elseClause)
{
    _children.push_back(new SyntaxNodeToken(ifKeyword));
    _children.push_back(&condition);
    _children.push_back(&thenStatement);
    if (elseClause) {
        _children.push_back(elseClause);
    }
}

SyntaxKind IfStatementSyntax::getKind() const {
    return SyntaxKind::IfStatement;
}

const std::vector<SyntaxNode*>& IfStatementSyntax::getChildren() const {
    return _children;
}