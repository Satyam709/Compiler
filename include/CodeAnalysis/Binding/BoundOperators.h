//
// Created by satya on 24-01-2025.
//

#ifndef BOUNDOPERATORS_H
#define BOUNDOPERATORS_H
#include <typeinfo>
#include <memory>
#include <vector>
#include "CodeAnalysis/Syntax/Syntax.h"

enum class BoundUnaryOperatorKind;
enum class BoundBinaryOperatorKind;

class BoundBinaryOperator {
public:
    BoundBinaryOperator(const SyntaxKind syntaxKind, const BoundBinaryOperatorKind kind,
                        const std::type_info &operandType);

    BoundBinaryOperator(const SyntaxKind syntaxKind, const BoundBinaryOperatorKind kind,
                        const std::type_info &operandType, const std::type_info &resultType);

    BoundBinaryOperator(const SyntaxKind syntaxKind, const BoundBinaryOperatorKind kind,
                        const std::type_info &leftType, const std::type_info &rightType,
                        const std::type_info &resultType);

    static std::shared_ptr<BoundBinaryOperator> Bind(const SyntaxKind syntaxKind,
                                                     const std::type_info &leftType,
                                                     const std::type_info &rightType);

    [[nodiscard]] SyntaxKind syntax_kind() const;
    [[nodiscard]] BoundBinaryOperatorKind kind1() const;
    [[nodiscard]] const std::type_info &left_type() const;
    [[nodiscard]] const std::type_info &right_type() const;
    [[nodiscard]] const std::type_info &result_type() const;

private:
    SyntaxKind syntaxKind;
    BoundBinaryOperatorKind kind;
    const std::type_info &leftType;
    const std::type_info &rightType;
    const std::type_info &resultType;

    static  std::vector<std::shared_ptr<BoundBinaryOperator>> operators;
};


class BoundUnaryOperator {
public:
    BoundUnaryOperator(const SyntaxKind syntaxKind, const BoundUnaryOperatorKind kind,
                       const std::type_info &operandType);

    BoundUnaryOperator(const SyntaxKind syntaxKind, const BoundUnaryOperatorKind kind,
                       const std::type_info &operandType, const std::type_info &resultType);

    static std::shared_ptr<BoundUnaryOperator> Bind(const SyntaxKind syntaxKind,
                                                    const std::type_info &operandType);

    [[nodiscard]] SyntaxKind syntax_kind() const;
    [[nodiscard]] BoundUnaryOperatorKind kind1() const;
    [[nodiscard]] const std::type_info &operand_type() const;
    [[nodiscard]] const std::type_info &result_type() const;

private:
    SyntaxKind syntaxKind;
    BoundUnaryOperatorKind kind;
    const std::type_info &operandType;
    const std::type_info &resultType;

    static  std::vector<std::shared_ptr<BoundUnaryOperator>> operators;
};

#endif //BOUNDOPERATORS_H
