//
// Created by satya on 17-04-2025.
//

#include "WhileStatementSyntax.h"

WhileStatementSyntax::WhileStatementSyntax(const SyntaxToken& whileKeyword,
                                         ExpressionSyntax& condition,
                                         StatementSyntax& body)
    : _whileKeyword(whileKeyword)
    , _condition(condition)
    , _body(body) {
    _children.push_back(new SyntaxNodeToken(whileKeyword));
    _children.push_back(&condition);
    _children.push_back(&body);
}

SyntaxKind WhileStatementSyntax::getKind() const {
    return SyntaxKind::WhileStatement;
}

const std::vector<SyntaxNode*>& WhileStatementSyntax::getChildren() const {
    return _children;
}