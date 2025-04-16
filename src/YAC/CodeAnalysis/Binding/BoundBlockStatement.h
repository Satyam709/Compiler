//
// Created by satya on 16-04-2025.
//

#ifndef BOUNDBLOCKSTATEMENT_H
#define BOUNDBLOCKSTATEMENT_H

#include "BoundStatement.h"
#include <vector>

class BoundBlockStatement final : public BoundStatement {
public:
    explicit BoundBlockStatement(const std::vector<BoundStatement*>& statements);

    BoundNodeKind getKind() const override;
    const std::vector<BoundStatement*>& statements() const;

private:
    std::vector<BoundStatement*> _statements;
};

#endif // BOUNDBLOCKSTATEMENT_H
