#include "Evaluator.h"
#include <format>
#include <sstream>

#include "Binding/Binder.h"
#include "Binding/BoundBlockStatement.h"
#include "Binding/BoundExpressionStatement.h"
#include "Binding/BoundVariableDeclaration.h"
#include "Syntax/Expression.h"

Evaluator::Evaluator(const BoundStatement &root, std::unordered_map<VariableSymbol, std::any> &variables)
    : _root(root), _variables(variables) {
}

void Evaluator::EvaluateStatement(const BoundStatement *node) {
    switch (node->getKind()) {
        case BoundNodeKind::BlockStatement: {
            if (const auto it = dynamic_cast<const BoundBlockStatement *>(node)) {
                return EvaluateBlockStatement(it);
            }
            break;
        }
        case BoundNodeKind::ExpressionStatement: {
            if (const auto it = dynamic_cast<const BoundExpressionStatement *>(node)) {
                return EvaluateExpressionStatement(it);
            }
            break;
        }
        case BoundNodeKind::VariableDeclaration: {
            if (const auto it = dynamic_cast<const BoundVariableDeclaration *>(node)) {
                return EvaluateVariableDeclaration(it);
            }
            break;
        }
        default:
            throw std::runtime_error("Cannot evaluate: Invalid statement node kind");
    }
}

void Evaluator::EvaluateVariableDeclaration(const BoundVariableDeclaration *node) {
    const auto value = evaluateExpression(node->getInitializer());
    _variables[*node->getVariable()] = value;
    _lastValue = value;
}

void Evaluator::EvaluateBlockStatement(const BoundBlockStatement *node) {
    for (const auto statement: node->statements()) {
        EvaluateStatement(statement);
    }
}

void Evaluator::EvaluateExpressionStatement(const BoundExpressionStatement *node) {
    _lastValue = evaluateExpression(node->expression());
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
        if (unaryNode->getOperatorKind() == BoundUnaryOperatorKind::LogicalAnd) {
            return !std::any_cast<bool>(operand);
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

    if (const auto variableNode = dynamic_cast<const BoundVariableExpression *>(node)) {
        const VariableSymbol &symbol = variableNode->getVariable();
        auto it = _variables.find(symbol);
        if (it != _variables.end()) {
            return it->second;
        }
        throw std::runtime_error("Undefined variable: " + variableNode->getName());
    }

    if (const auto assignmentNode = dynamic_cast<const BoundAssignmentExpression *>(node)) {
        auto value = evaluateExpression(assignmentNode->getExpression());
        const VariableSymbol &symbol = assignmentNode->getVariable();
        _variables[symbol] = value;
        return value;
    }

    if (const auto binaryNode = dynamic_cast<const BoundBinaryExpression *>(node)) {
        const auto leftResult = evaluateExpression(&binaryNode->left());
        const auto rightResult = evaluateExpression(&binaryNode->right());

        switch (binaryNode->getOperator().kind1()) {
            // Evaluate boolean operations
            case BoundBinaryOperatorKind::LogicalAnd:
                return std::any_cast<bool>(leftResult) && std::any_cast<bool>(rightResult);

            case BoundBinaryOperatorKind::LogicalOR:
                return std::any_cast<bool>(leftResult) || std::any_cast<bool>(rightResult);

            case BoundBinaryOperatorKind::Equals: {
                // Check if both types are the same
                if (leftResult.type() == rightResult.type()) {
                    if (leftResult.type() == typeid(int)) {
                        return std::any_cast<int>(leftResult) == std::any_cast<int>(rightResult);
                    } else if (leftResult.type() == typeid(bool)) {
                        return std::any_cast<bool>(leftResult) == std::any_cast<bool>(rightResult);
                    }
                }
                throw std::runtime_error("Type mismatch or unsupported type for equality comparison");
            }

            case BoundBinaryOperatorKind::NotEquals: {
                if (leftResult.type() == rightResult.type()) {
                    if (leftResult.type() == typeid(int)) {
                        return std::any_cast<int>(leftResult) != std::any_cast<int>(rightResult);
                    } else if (leftResult.type() == typeid(bool)) {
                        return std::any_cast<bool>(leftResult) != std::any_cast<bool>(rightResult);
                    }
                }
                throw std::runtime_error("Type mismatch or unsupported type for equality comparison");
            }

            case BoundBinaryOperatorKind::Addition: {
                const auto left = std::any_cast<int>(leftResult);
                const auto right = std::any_cast<int>(rightResult);
                return left + right;
            }

            case BoundBinaryOperatorKind::Subtraction: {
                const auto left = std::any_cast<int>(leftResult);
                const auto right = std::any_cast<int>(rightResult);
                return left - right;
            }

            case BoundBinaryOperatorKind::Multiplication: {
                const auto left = std::any_cast<int>(leftResult);
                const auto right = std::any_cast<int>(rightResult);
                return left * right;
            }

            case BoundBinaryOperatorKind::Division: {
                const auto left = std::any_cast<int>(leftResult);
                const auto right = std::any_cast<int>(rightResult);
                if (right == 0) {
                    throw std::runtime_error("Division by zero");
                }
                return left / right;
            }

            case BoundBinaryOperatorKind::Less: {
                const auto left = std::any_cast<int>(leftResult);
                const auto right = std::any_cast<int>(rightResult);
                return left < right;
            }

            case BoundBinaryOperatorKind::Greater: {
                const auto left = std::any_cast<int>(leftResult);
                const auto right = std::any_cast<int>(rightResult);
                return left > right;
            }

            case BoundBinaryOperatorKind::LessOrEquals: {
                const auto left = std::any_cast<int>(leftResult);
                const auto right = std::any_cast<int>(rightResult);
                return left <= right;
            }

            case BoundBinaryOperatorKind::GreaterOrEquals: {
                const auto left = std::any_cast<int>(leftResult);
                const auto right = std::any_cast<int>(rightResult);
                return left >= right;
            }

            default: {
                std::stringstream ss;
                ss << "Unexpected binary operator " << boundKindsToString(binaryNode->getOperator().kind1());
                throw std::runtime_error(ss.str());
            }
        }
    }

    std::stringstream ss;
    ss << "Unexpected node <" << boundKindsToString(node->getKind()) << ">";
    throw std::runtime_error(ss.str());
}

std::any Evaluator::evaluate() {
    EvaluateStatement(&_root);
    return _lastValue;
}