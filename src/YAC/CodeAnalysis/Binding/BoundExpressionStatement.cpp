//
// Created by satya on 16-04-2025.
//

#include "BoundExpressionStatement.h"

BoundExpressionStatement::BoundExpressionStatement(const BoundExpression* expression)
    : _expression(expression) {
}

BoundNodeKind BoundExpressionStatement::getKind() const {
    return BoundNodeKind::ExpressionStatement;
}

const BoundExpression* BoundExpressionStatement::expression() const {
    return _expression;
}

std::vector<const BoundNode*> BoundExpressionStatement::getChildren() const {
    return { _expression };
}

std::vector<std::pair<std::string, std::string>>
BoundExpressionStatement::getProperties() const {
    return {};
}