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

std::vector<const BoundNode*> BoundBlockStatement::getChildren() const {
    std::vector<const BoundNode*> children;
    for (auto& stmt : _statements) {
        children.push_back(stmt);
    }
    return children;
}

std::vector<std::pair<std::string, std::string>>
BoundBlockStatement::getProperties() const {
    return {{"StatementCount", std::to_string(_statements.size())}};
}
