#include <vector>
#include "Syntax.h"
#include "Expression.h"

// sealed classes for diff types of expression

// Number expression
LiteralExpressionSyntax::LiteralExpressionSyntax(const SyntaxToken &token): _token(token) {
    _children.push_back(new SyntaxNodeToken(token));
}

LiteralExpressionSyntax::LiteralExpressionSyntax(const SyntaxToken &token,const std::any& value): _token(token) {
    _token.val = value;
    _children.push_back(new SyntaxNodeToken(token));
}

SyntaxKind LiteralExpressionSyntax::getKind() const {
    return SyntaxKind::LiteralExpression;
};

const std::vector<SyntaxNode *> &LiteralExpressionSyntax::getChildren() const {
    return _children;
}

const SyntaxToken &LiteralExpressionSyntax::getToken() const {
    return _token;
}

LiteralExpressionSyntax::LiteralExpressionSyntax() = default;

// Binary expression
BinaryExpressionSyntax::BinaryExpressionSyntax(ExpressionSyntax &left, SyntaxToken operator_token,
                                               ExpressionSyntax &right): _left(left),
                                                                         _right(right),
                                                                         _operator_token(operator_token) {
    _children.push_back(&left);
    _children.push_back(new SyntaxNodeToken(operator_token));
    _children.push_back(&right);
}

SyntaxKind BinaryExpressionSyntax::getKind() const {
    return SyntaxKind::BinaryExpression;
};

const std::vector<SyntaxNode *> &BinaryExpressionSyntax::getChildren() const {
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

// Parenthesized expression
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
    return SyntaxKind::ParenthesizedExpression;
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

// Unary expression
UnaryExpressionSyntax::UnaryExpressionSyntax(const SyntaxToken &operatorToken,
                                             ExpressionSyntax &operand) : _token(operatorToken), _operand(operand) {
    _children.push_back(new SyntaxNodeToken(operatorToken));
    _children.push_back(&_operand);
}

SyntaxKind UnaryExpressionSyntax::getKind() const {
    return SyntaxKind::UnaryExpression;
}

const std::vector<SyntaxNode *> &UnaryExpressionSyntax::getChildren() const {
    return _children;
}

SyntaxToken UnaryExpressionSyntax::operatorToken() const {
    return _token;
}

ExpressionSyntax *UnaryExpressionSyntax::operand() const {
    return &_operand;
}

// NameExpressionSyntax Implementation
NameExpressionSyntax::NameExpressionSyntax(const SyntaxToken& identifierToken)
    : _identifierToken(identifierToken) {
    _children.push_back(new SyntaxNodeToken(identifierToken));
}

SyntaxKind NameExpressionSyntax::getKind() const {
    return SyntaxKind::NameExpression;
}

const std::vector<SyntaxNode*>& NameExpressionSyntax::getChildren() const {
    return _children;
}

const SyntaxToken& NameExpressionSyntax::getIdentifierToken() const {
    return _identifierToken;
}

// AssignmentExpressionSyntax Implementation
AssignmentExpressionSyntax::AssignmentExpressionSyntax(const SyntaxToken& identifierToken,
                                                      const SyntaxToken& equalsToken,
                                                      ExpressionSyntax& expression)
    : _identifierToken(identifierToken)
    , _equalsToken(equalsToken)
    , _expression(expression) {
    _children.push_back(new SyntaxNodeToken(identifierToken));
    _children.push_back(new SyntaxNodeToken(equalsToken));
    _children.push_back(&expression);
}

SyntaxKind AssignmentExpressionSyntax::getKind() const {
    return SyntaxKind::AssignmentExpression;
}

const std::vector<SyntaxNode*>& AssignmentExpressionSyntax::getChildren() const {
    return _children;
}

const SyntaxToken& AssignmentExpressionSyntax::getIdentifierToken() const {
    return _identifierToken;
}

const SyntaxToken& AssignmentExpressionSyntax::getEqualsToken() const {
    return _equalsToken;
}

ExpressionSyntax& AssignmentExpressionSyntax::expression() const {
    return _expression;
}