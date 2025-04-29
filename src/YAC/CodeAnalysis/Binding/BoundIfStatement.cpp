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

std::vector<const BoundNode*> BoundIfStatement::getChildren() const {
    std::vector<const BoundNode*> children;
    children.push_back(_condition);
    children.push_back(_thenStatement);
    if (_elseStatement) children.push_back(_elseStatement);
    return children;
}

std::vector<std::pair<std::string, std::string>>
BoundIfStatement::getProperties() const {
    return {}; // No additional properties needed
}