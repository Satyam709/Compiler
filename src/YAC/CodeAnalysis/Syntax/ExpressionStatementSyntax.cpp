#include "ExpressionStatementSyntax.h"

ExpressionStatementSyntax::ExpressionStatementSyntax(ExpressionSyntax& expression)
    : _expression(expression) {
    _children.push_back(&expression);
}

SyntaxKind ExpressionStatementSyntax::getKind() const {
    return SyntaxKind::ExpressionStatement;
}

const std::vector<SyntaxNode*>& ExpressionStatementSyntax::getChildren() const {
    return _children;
}

ExpressionSyntax& ExpressionStatementSyntax::expression() const {
    return _expression;
}