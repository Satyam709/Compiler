#ifndef BINDER_H
#define BINDER_H
#include <any>
#include <iostream>
#include <sstream>
#include <utility>

#include "CodeAnalysis/Syntax/Expression.h"
#include "CodeAnalysis/Syntax/Syntax.h"

enum class BoundNodeKind {
    UnaryExpression,
    LiteralExpression,
    BinaryExpression
};

class BoundNode {
public:
    virtual BoundNodeKind getKind() const = 0;
};

class BoundExpression : public BoundNode {
public:
    // add typessss
    virtual const std::type_info &getType() const = 0;
};


class BoundLiteralExpression : public BoundExpression {
public:
    explicit BoundLiteralExpression(std::any value): _value(std::move(value)) {
    }

    BoundNodeKind getKind() const override {
        return BoundNodeKind::LiteralExpression;
    }

    // fix
    const std::type_info &getType() const override {
        return _value.type();
    }

    const std::any &getValue() const {
        return _value;
    }

private:
    std::any _value;
};

enum class BoundUnaryOperatorKind {
    Identity,
    Negation,

    Invalid
};

enum class BoundBinaryOperatorKind {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Invalid,
};

class BoundBinaryExpression : public BoundExpression {
public:
    BoundBinaryExpression(const BoundExpression &left, const BoundBinaryOperatorKind operatorKind,
                          const BoundExpression &right): _left(left),
                                                         _right(right), _operator(operatorKind) {
    };

    BoundNodeKind getKind() const override {
        return BoundNodeKind::BinaryExpression;
    }

    // currently just returning type of left exp !!!!!!
    //........
    const std::type_info &getType() const override {
        return _left.getType();
    }

    BoundBinaryOperatorKind getOperator() const {
        return _operator;
    }

private:
    const BoundExpression &_left;
    const BoundExpression &_right;
    BoundBinaryOperatorKind _operator;
};

class BoundUnaryExpression : public BoundExpression {
public:
    BoundUnaryExpression(const BoundUnaryOperatorKind kind, const BoundExpression &operand): _kind(kind),
        _operand(operand) {
    };
    BoundNodeKind getKind() const override { return BoundNodeKind::UnaryExpression; }
    const std::type_info &getType() const override { return _operand.getType(); } // added type
    const BoundExpression *getOperand() const { return _operand; }
    BoundUnaryOperatorKind getOperatorKind() const { return _kind; }

private:
    BoundUnaryOperatorKind _kind;
    const BoundExpression &_operand;
};


class Binder {
private:
    static std::vector<std::string> _diagnostics;

public:
    [[nodiscard]] static const std::vector<std::string> &diagnostics() {
        return _diagnostics;
    }

    static const BoundExpression *BindLiteralExpression(const ExpressionSyntax &syntax) {
        if (const auto *exp = dynamic_cast<const LiteralExpressionSyntax *>(&syntax)) {
            try {
                auto val = 0;
                val = std::any_cast<int>(exp->getToken().val);
                return new BoundLiteralExpression(val);
            } catch (const std::bad_any_cast &e) {
                std::cerr << "Cant bind to literal exp!!: " << std::endl;
                std::cerr << "Error: Failed to cast to int. Reason: " << e.what() << std::endl;
            }
        }
        throw std::runtime_error("Failed to bind literal expression");
    }

    // binds the unary operator
    static BoundUnaryOperatorKind bindUnaryOperatorKind(const SyntaxKind kind, const std::type_info &operandType) {
        if (operandType != typeid(int)) {
            return BoundUnaryOperatorKind::Invalid;
        }
        switch (kind) {
            case SyntaxKind::PlusToken:
                return BoundUnaryOperatorKind::Identity;
            case SyntaxKind::MinusToken:
                return BoundUnaryOperatorKind::Negation;
            default:
                throw std::runtime_error("Unknown syntax kind");
        }
    }


    static const BoundExpression *BindUnaryExpression(const ExpressionSyntax &syntax) {
        if (const auto *exp = dynamic_cast<const UnaryExpressionSyntax *>(&syntax)) {
            const auto boundOperand = bindExpression(*(exp->operand()));
            const BoundUnaryOperatorKind boundOperator = bindUnaryOperatorKind(
                exp->operatorToken().kind, boundOperand->getType());
            if (boundOperator == BoundUnaryOperatorKind::Invalid) {
                std::ostringstream os;
                os << "Unary operator: " << syntaxKindToString(exp->operatorToken().kind)
                        << " is not defined for type " << boundOperand->getType().name();
                _diagnostics.push_back(os.str());

                // for avoiding cascading type error or null returns
                return boundOperand;
            }
            return new BoundUnaryExpression(boundOperator, *boundOperand);
        }
        throw std::invalid_argument("Invalid expression: expression failed to bound");
    }

    static BoundBinaryOperatorKind bindBinaryOperatorKind(const SyntaxKind kind, const std::type_info &leftType,
                                                          const std::type_info &rightType) {
        if (leftType != typeid(int) || rightType != typeid(int)) {
            return BoundBinaryOperatorKind::Invalid;
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
                throw std::invalid_argument("Unknown syntax kind " + syntaxKindToString(kind));
        }
    };

    static const BoundExpression *BindBinaryExpression(const ExpressionSyntax &syntax) {
        if (const auto *exp = dynamic_cast<const BinaryExpressionSyntax *>(&syntax)) {
            const auto left = bindExpression(exp->left());
            const auto right = bindExpression(exp->right());
            const BoundBinaryOperatorKind op = bindBinaryOperatorKind(exp->operator_token().kind, left->getType(),
                                                                      right->getType());

            if (op == BoundBinaryOperatorKind::Invalid) {
                std::ostringstream os;
                os << "Binary operator: " << syntaxKindToString(exp->operator_token().kind)
                        << " is not defined for type " << left->getType().name()<<" and "<< right->getType().name();
                _diagnostics.push_back(os.str());

                // for avoiding cascading type error or null returns
                return left;
            }

            return new BoundBinaryExpression(*left, op, *right);
        }
        throw std::invalid_argument("Invalid expression: expression failed to bound");
    }

    const static BoundExpression *bindExpression(const ExpressionSyntax &syntax) {
        switch (syntax.getKind()) {
            case SyntaxKind::LiteralExpression: return BindLiteralExpression(syntax);
            case SyntaxKind::UnaryExpression: return BindUnaryExpression(syntax);
            case SyntaxKind::BinaryExpression: return BindBinaryExpression(syntax);
            default: throw std::runtime_error("Unknown syntax kind");
        }
    }
};


#endif //BINDER_H
