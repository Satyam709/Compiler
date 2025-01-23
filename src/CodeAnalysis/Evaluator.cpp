#include "CodeAnalysis/Evaluator.h"
#include <format>
#include <sstream>

#include "CodeAnalysis/Binding/Binder.h"
#include "CodeAnalysis/Syntax/Expression.h"


Evaluator::Evaluator(const BoundExpression &root): _root(root) {
}

int Evaluator::evaluateExpression(const BoundExpression *node) {

    if (const auto unaryNode = dynamic_cast<const BoundUnaryExpression *>(node)) {
        const auto operand = evaluateExpression(unaryNode->getOperand());
        if (unaryNode->getOperatorKind() == BoundUnaryOperatorKind::Identity) {
            return operand;
        }
        if (unaryNode->getOperatorKind() == BoundUnaryOperatorKind::Negation) {
            return -1*operand;
        }

        throw std::runtime_error("Invalid unary operator" + boundKindsToString(unaryNode->getOperatorKind()));
    }

    if (const auto numberNode = dynamic_cast<const BoundLiteralExpression *>(node)) {
        return std::any_cast<int>(numberNode->getValue());
    }

    if (const auto binaryNode = dynamic_cast<const BoundBinaryExpression *>(node)) {
        const auto left = evaluateExpression(&binaryNode->left());
        const auto right = evaluateExpression(&binaryNode->right());

        if (binaryNode->getOperator() == BoundBinaryOperatorKind::Addition) {
            return left + right;
        }
        if (binaryNode->getOperator() == BoundBinaryOperatorKind::Subtraction) {
            return left - right;
        }
        if (binaryNode->getOperator() == BoundBinaryOperatorKind::Multiplication) {
            return left * right;
        }
        if (binaryNode->getOperator() == BoundBinaryOperatorKind::Division) {
            return left / right;
        }
        std::stringstream ss;
        ss << "Unexpected binary operator " << boundKindsToString(binaryNode->getOperator());
        throw std::runtime_error(ss.str());
    }
    // if (const auto parenthesisNode = dynamic_cast<ParenthesizedExpressionSyntax *>(node)) {
    //     return evaluateExpression(&parenthesisNode->expression());
    // }
    std::stringstream ss;
    ss << "Unexpected node <" << boundKindsToString(node->getKind()) << ">";
    throw std::runtime_error(ss.str());
}

int Evaluator::evaluate() {
    return evaluateExpression(&_root);
}
