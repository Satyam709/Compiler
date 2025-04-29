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

std::vector<const BoundNode*> BoundVariableDeclaration::getChildren() const {
    return {_initializer}; // Declaration has the initializer as a child
}

std::vector<std::pair<std::string, std::string>>
BoundVariableDeclaration::getProperties() const {
    return {
            {"Name", _variable->getName()},
            {"Type", _variable->getType().name()}
    };
}