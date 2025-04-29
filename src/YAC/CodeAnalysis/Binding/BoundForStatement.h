//
// Created by satya on 17-04-2025.
//

#ifndef BOUNDFORSTATEMENT_H
#define BOUNDFORSTATEMENT_H

#include "BoundStatement.h"
#include "YAC/CodeAnalysis/Symbols/VariableSymbol.h"

class BoundForStatement final : public BoundStatement {
public:
    BoundForStatement(const VariableSymbol& variable,
                     const BoundExpression* lowerBound,
                     const BoundExpression* upperBound,
                     BoundStatement* body);

    BoundNodeKind getKind() const override;

    const VariableSymbol& variable() const { return _variable; }
    const BoundExpression* lowerBound() const { return _lowerBound; }
    const BoundExpression* upperBound() const { return _upperBound; }
    BoundStatement* body() const { return _body; }

    std::vector<const BoundNode*> getChildren() const override;
    std::vector<std::pair<std::string, std::string>> getProperties() const override;

private:
    VariableSymbol _variable;
    const BoundExpression* _lowerBound;
    const BoundExpression* _upperBound;
    BoundStatement* _body;
};

#endif // BOUNDFORSTATEMENT_H