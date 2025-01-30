//
// Created by satya on 24-01-2025.
//


#include "BoundOperators.h"
#include "Binder.h"



// Constructor 1: Single operand type
BoundBinaryOperator::BoundBinaryOperator(const SyntaxKind syntaxKind, const BoundBinaryOperatorKind kind,
                                         const std::type_info &operandType)
    : BoundBinaryOperator(syntaxKind, kind, operandType, operandType, operandType) {}

// Constructor 2: Operand type and result type
BoundBinaryOperator::BoundBinaryOperator(const SyntaxKind syntaxKind, const BoundBinaryOperatorKind kind,
                                         const std::type_info &operandType, const std::type_info &resultType)
    : BoundBinaryOperator(syntaxKind, kind, operandType, operandType, resultType) {}

// Constructor 3: Full specification
BoundBinaryOperator::BoundBinaryOperator(const SyntaxKind syntaxKind, const BoundBinaryOperatorKind kind,
                                         const std::type_info &leftType, const std::type_info &rightType,
                                         const std::type_info &resultType)
    : syntaxKind(syntaxKind), kind(kind), leftType(leftType), rightType(rightType), resultType(resultType) {}

// Method: Bind operator to SyntaxKind and types
std::shared_ptr<BoundBinaryOperator> BoundBinaryOperator::Bind(const SyntaxKind syntaxKind,
                                                               const std::type_info &leftType,
                                                               const std::type_info &rightType) {
    for (const auto &op : operators) {
        if (op->syntaxKind == syntaxKind && op->leftType == leftType && op->rightType == rightType) {
            return op;
        }
    }
    return nullptr;
}

// Getters
SyntaxKind BoundBinaryOperator::syntax_kind() const {
    return syntaxKind;
}

BoundBinaryOperatorKind BoundBinaryOperator::kind1() const {
    return kind;
}

const std::type_info &BoundBinaryOperator::left_type() const {
    return leftType;
}

const std::type_info &BoundBinaryOperator::right_type() const {
    return rightType;
}

const std::type_info &BoundBinaryOperator::result_type() const {
    return resultType;
}

// Static operator definitions
std::vector<std::shared_ptr<BoundBinaryOperator>> BoundBinaryOperator::operators = {
    std::make_shared<BoundBinaryOperator>(SyntaxKind::PlusToken,
                                          BoundBinaryOperatorKind::Addition, typeid(int)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::MinusToken,
                                          BoundBinaryOperatorKind::Subtraction, typeid(int)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::StarToken,
                                          BoundBinaryOperatorKind::Multiplication, typeid(int)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::SlashToken,
                                          BoundBinaryOperatorKind::Division, typeid(int)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::EqualEqualToken,
                                          BoundBinaryOperatorKind::Equals, typeid(int),
                                          typeid(bool)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::NotEqualToken,
                                          BoundBinaryOperatorKind::NotEquals, typeid(int),
                                          typeid(bool)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::AmpersandAmpersandToken,
                                          BoundBinaryOperatorKind::LogicalAnd, typeid(bool)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::PipePipeToken,
                                          BoundBinaryOperatorKind::LogicalOR, typeid(bool)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::EqualEqualToken,
                                          BoundBinaryOperatorKind::Equals, typeid(bool)),
    std::make_shared<BoundBinaryOperator>(SyntaxKind::NotEqualToken,
                                          BoundBinaryOperatorKind::NotEquals, typeid(bool))
};





// Constructor 1: Single operand type
BoundUnaryOperator::BoundUnaryOperator(const SyntaxKind syntaxKind, const BoundUnaryOperatorKind kind,
                                       const std::type_info &operandType)
    : BoundUnaryOperator(syntaxKind, kind, operandType, operandType) {}

// Constructor 2: Operand type and result type
BoundUnaryOperator::BoundUnaryOperator(const SyntaxKind syntaxKind, const BoundUnaryOperatorKind kind,
                                       const std::type_info &operandType, const std::type_info &resultType)
    : syntaxKind(syntaxKind), kind(kind), operandType(operandType), resultType(resultType) {}

// Method: Bind operator to SyntaxKind and operand type
std::shared_ptr<BoundUnaryOperator> BoundUnaryOperator::Bind(const SyntaxKind syntaxKind,
                                                             const std::type_info &operandType) {
    for (const auto &op : operators) {
        if (op->syntaxKind == syntaxKind && op->operandType == operandType) {
            return op;
        }
    }
    return nullptr;
}

// Getters
SyntaxKind BoundUnaryOperator::syntax_kind() const {
    return syntaxKind;
}

BoundUnaryOperatorKind BoundUnaryOperator::kind1() const {
    return kind;
}

const std::type_info &BoundUnaryOperator::operand_type() const {
    return operandType;
}

const std::type_info &BoundUnaryOperator::result_type() const {
    return resultType;
}

// Static operator definitions
std::vector<std::shared_ptr<BoundUnaryOperator>> BoundUnaryOperator::operators = {
    std::make_shared<BoundUnaryOperator>(SyntaxKind::PlusToken,
                                         BoundUnaryOperatorKind::Identity, typeid(int)),
    std::make_shared<BoundUnaryOperator>(SyntaxKind::MinusToken,
                                         BoundUnaryOperatorKind::Negation, typeid(int)),
    std::make_shared<BoundUnaryOperator>(SyntaxKind::BangToken,
                                         BoundUnaryOperatorKind::LogicalAnd, typeid(bool))
};
