//
// Created by satya on 17-04-2025.
//

#ifndef ELSECLAUSESYNTAX_H
#define ELSECLAUSESYNTAX_H

#include "SyntaxTree.h"
#include "StatementSyntax.h"

class ElseClauseSyntax final : public SyntaxNode {
public:
    ElseClauseSyntax(const SyntaxToken& elseKeyword, StatementSyntax& elseStatement);

    [[nodiscard]] SyntaxKind getKind() const override;
    [[nodiscard]] const std::vector<SyntaxNode*>& getChildren() const override;

    [[nodiscard]] const SyntaxToken& elseKeyword() const { return _elseKeyword; }
    [[nodiscard]] StatementSyntax& elseStatement() const { return _elseStatement; }

private:
    SyntaxToken _elseKeyword;
    StatementSyntax& _elseStatement;
    std::vector<SyntaxNode*> _children;
};

#endif // ELSECLAUSESYNTAX_H