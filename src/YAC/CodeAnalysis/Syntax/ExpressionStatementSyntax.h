#ifndef EXPRESSIONSTATEMENTSYNTAX_H
#define EXPRESSIONSTATEMENTSYNTAX_H

#include "StatementSyntax.h"
#include "Expression.h"

class ExpressionStatementSyntax final : public StatementSyntax {
public:
    explicit ExpressionStatementSyntax(ExpressionSyntax& expression);

    [[nodiscard]] SyntaxKind getKind() const override;
    [[nodiscard]] const std::vector<SyntaxNode*>& getChildren() const override;
    [[nodiscard]] ExpressionSyntax& expression() const;

private:
    ExpressionSyntax& _expression;
    std::vector<SyntaxNode*> _children;
};

#endif // EXPRESSIONSTATEMENTSYNTAX_H