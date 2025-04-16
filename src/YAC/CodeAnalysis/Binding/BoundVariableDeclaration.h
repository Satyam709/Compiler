//
// Created by satya on 17-04-2025.
//

#ifndef BOUNDVARIABLEDECLARATION_H
#define BOUNDVARIABLEDECLARATION_H

#include "BoundStatement.h"

class BoundVariableDeclaration final : public BoundStatement {
public:
    BoundVariableDeclaration(VariableSymbol* variable,const BoundExpression* initializer);

    BoundNodeKind getKind() const override;
    VariableSymbol* getVariable() const;
    const BoundExpression* getInitializer() const;

private:
    VariableSymbol* _variable;
    const BoundExpression* _initializer;
};

#endif // BOUNDVARIABLEDECLARATION_H
