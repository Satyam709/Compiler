#include "BoundNode.h"
#include <stdexcept>  // For std::runtime_error

class VariableSymbol;
// =============================================
// BoundLiteralExpression Implementation
// =============================================
BoundLiteralExpression::BoundLiteralExpression(std::any value)
    : _value(std::move(value)) {}

BoundNodeKind BoundLiteralExpression::getKind() const {
    return BoundNodeKind::LiteralExpression;
}

const std::type_info& BoundLiteralExpression::getType() const {
    return _value.type();
}

const std::any& BoundLiteralExpression::getValue() const {
    return _value;
}

// =============================================
// BoundBinaryExpression Implementation
// =============================================
BoundBinaryExpression::BoundBinaryExpression(const BoundExpression& left,
                                            const BoundBinaryOperator& op,
                                            const BoundExpression& right)
    : _left(left), _right(right), _operator(op) {}

BoundNodeKind BoundBinaryExpression::getKind() const {
    return BoundNodeKind::BinaryExpression;
}

const BoundExpression& BoundBinaryExpression::left() const {
    return _left;
}

const BoundExpression& BoundBinaryExpression::right() const {
    return _right;
}

BoundBinaryOperatorKind BoundBinaryExpression::operator_() const {
    return _operator.kind1();
}

const std::type_info& BoundBinaryExpression::getType() const {
    return _operator.result_type();
}

BoundBinaryOperator BoundBinaryExpression::getOperator() const {
    return _operator;
}

// =============================================
// BoundUnaryExpression Implementation
// =============================================
BoundUnaryExpression::BoundUnaryExpression(const BoundUnaryOperator op,
                                          const BoundExpression& operand)
    : _op(op), _operand(operand) {}

BoundNodeKind BoundUnaryExpression::getKind() const {
    return BoundNodeKind::UnaryExpression;
}

const std::type_info& BoundUnaryExpression::getType() const {
    return _op.result_type();
}

const BoundExpression* BoundUnaryExpression::getOperand() const {
    return &_operand;
}

BoundUnaryOperatorKind BoundUnaryExpression::getOperatorKind() const {
    return _op.kind1();
}

// =============================================
// BoundVariableExpression Implementation
// =============================================
BoundVariableExpression::BoundVariableExpression(const VariableSymbol& variable)
    : _variable(variable) {}

BoundNodeKind BoundVariableExpression::getKind() const {
    return BoundNodeKind::VariableExpression;
}

const std::type_info& BoundVariableExpression::getType() const {
    return _variable.getType();
}

const std::string& BoundVariableExpression::getName() const {
    return _variable.getName();
}

const VariableSymbol& BoundVariableExpression::getVariable() const {
    return _variable;
}

// =============================================
// BoundAssignmentExpression Implementation
// =============================================
BoundAssignmentExpression::BoundAssignmentExpression(const VariableSymbol& variable,
                                                    const BoundExpression* expression)
    : _variable(variable), _expression(expression) {}

BoundNodeKind BoundAssignmentExpression::getKind() const {
    return BoundNodeKind::AssignmentExpression;
}

const std::type_info& BoundAssignmentExpression::getType() const {
    return _expression->getType();
}

const std::string& BoundAssignmentExpression::getName() const {
    return _variable.getName();
}

const BoundExpression* BoundAssignmentExpression::getExpression() const {
    return _expression;
}

const VariableSymbol& BoundAssignmentExpression::getVariable() const {
    return _variable;
}

// =============================================
// Enum Conversion Helpers
// =============================================

std::string boundKindsToString(const BoundUnaryOperatorKind kind) {
    switch (kind) {
        case BoundUnaryOperatorKind::Identity: return "Identity";
        case BoundUnaryOperatorKind::Negation: return "Negation";
        case BoundUnaryOperatorKind::Invalid: return "Invalid";
        case BoundUnaryOperatorKind::LogicalAnd: return "LogicalAnd";
        case BoundUnaryOperatorKind::OnesComplement: return "OnesComplement";
        default: return "UnknownUnaryOperator";
    }
}

std::string boundKindsToString(const BoundBinaryOperatorKind kind) {
    switch (kind) {
        case BoundBinaryOperatorKind::Addition: return "Addition";
        case BoundBinaryOperatorKind::Subtraction: return "Subtraction";
        case BoundBinaryOperatorKind::Multiplication: return "Multiplication";
        case BoundBinaryOperatorKind::Division: return "Division";
        case BoundBinaryOperatorKind::Invalid: return "Invalid";
        case BoundBinaryOperatorKind::LogicalAnd: return "LogicalAnd";
        case BoundBinaryOperatorKind::LogicalOR: return "LogicalOR";
        case BoundBinaryOperatorKind::Equals: return "Equals";
        case BoundBinaryOperatorKind::NotEquals: return "NotEquals";
        case BoundBinaryOperatorKind::Less: return "Less";
        case BoundBinaryOperatorKind::LessOrEquals: return "LessOrEquals";
        case BoundBinaryOperatorKind::Greater: return "Greater";
        case BoundBinaryOperatorKind::BitwiseAnd: return "BitwiseAnd";
        case BoundBinaryOperatorKind::BitwiseOr: return "BitwiseOr";
        case BoundBinaryOperatorKind::BitwiseXor: return "BitwiseXor";
        case BoundBinaryOperatorKind::GreaterOrEquals: return "GreaterOrEquals";
        default: return "UnknownBinaryOperator";
    }
}

std::string boundKindsToString(const BoundNodeKind kind) {
    switch (kind) {
        case BoundNodeKind::UnaryExpression: return "UnaryExpression";
        case BoundNodeKind::LiteralExpression: return "LiteralExpression";
        case BoundNodeKind::BinaryExpression: return "BinaryExpression";
        case BoundNodeKind::VariableExpression: return "VariableExpression";
        case BoundNodeKind::AssignmentExpression: return "AssignmentExpression";
        case BoundNodeKind::BlockStatement: return "BlockStatement";
        case BoundNodeKind::ExpressionStatement: return "ExpressionStatement";
        case BoundNodeKind::VariableDeclaration: return "VariableDeclaration";
        case BoundNodeKind::IfStatement: return "IfStatement";
        case BoundNodeKind::WhileStatement: return "WhileStatement";
        case BoundNodeKind::ForStatement: return "ForStatement";
        default: return "UnknownNodeKind";
    }
}