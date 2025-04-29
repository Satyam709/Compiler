//
// Created by satya on 16-04-2025.
//

#ifndef BOUNDEXPRESSIONSTATEMENT_H
#define BOUNDEXPRESSIONSTATEMENT_H

#include "BoundStatement.h"

class BoundExpressionStatement final : public BoundStatement {
public:
    explicit BoundExpressionStatement(const BoundExpression* expression);

    [[nodiscard]] BoundNodeKind getKind() const override;
    [[nodiscard]] const BoundExpression* expression() const;

    std::vector<const BoundNode*> getChildren() const override;
    std::vector<std::pair<std::string, std::string>> getProperties() const override;

private:
    const BoundExpression* _expression;
};

#endif // BOUNDEXPRESSIONSTATEMENT_H
