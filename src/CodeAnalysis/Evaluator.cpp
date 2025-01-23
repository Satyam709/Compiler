#include "CodeAnalysis/Evaluator.h"
#include <format>
#include <sstream>

#include "CodeAnalysis/Binding/Binder.h"
#include "CodeAnalysis/Syntax/Expression.h"


Evaluator::Evaluator(const BoundExpression &root): _root(root) {
}

std::any Evaluator::evaluateExpression(const BoundExpression *node) {
    if (const auto unaryNode = dynamic_cast<const BoundUnaryExpression *>(node)) {
        auto operand = evaluateExpression(unaryNode->getOperand());
        if (unaryNode->getOperatorKind() == BoundUnaryOperatorKind::Identity) {
            return operand;
        }
        if (unaryNode->getOperatorKind() == BoundUnaryOperatorKind::Negation) {
            return -1 * std::any_cast<int>(operand);
        }

        throw std::runtime_error("Invalid unary operator" + boundKindsToString(unaryNode->getOperatorKind()));
    }

    if (const auto numberNode = dynamic_cast<const BoundLiteralExpression *>(node)) {
        try {
            return numberNode->getValue();
        } catch (std::bad_any_cast &e) {
            throw std::runtime_error("cant cast");
        }
    }

    if (const auto binaryNode = dynamic_cast<const BoundBinaryExpression *>(node)) {
        const auto left = std::any_cast<int>(evaluateExpression(&binaryNode->left()));
        const auto right = std::any_cast<int>(evaluateExpression(&binaryNode->right()));

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

std::any Evaluator::evaluate() {
    return evaluateExpression(&_root);
}
