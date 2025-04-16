//
// Created by satya on 17-04-2025.
//

#include "BoundVariableDeclaration.h"

BoundVariableDeclaration::BoundVariableDeclaration(VariableSymbol* variable,const BoundExpression* initializer)
    : _variable(variable)
    , _initializer(initializer) {
}

BoundNodeKind BoundVariableDeclaration::getKind() const {
    return BoundNodeKind::VariableDeclaration;
}

VariableSymbol* BoundVariableDeclaration::getVariable() const {
    return _variable;
}

const BoundExpression* BoundVariableDeclaration::getInitializer() const {
    return _initializer;
}