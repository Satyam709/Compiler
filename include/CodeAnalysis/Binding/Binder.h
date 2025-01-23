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

    Invalid,
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

    [[nodiscard]] const BoundExpression& left() const {
        return _left;
    }

    [[nodiscard]] const BoundExpression& right() const {
        return _right;
    }

    [[nodiscard]] BoundBinaryOperatorKind operator_() const {
        return _operator;
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
    const BoundExpression *getOperand() const { return &_operand; }
    BoundUnaryOperatorKind getOperatorKind() const { return _kind; }

private:
    BoundUnaryOperatorKind _kind;
    const BoundExpression &_operand;
};


class Binder {
private:
     std::vector<std::string> _diagnostics;

public:
    [[nodiscard]]  const std::vector<std::string> &diagnostics() {
        return _diagnostics;
    }

     const BoundExpression *BindLiteralExpression(const ExpressionSyntax &syntax) {
        if (const auto *exp = dynamic_cast<const LiteralExpressionSyntax *>(&syntax)) {
            try {
                std::any val = 0;
                val = exp->getToken().val;
                return new BoundLiteralExpression(val);
            } catch (const std::bad_any_cast &e) {

                std::cerr << "Cant bind to literal exp!!: " << std::endl;
                std::cerr << "Error: Failed to cast to int. Reason: " << e.what() << std::endl;
                return new BoundLiteralExpression(0);
            }
        }
        throw std::runtime_error("Failed to bind literal expression");
    }

    // binds the unary operator
     BoundUnaryOperatorKind bindUnaryOperatorKind(const SyntaxKind kind, const std::type_info &operandType) {
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


     const BoundExpression *BindUnaryExpression(const ExpressionSyntax &syntax) {
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

     BoundBinaryOperatorKind bindBinaryOperatorKind(const SyntaxKind kind, const std::type_info &leftType,
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

     const BoundExpression *BindBinaryExpression(const ExpressionSyntax &syntax) {
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

    const  BoundExpression *bindExpression(const ExpressionSyntax &syntax) {
        switch (syntax.getKind()) {
            case SyntaxKind::LiteralExpression: return BindLiteralExpression(syntax);
            case SyntaxKind::UnaryExpression: return BindUnaryExpression(syntax);
            case SyntaxKind::BinaryExpression: return BindBinaryExpression(syntax);
            default: throw std::runtime_error("Unknown syntax kind");
        }
    }
};

inline std::string boundKindsToString(const BoundUnaryOperatorKind kind) {
    switch (kind) {
        case BoundUnaryOperatorKind::Invalid:
            return "Invalid";
        case BoundUnaryOperatorKind::Identity:
            return "Identity";
        case BoundUnaryOperatorKind::Negation:
            return "Negation";
    }
    return "Not Found";
}

inline std::string boundKindsToString(const BoundBinaryOperatorKind kind) {
    switch (kind) {
        case BoundBinaryOperatorKind::Addition:
            return "Addition";
        case BoundBinaryOperatorKind::Subtraction:
            return "Subtraction";
        case BoundBinaryOperatorKind::Multiplication:
            return "Multiplication";
        case BoundBinaryOperatorKind::Division:
            return "Division";
        case BoundBinaryOperatorKind::Invalid:
            return "Invalid";
    }
    return "Not Found";
}

inline std::string boundKindsToString(const BoundNodeKind kind) {
    switch (kind) {
        case BoundNodeKind::BinaryExpression:
            return "BinaryExpression";
        case BoundNodeKind::UnaryExpression:
            return "UnaryExpression";
        case BoundNodeKind::LiteralExpression:
            return "LiteralExpression";
    }
    return "Not Found";
}


#endif //BINDER_H
