//
// Created by satya on 17-04-2025.
//

#ifndef FORSTATEMENT_H
#define FORSTATEMENT_H

#include "StatementSyntax.h"
#include "Expression.h"
#include "Syntax.h"

class ForStatementSyntax final : public StatementSyntax {
public:
    ForStatementSyntax(const SyntaxToken &keyword,
                       const SyntaxToken &identifier,
                       const SyntaxToken &equalsToken,
                       ExpressionSyntax &lowerBound,
                       const SyntaxToken &toKeyword,
                       ExpressionSyntax &upperBound,
                       StatementSyntax &body);

    SyntaxKind getKind() const override;

    const std::vector<SyntaxNode *> &getChildren() const override;

    const SyntaxToken &keyword() const { return _keyword; }
    const SyntaxToken &identifier() const { return _identifier; }
    const SyntaxToken &equalsToken() const { return _equalsToken; }
    ExpressionSyntax &lowerBound() const { return _lowerBound; }
    const SyntaxToken &toKeyword() const { return _toKeyword; }
    ExpressionSyntax &upperBound() const { return _upperBound; }
    StatementSyntax &body() const { return _body; }

private:
    SyntaxToken _keyword;
    SyntaxToken _identifier;
    SyntaxToken _equalsToken;
    ExpressionSyntax &_lowerBound;
    SyntaxToken _toKeyword;
    ExpressionSyntax &_upperBound;
    StatementSyntax &_body;
    std::vector<SyntaxNode *> _children;
};

#endif // FORSTATEMENT_H
