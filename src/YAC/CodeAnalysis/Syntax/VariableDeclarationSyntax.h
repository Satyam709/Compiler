//
// Created by satya on 17-04-2025.
//

#ifndef VARIABLEDECLARATIONSYNTAX_H
#define VARIABLEDECLARATIONSYNTAX_H

#include "StatementSyntax.h"
#include "Expression.h"

class VariableDeclarationSyntax final : public StatementSyntax {
public:
    VariableDeclarationSyntax(const SyntaxToken& keyword,
                             const SyntaxToken& identifier,
                             const SyntaxToken& equalsToken,
                             ExpressionSyntax& initializer);

    SyntaxKind getKind() const override;
    const std::vector<SyntaxNode*>& getChildren() const override;

    const SyntaxToken& getKeyword() const;
    const SyntaxToken& getIdentifier() const;
    const SyntaxToken& getEqualsToken() const;
    ExpressionSyntax& getInitializer() const;

private:
    VariableDeclarationSyntax() = default;

    SyntaxToken _keyword;
    SyntaxToken _identifier;
    SyntaxToken _equalsToken;
    ExpressionSyntax& _initializer;
    std::vector<SyntaxNode*> _children;
};

#endif // VARIABLEDECLARATIONSYNTAX_H
