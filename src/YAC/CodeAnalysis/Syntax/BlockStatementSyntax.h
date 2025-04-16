//
// Created by satya on 16-04-2025.
//

#ifndef BLOCKSTATEMENTSYNTAX_H
#define BLOCKSTATEMENTSYNTAX_H

#include "StatementSyntax.h"
#include <vector>

class BlockStatementSyntax final : public StatementSyntax {
public:
    BlockStatementSyntax(const SyntaxToken& openBraceToken,
                        const std::vector<StatementSyntax*>& statements,
                        const SyntaxToken& closeBraceToken);

    SyntaxKind getKind() const override;
    const std::vector<SyntaxNode*>& getChildren() const override;
    const SyntaxToken& getOpenBraceToken() const;
    const SyntaxToken& getCloseBraceToken() const;
    const std::vector<StatementSyntax*>& statements() const;

private:
    BlockStatementSyntax() = default;
    SyntaxToken _openBraceToken;
    SyntaxToken _closeBraceToken;
    std::vector<StatementSyntax*> _statements;
    std::vector<SyntaxNode*> _children;
};

#endif // BLOCKSTATEMENTSYNTAX_H
