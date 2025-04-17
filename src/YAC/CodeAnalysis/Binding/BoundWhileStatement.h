//
// Created by satya on 17-04-2025.
//

#ifndef BOUNDWHILESTATEMENT_H
#define BOUNDWHILESTATEMENT_H

#include "BoundStatement.h"

class BoundWhileStatement : public BoundStatement {
public:
    BoundWhileStatement(const BoundExpression* condition, const BoundStatement* body);

    BoundNodeKind getKind() const override;
    const BoundExpression* condition() const { return _condition; }
    const BoundStatement* body() const { return _body; }

private:
    const BoundExpression* _condition;
    const BoundStatement* _body;
};

#endif // BOUNDWHILESTATEMENT_H