#include "CodeAnalysis/Evaluator.h"
#include <format>
#include <sstream>
#include "CodeAnalysis/Syntax/Expression.h"


Evaluator::Evaluator(ExpressionSyntax &root): _root(root) {
}

int Evaluator::evaluateExpression(ExpressionSyntax *node) {
    if (const auto unaryNode = dynamic_cast<UnaryExpressionSyntax *>(node)) {
        const auto operand = evaluateExpression(unaryNode->operand());
        if (unaryNode->operatorToken().kind == SyntaxKind::PlusToken) {
            return operand;
        }
        if (unaryNode->operatorToken().kind == SyntaxKind::MinusToken) {
            return -1*operand;
        }

        throw std::runtime_error("Invalid unary operator" + syntaxKindToString(unaryNode->operatorToken().kind));
    }

    if (const auto numberNode = dynamic_cast<LiteralExpressionSyntax *>(node)) {
        return std::any_cast<int>(numberNode->getToken().val);
    }

    if (const auto binaryNode = dynamic_cast<BinaryExpressionSyntax *>(node)) {
        const auto left = evaluateExpression(&binaryNode->left());
        const auto right = evaluateExpression(&binaryNode->right());

        if (binaryNode->operator_token().kind == SyntaxKind::PlusToken) {
            return left + right;
        }
        if (binaryNode->operator_token().kind == SyntaxKind::MinusToken) {
            return left - right;
        }
        if (binaryNode->operator_token().kind == SyntaxKind::StarToken) {
            return left * right;
        }
        if (binaryNode->operator_token().kind == SyntaxKind::SlashToken) {
            return left / right;
        }
        std::stringstream ss;
        ss << "Unexpected binary operator " << binaryNode->operator_token();
        throw std::runtime_error(ss.str());
    }
    if (const auto parenthesisNode = dynamic_cast<ParenthesizedExpressionSyntax *>(node)) {
        return evaluateExpression(&parenthesisNode->expression());
    }
    std::stringstream ss;
    ss << "Unexpected node <" << syntaxKindToString(node->getKind()) << ">";
    throw std::runtime_error(ss.str());
}

int Evaluator::evaluate() {
    return evaluateExpression(&_root);
}
