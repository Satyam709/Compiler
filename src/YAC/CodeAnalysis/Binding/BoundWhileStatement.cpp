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

std::vector<const BoundNode*> BoundWhileStatement::getChildren() const {
    return {_condition, _body};
}

std::vector<std::pair<std::string, std::string>>
BoundWhileStatement::getProperties() const {
    return {};
}