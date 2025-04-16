//
// Created by satya on 16-04-2025.
//

#include "BoundBlockStatement.h"

BoundBlockStatement::BoundBlockStatement(const std::vector<BoundStatement*>& statements)
    : _statements(statements) {
}

BoundNodeKind BoundBlockStatement::getKind() const {
    return BoundNodeKind::BlockStatement;
}

const std::vector<BoundStatement*>& BoundBlockStatement::statements() const {
    return _statements;
}
