#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "SyntaxTree.h"


// abstract class
class ExpressionSyntax : public SyntaxNode {
};

// sealed classes for diff types of expression
class LiteralExpressionSyntax final : public ExpressionSyntax {
public:
    explicit LiteralExpressionSyntax(const SyntaxToken &token);
    explicit LiteralExpressionSyntax(const SyntaxToken &token,const std::any &value);
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

class UnaryExpressionSyntax final : public ExpressionSyntax {
public:
    explicit UnaryExpressionSyntax(const SyntaxToken &operatorToken, ExpressionSyntax& operand);

    SyntaxKind getKind() const override;

    const std::vector<SyntaxNode *> &getChildren() const override;

    ExpressionSyntax *operand() const;

    SyntaxToken operatorToken() const;

private:
    UnaryExpressionSyntax() = default;

    SyntaxToken _token;
    ExpressionSyntax& _operand;
    std::vector<SyntaxNode *> _children;
};

class NameExpressionSyntax final : public ExpressionSyntax {
public:
    explicit NameExpressionSyntax(const SyntaxToken& identifierToken);

    SyntaxKind getKind() const override;
    const std::vector<SyntaxNode*>& getChildren() const override;
    const SyntaxToken& getIdentifierToken() const;

private:
    NameExpressionSyntax() = default;

    SyntaxToken _identifierToken;
    std::vector<SyntaxNode*> _children;
};

class AssignmentExpressionSyntax final : public ExpressionSyntax {
public:
    AssignmentExpressionSyntax(const SyntaxToken& identifierToken,
                              const SyntaxToken& equalsToken,
                              ExpressionSyntax& expression);

    SyntaxKind getKind() const override;
    const std::vector<SyntaxNode*>& getChildren() const override;
    const SyntaxToken& getIdentifierToken() const;
    const SyntaxToken& getEqualsToken() const;
    ExpressionSyntax& expression() const;

private:
    AssignmentExpressionSyntax() = default;

    SyntaxToken _identifierToken;
    SyntaxToken _equalsToken;
    ExpressionSyntax& _expression;
    std::vector<SyntaxNode*> _children;
};

#endif //EXPRESSION_H
