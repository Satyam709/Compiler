//
// Created by satya on 17-04-2025.
//

#include "BoundWhileStatement.h"

BoundWhileStatement::BoundWhileStatement(const BoundExpression* condition, const BoundStatement* body)
    : _condition(condition)
    , _body(body) {
}

BoundNodeKind BoundWhileStatement::getKind() const {
    return BoundNodeKind::WhileStatement;
}