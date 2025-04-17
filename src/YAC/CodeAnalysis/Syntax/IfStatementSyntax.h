//
// Created by satya on 17-04-2025.
//

#ifndef IFSTATEMENTSYNTAX_H
#define IFSTATEMENTSYNTAX_H

#include "StatementSyntax.h"
#include "ElseClauseSyntax.h"
#include "Expression.h"

class IfStatementSyntax final : public StatementSyntax {
public:
    IfStatementSyntax(const SyntaxToken& ifKeyword,
                      ExpressionSyntax& condition,
                      StatementSyntax& thenStatement,
                      ElseClauseSyntax* elseClause);

    [[nodiscard]] SyntaxKind getKind() const override;
    [[nodiscard]] const std::vector<SyntaxNode*>& getChildren() const override;

    [[nodiscard]] const SyntaxToken& ifKeyword() const { return _ifKeyword; }
    [[nodiscard]] ExpressionSyntax& condition() const { return _condition; }
    [[nodiscard]] StatementSyntax& thenStatement() const { return _thenStatement; }
    [[nodiscard]] ElseClauseSyntax* elseClause() const { return _elseClause; }

private:
    SyntaxToken _ifKeyword;
    ExpressionSyntax& _condition;
    StatementSyntax& _thenStatement;
    ElseClauseSyntax* _elseClause;
    std::vector<SyntaxNode*> _children;
};

#endif // IFSTATEMENTSYNTAX_H