#ifndef BINDER_H
#define BINDER_H
#include <any>
#include <iostream>
#include <sstream>
#include <utility>

#include "BoundOperators.h"
#include "CodeAnalysis/Syntax/Expression.h"
#include "CodeAnalysis/Syntax/Syntax.h"
#include "CodeAnalysis/Utils/Caster.h"

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
    LogicalAnd,
};

enum class BoundBinaryOperatorKind {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Invalid,
    LogicalAnd,
    LogicalOR,
    Equals,
    NotEquals
};

#include <vector>
#include <typeinfo>
#include <memory>




class BoundBinaryExpression : public BoundExpression {
public:
    BoundBinaryExpression(const BoundExpression &left, const BoundBinaryOperator op,
                          const BoundExpression &right): _left(left),
                                                         _right(right), _operator(op) {
    };

    BoundNodeKind getKind() const override {
        return BoundNodeKind::BinaryExpression;
    }

    [[nodiscard]] const BoundExpression &left() const {
        return _left;
    }

    [[nodiscard]] const BoundExpression &right() const {
        return _right;
    }

    [[nodiscard]] BoundBinaryOperatorKind operator_() const {
        return _operator.kind1();
    }

    // currently just returning type of left exp !!!!!!
    //........
    const std::type_info &getType() const override {
        return _operator.result_type();
    }

    BoundBinaryOperator getOperator() const {
        return _operator;
    }

private:
    const BoundExpression &_left;
    const BoundExpression &_right;
    BoundBinaryOperator _operator;
};

// class BoundUnaryOperator {
// public:
//     BoundUnaryOperator(SyntaxKind syntaxKind, BoundUnaryOperatorKind kind, const std::type_info &operandType)
//         : BoundUnaryOperator(syntaxKind, kind, operandType, operandType) {
//     }
//
//     BoundUnaryOperator(SyntaxKind syntaxKind, BoundUnaryOperatorKind kind, const std::type_info &operandType,
//                        const std::type_info &resultType)
//         : syntaxKind(syntaxKind), kind(kind), operandType(operandType), resultType(resultType) {
//     }
//
//     static std::shared_ptr<BoundUnaryOperator> Bind(SyntaxKind syntaxKind, const std::type_info &operandType) {
//         for (const auto &op: unaryOperators) {
//             if (op->syntaxKind == syntaxKind && op->operandType == operandType) {
//                 return op;
//             }
//         }
//         return nullptr;
//     }
//
// private:
//     SyntaxKind syntaxKind;
//
// public:
//     [[nodiscard]] SyntaxKind syntax_kind() const {
//         return syntaxKind;
//     }
//
//     [[nodiscard]] BoundUnaryOperatorKind kind1() const {
//         return kind;
//     }
//
//     [[nodiscard]] const std::type_info &operand_type() const {
//         return operandType;
//     }
//
//     [[nodiscard]] const std::type_info &result_type() const {
//         return resultType;
//     }
//
// private:
//     BoundUnaryOperatorKind kind;
//     const std::type_info &operandType;
//     const std::type_info &resultType;
//
//     static inline  const std::vector<std::shared_ptr<BoundUnaryOperator> > unaryOperators;
// };
//
// // Initialize the operators vector using the existing SyntaxKind and BoundUnaryOperatorKind enums
// inline const std::vector<std::shared_ptr<BoundUnaryOperator> > BoundUnaryOperator::unaryOperators = {
//     std::make_shared<BoundUnaryOperator>(SyntaxKind::BangToken, BoundUnaryOperatorKind::LogicalAnd, typeid(bool)),
//     std::make_shared<BoundUnaryOperator>(SyntaxKind::PlusToken, BoundUnaryOperatorKind::Identity, typeid(int)),
//     std::make_shared<BoundUnaryOperator>(SyntaxKind::MinusToken, BoundUnaryOperatorKind::Negation, typeid(int))
// };


class BoundUnaryExpression : public BoundExpression {
public:
    BoundUnaryExpression(const BoundUnaryOperator op, const BoundExpression &operand): _op(op),
        _operand(operand) {
    };
    BoundNodeKind getKind() const override { return BoundNodeKind::UnaryExpression; }
    const std::type_info &getType() const override { return _op.result_type(); } // added type
    const BoundExpression *getOperand() const { return &_operand; }
    BoundUnaryOperatorKind getOperatorKind() const { return _op.kind1(); }

private:
    BoundUnaryOperator _op;
    const BoundExpression &_operand;
};


class Binder {
private:
    std::vector<std::string> _diagnostics;

public:
    [[nodiscard]] const std::vector<std::string> &diagnostics() {
        return _diagnostics;
    }

    const BoundExpression *BindLiteralExpression(const ExpressionSyntax &syntax) {
        if (const auto *exp = dynamic_cast<const LiteralExpressionSyntax *>(&syntax)) {
            try {
                std::any val = 0;
                val = exp->getToken().val; // nulls ?

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
    // BoundUnaryOperatorKind bindUnaryOperatorKind(const SyntaxKind kind, const std::type_info &operandType) {
    //     if (operandType == typeid(int)) {
    //         switch (kind) {
    //             case SyntaxKind::PlusToken:
    //                 return BoundUnaryOperatorKind::Identity;
    //             case SyntaxKind::MinusToken:
    //                 return BoundUnaryOperatorKind::Negation;
    //         }
    //     }
    //
    //     if (operandType == typeid(bool)) {
    //         switch (kind) {
    //             case SyntaxKind::BangToken:
    //                 return BoundUnaryOperatorKind::LogicalAnd;
    //         }
    //     }
    //
    //
    //     return BoundUnaryOperatorKind::Invalid;
    // }


    const BoundExpression *BindUnaryExpression(const ExpressionSyntax &syntax) {
        if (const auto *exp = dynamic_cast<const UnaryExpressionSyntax *>(&syntax)) {
            const auto boundOperand = bindExpression(*(exp->operand()));
            const auto boundOperator = BoundUnaryOperator::Bind(exp->operatorToken().kind, boundOperand->getType());
            if (boundOperator == nullptr) {
                std::ostringstream os;
                os << "Unary operator: " << syntaxKindToString(exp->operatorToken().kind)
                        << " is not defined for type " << getTypeName(boundOperand->getType());
                _diagnostics.push_back(os.str());

                // for avoiding cascading type error or null returns
                return boundOperand;
            }
            return new BoundUnaryExpression(*boundOperator, *boundOperand);
        }
        throw std::invalid_argument("Invalid expression: expression failed to bound");
    }

    // BoundBinaryOperatorKind bindBinaryOperatorKind(const SyntaxKind kind, const std::type_info &leftType,
    //                                                const std::type_info &rightType) {
    //     if (leftType == typeid(int) && rightType == typeid(int)) {
    //         switch (kind) {
    //             case SyntaxKind::PlusToken:
    //                 return BoundBinaryOperatorKind::Addition;
    //             case SyntaxKind::MinusToken:
    //                 return BoundBinaryOperatorKind::Subtraction;
    //             case SyntaxKind::StarToken:
    //                 return BoundBinaryOperatorKind::Multiplication;
    //             case SyntaxKind::SlashToken:
    //                 return BoundBinaryOperatorKind::Division;
    //         }
    //     }
    //
    //     if (leftType == typeid(bool) && rightType == typeid(bool)) {
    //         switch (kind) {
    //             case SyntaxKind::AmpersandAmpersandToken:
    //                 return BoundBinaryOperatorKind::LogicalAnd;
    //             case SyntaxKind::PipePipeToken:
    //                 return BoundBinaryOperatorKind::LogicalOR;
    //         }
    //     }
    //
    //     return BoundBinaryOperatorKind::Invalid;
    // };

    const BoundExpression *BindBinaryExpression(const ExpressionSyntax &syntax) {
        if (const auto *exp = dynamic_cast<const BinaryExpressionSyntax *>(&syntax)) {
            const auto left = bindExpression(exp->left());
            const auto right = bindExpression(exp->right());
            const auto op = BoundBinaryOperator::Bind(exp->operator_token().kind, left->getType(),
                                                      right->getType());

            if (op == nullptr) {
                std::ostringstream os;
                os << "Binary operator: " << syntaxKindToString(exp->operator_token().kind)
                        << " is not defined for type " << getTypeName(left->getType()) << " and " << getTypeName(
                            right->getType());
                _diagnostics.push_back(os.str());

                // for avoiding cascading type error or null returns
                return left;
            }

            return new BoundBinaryExpression(*left, *op, *right);
        }
        throw std::invalid_argument("Invalid expression: expression failed to bound");
    }

    const BoundExpression *bindExpression(const ExpressionSyntax &syntax) {
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
