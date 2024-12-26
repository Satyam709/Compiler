#include <vector>
#include "Compiler/Syntax.h"
#include "Compiler/Expression.h"

// sealed classes for diff types of expression

// Number expression
NumberExpressionSyntax::NumberExpressionSyntax(const SyntaxToken &token): _token(token) {
    _children.emplace_back(new SyntaxNodeToken(token));
}

SyntaxKind NumberExpressionSyntax::getKind() const  {
    return NumberExpression;
};

const std::vector<SyntaxNode *> &NumberExpressionSyntax::getChildren() const  {
    return _children;
}

const SyntaxToken &NumberExpressionSyntax::getToken() const {
    return _token;
}

NumberExpressionSyntax::NumberExpressionSyntax() = default;

// Binary expression
BinaryExpressionSyntax::BinaryExpressionSyntax(ExpressionSyntax &left, SyntaxToken operator_token,
                                               ExpressionSyntax &right): _left(left),
                                                                         _right(right),
                                                                         _operator_token(operator_token) {
    _children.push_back(&left);
    _children.push_back(new SyntaxNodeToken(operator_token));
    _children.push_back(&right);
}

SyntaxKind BinaryExpressionSyntax::getKind() const  {
    return BinaryExpression;
};

const std::vector<SyntaxNode *> &BinaryExpressionSyntax::getChildren() const  {
    return _children;
};

ExpressionSyntax &BinaryExpressionSyntax::left() const {
    return _left;
}

SyntaxToken BinaryExpressionSyntax::operator_token() const {
    return _operator_token;
}

ExpressionSyntax &BinaryExpressionSyntax::right() const {
    return _right;
}


ParenthesizedExpressionSyntax::ParenthesizedExpressionSyntax(SyntaxToken openParenthesisToken,
                                                             ExpressionSyntax &expression,
                                                             SyntaxToken closeParenthesisToken): _openParenthesisToken(
        openParenthesisToken),
    _closeParenthesisToken(closeParenthesisToken),
    _expression(expression) {
    _children.push_back(new SyntaxNodeToken(_openParenthesisToken));
    _children.push_back(&expression);
    _children.push_back(new SyntaxNodeToken(_closeParenthesisToken));
}

SyntaxKind ParenthesizedExpressionSyntax::getKind() const {
    return ParenthesizedExpression;
}

const std::vector<SyntaxNode *> &ParenthesizedExpressionSyntax::getChildren() const {
    return _children;
}

SyntaxToken ParenthesizedExpressionSyntax::openParenthesis() const {
    return _openParenthesisToken;
}

SyntaxToken ParenthesizedExpressionSyntax::closeParenthesis() const {
    return _closeParenthesisToken;
}

ExpressionSyntax &ParenthesizedExpressionSyntax::expression() const {
    return _expression;
}
