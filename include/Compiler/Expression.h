#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "Compiler/SyntaxTree.h"


// abstract class
class ExpressionSyntax : public SyntaxNode {
};

// sealed classes for diff types of expression
class LiteralExpressionSyntax final : public ExpressionSyntax {
public:
    explicit LiteralExpressionSyntax(const SyntaxToken &token);

    SyntaxKind getKind() const override;

    const std::vector<SyntaxNode *> &getChildren() const override;

    const SyntaxToken &getToken() const;

private:
    LiteralExpressionSyntax();

    SyntaxToken _token;
    std::vector<SyntaxNode *> _children;
};

class BinaryExpressionSyntax final : public ExpressionSyntax {
public:
    BinaryExpressionSyntax(ExpressionSyntax &left, SyntaxToken operator_token, ExpressionSyntax &right);

    SyntaxKind getKind() const override;

    const std::vector<SyntaxNode *> &getChildren() const override;

    ExpressionSyntax &left() const;

    SyntaxToken operator_token() const;

    ExpressionSyntax &right() const;

private:
    BinaryExpressionSyntax();

    ExpressionSyntax &_left;
    SyntaxToken _operator_token;
    ExpressionSyntax &_right;
    std::vector<SyntaxNode *> _children;
};

class ParenthesizedExpressionSyntax final : public ExpressionSyntax {
public:
    ParenthesizedExpressionSyntax(SyntaxToken openParenthesisToken, ExpressionSyntax &expression,
                                  SyntaxToken closeParenthesisToken);

    SyntaxKind getKind() const override;

    const std::vector<SyntaxNode *> &getChildren() const override;

    SyntaxToken openParenthesis() const;

    SyntaxToken closeParenthesis() const;

    ExpressionSyntax &expression() const;

private:
    ParenthesizedExpressionSyntax();

    SyntaxToken _openParenthesisToken;
    SyntaxToken _closeParenthesisToken;
    ExpressionSyntax &_expression;
    std::vector<SyntaxNode *> _children;
};

#endif //EXPRESSION_H
