//
// Created by satya on 16-04-2025.
//

#include "BlockStatementSyntax.h"


BlockStatementSyntax::BlockStatementSyntax(const SyntaxToken& openBraceToken,
                                           const std::vector<StatementSyntax*>& statements,
                                           const SyntaxToken& closeBraceToken)
    : _openBraceToken(openBraceToken)
    , _closeBraceToken(closeBraceToken)
    , _statements(statements) {
    _children.push_back(new SyntaxNodeToken(openBraceToken));
    for (auto* statement : statements) {
        _children.push_back(statement);
    }
    _children.push_back(new SyntaxNodeToken(closeBraceToken));
}

SyntaxKind BlockStatementSyntax::getKind() const {
    return SyntaxKind::BlockStatement;
}

const std::vector<SyntaxNode*>& BlockStatementSyntax::getChildren() const {
    return _children;
}

const SyntaxToken& BlockStatementSyntax::getOpenBraceToken() const {
    return _openBraceToken;
}

const SyntaxToken& BlockStatementSyntax::getCloseBraceToken() const {
    return _closeBraceToken;
}

const std::vector<StatementSyntax*>& BlockStatementSyntax::statements() const {
    return _statements;
}