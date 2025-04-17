//
// Created by satya on 17-04-2025.
//

#include "BoundIfStatement.h"

BoundIfStatement::BoundIfStatement(const BoundExpression* condition,
                                 const BoundStatement* thenStatement,
                                 const BoundStatement* elseStatement)
    : _condition(condition)
    , _thenStatement(thenStatement)
    , _elseStatement(elseStatement)
{
}

BoundNodeKind BoundIfStatement::getKind() const {
    return BoundNodeKind::IfStatement;
}