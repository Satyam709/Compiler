//
// Created by satya on 17-04-2025.
//

#include "BoundForStatement.h"

BoundForStatement::BoundForStatement(const VariableSymbol& variable,
                                   const BoundExpression* lowerBound,
                                   const BoundExpression* upperBound,
                                   BoundStatement* body)
    : _variable(variable)
    , _lowerBound(lowerBound)
    , _upperBound(upperBound)
    , _body(body) {
}

BoundNodeKind BoundForStatement::getKind() const {
    return BoundNodeKind::ForStatement;
}