//
// Created by satya on 15-04-2025.
//

#ifndef COMPILATIONUNIT_H
#define COMPILATIONUNIT_H
#include <vector>

#include "Syntax.h"
#include "SyntaxTree.h"

class StatementSyntax;
class SyntaxToken;

class CompilationUnitSyntax final : public SyntaxNode {
public:
    CompilationUnitSyntax(StatementSyntax &root, SyntaxToken endOfFileToken);

    [[nodiscard]] SyntaxKind getKind() const override;

    [[nodiscard]] const std::vector<SyntaxNode *> & getChildren() const override;

    [[nodiscard]] StatementSyntax& statement() const {
        return _statement;
    }

    [[nodiscard]] SyntaxToken eof() const {
        return _eof;
    }

private:
    StatementSyntax &_statement;
    SyntaxToken _eof;
};


#endif //COMPILATIONUNIT_H
