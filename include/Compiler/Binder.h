#ifndef BINDER_H
#define BINDER_H

#include <iostream>
#include <string>
#include <typeinfo>
#include <optional>
#include <stdexcept>
#include "SyntaxTree.h"
#include "Syntax.h"
#include "Expression.h"

enum class BoundNodeKind {
    // BoundNodeKind for BoundBinaryExpression
    BinaryExpression,
    // BoundNodeKind for BoundLiteralExpression
    LiteralExpression,
    // BoundNodeKind for BoundUnaryExpression
    UnaryExpression
};

class BoundNode {
public:
    // getter for the NodeKind
    virtual BoundNodeKind Kind() = 0;
};

class BoundExpression : public BoundNode {
private:
    const std::string _type;
public:
    // getter for the Type
    virtual const std::string& Type() = 0;
};

enum class BoundUnaryOperatorKind {
    Identity,
    Negation
};

template<typename T>
class BoundLiteralExpression final : public BoundExpression {
public:
    T _value;
    const std::string _type;

public:
    BoundLiteralExpression(T value) : _value(value), _type(typeid(T).name()) {}

    BoundNodeKind Kind() override {
        return BoundNodeKind::LiteralExpression;
    }

    const std::string& Type() override {
        return _type;
    }
};

class BoundUnaryExpression final : public BoundExpression {
private:
    const std::string _type;
public:
    BoundUnaryOperatorKind _op;
    BoundExpression* _operand;

public:
    BoundUnaryExpression(BoundUnaryOperatorKind op, BoundExpression* operand) 
        : _op(op), _operand(operand), _type(_operand->Type()) {}

    BoundNodeKind Kind() override {
        return BoundNodeKind::UnaryExpression;
    }

    const std::string& Type() override {
        return _operand->Type();
    }
};

enum class BoundBinaryOperatorKind {
    Addition,
    Subtraction,
    Multiplication,
    Division,
};

class BoundBinaryExpression final : public BoundExpression {
public:
    BoundExpression* _left;
    BoundExpression* _right;
    BoundBinaryOperatorKind _op;

public:
    BoundBinaryExpression(BoundExpression* left, BoundBinaryOperatorKind op, BoundExpression* right)
        : _left(left), _right(right), _op(op) {}

    BoundNodeKind Kind() override {
        return BoundNodeKind::BinaryExpression;
    }

    const std::string& Type() override {
        return _left->Type();
    }
};

class Binder {
private:
    std::vector<std::string_view> _diagnostics;
public:

    std::vector<std::string_view> Diagnostics() {
        return _diagnostics;
    }

    BoundExpression* BindExpression(ExpressionSyntax* syntax) {
        switch (syntax->getKind()) {
            case SyntaxKind::LiteralExpression:
                return BindLiteralExpression(dynamic_cast<LiteralExpressionSyntax*>(syntax));
            case SyntaxKind::UnaryExpression:
                return BindUnaryExpression(dynamic_cast<UnaryExpressionSyntax*>(syntax));
            case SyntaxKind::BinaryExpression:
                return BindBinaryExpression(dynamic_cast<BinaryExpressionSyntax*>(syntax));
            default:
                throw std::runtime_error("Unexpected syntax node : " + std::to_string(static_cast<int>(syntax->getKind())));
        }
    }

    BoundExpression* BindLiteralExpression(LiteralExpressionSyntax* syntax) {
        // Assuming we are casting value to int from the token
        auto value = std::any_cast<int>(syntax->getToken().val);
        return new BoundLiteralExpression<int>(value);
    }

    BoundExpression* BindUnaryExpression(UnaryExpressionSyntax* syntax) {
        auto BoundOperand = BindExpression(&syntax->operand());
        auto BoundOperatorKind = BindUnaryOperatorKind(syntax->operatorToken().kind, BoundOperand->Type());
        if(BoundOperatorKind == std::nullopt) {
            std::string err = "Unary operator ";
            err += syntax->operatorToken().text;
            err += " is not defined for type ";
            err += BoundOperand->Type();
            _diagnostics.push_back(err);
            return BoundOperand;
        }
        return new BoundUnaryExpression(BoundOperatorKind.value(), BoundOperand);
    }

    BoundExpression* BindBinaryExpression(BinaryExpressionSyntax* syntax) {
        auto BoundLeft = BindExpression(&syntax->left());
        auto BoundRight = BindExpression(&syntax->right());
        auto BoundOperatorKind = BindBinaryOperatorKind(syntax->operatorToken().kind, BoundLeft->Type(), BoundRight->Type());
        return new BoundBinaryExpression(BoundLeft, BoundOperatorKind.value(), BoundRight);
    }

private:
    // BindUnaryOperatorKind should return BoundUnaryOperatorKind or std::nullopt
    std::optional<BoundUnaryOperatorKind> BindUnaryOperatorKind(SyntaxKind kind, const std::string& operandType) {
        if (operandType != typeid(int).name()) {
            return std::nullopt;
        }

        switch (kind) {
            case SyntaxKind::PlusToken:
                return BoundUnaryOperatorKind::Identity;
            case SyntaxKind::MinusToken:
                return BoundUnaryOperatorKind::Negation;
            default:
                throw std::runtime_error("Unexpected unary operator : " + std::to_string(static_cast<int>(kind)));
        }
    }

    // BindBinaryOperatorKind should return BoundBinaryOperatorKind or std::nullopt
    std::optional<BoundBinaryOperatorKind> BindBinaryOperatorKind(SyntaxKind kind, const std::string& leftType, const std::string& rightType) {
        if (leftType != typeid(int).name() || rightType != typeid(int).name()) {
            return std::nullopt;
        }

        switch (kind) {
            case SyntaxKind::PlusToken:
                return BoundBinaryOperatorKind::Addition;
            case SyntaxKind::MinusToken:
                return BoundBinaryOperatorKind::Subtraction;
            case SyntaxKind::StarToken:
                return BoundBinaryOperatorKind::Multiplication;
            case SyntaxKind::SlashToken:
                return BoundBinaryOperatorKind::Division;
            default:
                throw std::runtime_error("Unexpected binary operator : " + std::to_string(static_cast<int>(kind)));
        }
    }
};

#endif // BINDER_H
