//
// Created by satya on 17-04-2025.
//

#ifndef BOUNDIFSTATEMENT_H
#define BOUNDIFSTATEMENT_H

#include "BoundStatement.h"

class BoundIfStatement final : public BoundStatement {
public:
    BoundIfStatement(const BoundExpression* condition,
                     const BoundStatement* thenStatement,
                     const BoundStatement* elseStatement);

    [[nodiscard]] BoundNodeKind getKind() const override;

    [[nodiscard]] const BoundExpression* condition() const { return _condition; }
    [[nodiscard]] const BoundStatement* thenStatement() const { return _thenStatement; }
    [[nodiscard]] const BoundStatement* elseStatement() const { return _elseStatement; }

private:
    const BoundExpression* _condition;
    const BoundStatement* _thenStatement;
    const BoundStatement* _elseStatement;
};

#endif // BOUNDIFSTATEMENT_H