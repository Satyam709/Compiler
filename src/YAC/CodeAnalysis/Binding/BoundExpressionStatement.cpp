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
