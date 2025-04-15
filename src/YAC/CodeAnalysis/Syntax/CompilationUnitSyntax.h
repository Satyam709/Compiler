//
// Created by satya on 15-04-2025.
//

#ifndef COMPILATIONUNIT_H
#define COMPILATIONUNIT_H
#include <vector>

#include "Syntax.h"
#include "SyntaxTree.h"

class SyntaxToken;
class ExpressionSyntax;

class CompilationUnitSyntax final : public SyntaxNode {
public:
    CompilationUnitSyntax(ExpressionSyntax &root, SyntaxToken endOfFileToken);

    [[nodiscard]] SyntaxKind getKind() const override;

    [[nodiscard]] const std::vector<SyntaxNode *> & getChildren() const override;

    [[nodiscard]] ExpressionSyntax& exp() const {
        return _exp;
    }

    [[nodiscard]] SyntaxToken eof() const {
        return _eof;
    }

private:
    ExpressionSyntax &_exp;
    SyntaxToken _eof;
};


#endif //COMPILATIONUNIT_H
