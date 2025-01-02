
#include "Compiler/Evaluator.h"
#include <format>
#include <sstream>

#include "Compiler/Expression.h"
#include "Compiler/Syntax.h"
#include "Compiler/Binder.h"



Evaluator::Evaluator(BoundExpression &root): _root(root) {
}

int Evaluator::evaluateExpression(BoundExpression *node) {
    if (auto literalNode = dynamic_cast<BoundLiteralExpression<int> *>(node)) {
        return std::any_cast<int>(literalNode->_value);
    }

    if (auto unaryNode = dynamic_cast<BoundUnaryExpression *>(node)) {
        auto operand = evaluateExpression(unaryNode->_operand);
        switch(unaryNode->_op){
            case BoundUnaryOperatorKind::Identity:
                return operand;
            case BoundUnaryOperatorKind::Negation:
                return -operand;
            default:
                std::string err;
                switch(unaryNode->_op){
                    case BoundUnaryOperatorKind::Identity:
                        err = "Identity"; break;
                    case BoundUnaryOperatorKind::Negation:
                        err = "Negation"; break;
                }
                std::stringstream ss;
                ss << "Unexpected unary operator " << err;
                throw std::runtime_error(ss.str());
        }
    }

    if (auto binaryNode = dynamic_cast<BoundBinaryExpression *>(node)) {
        auto left = evaluateExpression(binaryNode->_left);
        auto right = evaluateExpression(binaryNode->_right);

        if (binaryNode->_op == BoundBinaryOperatorKind::Addition) {
            return left + right;
        }
        if (binaryNode->_op == BoundBinaryOperatorKind::Subtraction) {
            return left - right;
        }
        if (binaryNode->_op == BoundBinaryOperatorKind::Multiplication) {
            return left * right;
        }
        if (binaryNode->_op == BoundBinaryOperatorKind::Division) {
            return left / right;
        }
        std::string err;
        switch(binaryNode->_op){
            case BoundBinaryOperatorKind::Addition:
                err = "Addition"; break;
            case BoundBinaryOperatorKind::Subtraction:
                err = "Subtraction"; break;
            case BoundBinaryOperatorKind::Multiplication:
                err = "Multiplication"; break;
            case BoundBinaryOperatorKind::Division:
                err = "Division"; break;
        }
        std::stringstream ss;
        ss << "Unexpected binary operator " << err;
        throw std::runtime_error(ss.str());       
    }
    // if (const auto parenthesisNode = dynamic_cast<ParenthesizedExpressionSyntax *>(node)) {
    //     return evaluateExpression(&parenthesisNode->expression());
    // }
    throw std::runtime_error("Unexpected node");
}

int Evaluator::evaluate() {
    return evaluateExpression(&_root);
}
