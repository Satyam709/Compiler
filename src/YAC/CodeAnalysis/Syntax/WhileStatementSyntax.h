//
// Created by satya on 17-04-2025.
//

#ifndef WHILESTATEMENT_H
#define WHILESTATEMENT_H

#include "StatementSyntax.h"
#include "Expression.h"
#include "Syntax.h"

class WhileStatementSyntax final : public StatementSyntax {
public:
    WhileStatementSyntax(const SyntaxToken& whileKeyword,
                        ExpressionSyntax& condition,
                        StatementSyntax& body);

    SyntaxKind getKind() const override;
    const std::vector<SyntaxNode*>& getChildren() const override;

    const SyntaxToken& whileKeyword() const { return _whileKeyword; }
    ExpressionSyntax& condition() const { return _condition; }
    StatementSyntax& body() const { return _body; }

private:
    SyntaxToken _whileKeyword;
    ExpressionSyntax& _condition;
    StatementSyntax& _body;
    std::vector<SyntaxNode*> _children;
};

#endif // WHILESTATEMENT_H