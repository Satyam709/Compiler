
#include "Compiler/Evaluator.h"
#include <format>
#include <sstream>

#include "Compiler/Expression.h"


Evaluator::Evaluator(ExpressionSyntax &root): _root(root) {
}

int Evaluator::evaluateExpression(ExpressionSyntax *node) {
    if (auto numberNode = dynamic_cast<LiteralExpressionSyntax *>(node)) {
        return std::any_cast<int>(numberNode->getToken().val);
    }

    if (auto binaryNode = dynamic_cast<BinaryExpressionSyntax *>(node)) {
        auto left = evaluateExpression(&binaryNode->left());
        auto right = evaluateExpression(&binaryNode->right());

        if (binaryNode->operator_token().kind == PlusToken) {
            return left + right;
        }
        if (binaryNode->operator_token().kind == MinusToken) {
            return left - right;
        }
        if (binaryNode->operator_token().kind == StarToken) {
            return left * right;
        }
        if (binaryNode->operator_token().kind == SlashToken) {
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
